#ifndef _PREDICT_H
#define	_PREDICT_H

#include<torch/script.h>
#include <torch/torch.h>

torch::Tensor evaluate(RNN& rnn, torch::Device device, torch::Tensor line_tensor)
{
	rnn.to(device);
	rnn.train(false);

	torch::Tensor output, hidden;

	hidden = rnn.initHidden();

	int nline_tensor_size = 0;
	nline_tensor_size = line_tensor.sizes()[0];

	for (int i = 0; i < nline_tensor_size; i++)
	{
		std::tuple<torch::Tensor, torch::Tensor> ret = rnn.forward(line_tensor[i], hidden);
		output = std::get<0>(ret);
		hidden = std::get<1>(ret);
	}

	return output;
}

void predict(std::string line, int n_predictions, RNN& rnn, torch::Device device)
{
	torch::Tensor output = evaluate(rnn, device, lineToTensor(line.c_str()));

	std::tuple<torch::Tensor, torch::Tensor> topRet = output.topk(n_predictions, 1, true);

	torch::Tensor topv = std::get<0>(topRet);
	torch::Tensor topi = std::get<1>(topRet);

	for (int i = 0; i < n_predictions; i++)
	{
		float value = topv[0][i].item<float>();
		int category_index = topi[0][i].item<int>();
		printf("(%.2f) %s\n", value, FindListElem(gAllCategorysList, category_index).c_str());
	}
}

#endif // !_PREDICT_H
