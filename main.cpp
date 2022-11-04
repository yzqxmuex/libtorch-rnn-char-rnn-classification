#include <torch/torch.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <corecrt_io.h>

#include <iostream>
#include <cstring>        // for strcpy(), strcat()
#include <io.h>
#include <stdio.h>

#define	_TRAIN_STAGE_	0

#define	DIRECTORYPATH	L"..\\data\\names"

//自定义头文件
#include "version.h"
#include "data.hpp"
#include "model.hpp"
#include "train.hpp"
#include "predict.hpp"

using namespace std;
using namespace torch;
using namespace cv;

torch::DeviceType device_type;

int n_hidden = 128;
int n_epochs = 100000;
int print_every = 5000;
int plot_every = 1000;
float learning_rate = 0.005;				//If you set this too high, it might explode.If too low, it might not learn
int gn_categories = 0;

int main()
{
	if (torch::cuda::is_available())
	{
		printf("torch::cuda::is_available\n");
		device_type = torch::kCUDA;
	}
	else
	{
		printf("cpu is_available\n");
		device_type = torch::kCPU;
	}
	torch::Device device(device_type);

#if _TRAIN_STAGE_

	srand((unsigned)time(NULL));

	//数据预处理
	getAllCategorys(DIRECTORYPATH, gAllCategorysList);
	getAllLinesForCategorys(DIRECTORYPATH);

	gn_categories = GetListElemCount(gAllCategorysList);

	//输入为所有字符总数,隐藏层节点数自定义,输出要分类的类别数量
	RNN rnn(gn_letters, n_hidden, gn_categories);

	auto optimizer = torch::optim::SGD(rnn.parameters(), torch::optim::SGDOptions(learning_rate));
	auto criterion = torch::nn::NLLLoss();

	for (int epoch = 0; epoch < n_epochs; epoch ++)
	{
		std::string category, line;
		//torch::Tensor category_tensor, line_tensor;

		std::tuple<torch::Tensor, torch::Tensor> ret = randomTrainingPair(category, line/*, category_tensor, line_tensor*/);
		torch::Tensor category_tensor = std::get<0>(ret);
		torch::Tensor line_tensor = std::get<1>(ret);
		std::tuple<torch::Tensor, torch::Tensor> retTran = train(rnn, device, category_tensor, line_tensor, optimizer, criterion);
		torch::Tensor output = std::get<0>(retTran);
		torch::Tensor loss = std::get<1>(retTran);

		//Print epoch number, loss, name and guess
		if (0 == (epoch % print_every))
		{
			std::string guss = categoryFromOutput(output);
			if (0 == guss.compare(category))
				printf("%s %s right\n", guss.c_str(), category.c_str());
			printf("loss: %.3f\n", loss.item().toFloat());
		}
	}

	printf("Finish training!\n");
	torch::serialize::OutputArchive archive;
	rnn.save(archive);
	archive.save_to("..\\retModel\\char-rnn-classification-c++.pt");
	printf("Save the training result to ..\\char-rnn-classification-c++.pt.\n");

#endif // _TRAIN_STAGE_

	srand((unsigned)time(NULL));

	//数据预处理
	getAllCategorys(DIRECTORYPATH, gAllCategorysList);
	gn_categories = GetListElemCount(gAllCategorysList);

	RNN		rnn(gn_letters, n_hidden, gn_categories);
	torch::serialize::InputArchive archive;
	archive.load_from("..\\retModel\\char-rnn-classification-c++.pt");

	rnn.load(archive);

	std::string line = "Leaver";
	predict(line,3, rnn, device);

	return 0;
}