#ifndef		_MODEL_H
#define		_MODEL_H

#include<torch/script.h>
#include <torch/torch.h>

struct RNN : public torch::nn::Module
{
	RNN(int input_size, int hidden_size, int output_size)
	{
		m_hidden_size = hidden_size;

		i2h = register_module("i2h", torch::nn::Linear(input_size + hidden_size, hidden_size));
		i2o = register_module("i2o", torch::nn::Linear(input_size + hidden_size, output_size));
		softmax = register_module("softmax", torch::nn::LogSoftmax(torch::nn::LogSoftmaxOptions(1)));
	}

	~RNN()
	{

	}

	std::tuple<torch::Tensor, torch::Tensor> forward(torch::Tensor input, torch::Tensor hidden)
	{
		//相同tensor.type才能够cat
		combined = torch::cat({ input, hidden }, 1);
		hidden = i2h(combined);
		output = i2o(combined);
		output = softmax(output);
		//output = torch::log_softmax(output, 1);	后备方案，此句可用
		return { output, hidden };
	}

	torch::Tensor initHidden()
	{
		return torch::zeros({1, m_hidden_size });
	}

	int m_hidden_size = 0;

	torch::nn::Linear i2h{ nullptr }, i2o{ nullptr };
	torch::nn::LogSoftmax softmax = nullptr;
	torch::Tensor combined, output;
};

#endif // _MODEL_H

