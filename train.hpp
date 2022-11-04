#ifndef _TRAIN_H
#define	_TRAIN_H

#include<torch/script.h>
#include <torch/torch.h>

#include <stdlib.h>
#include <time.h>

#include "model.hpp"

using namespace std;

template<typename T>
std::string randomChoice(/*allCategorysList_t*/T l)
{
	int nlMaxSize = l.size();
	int nRandElem = rand() % nlMaxSize;

	std::string strElem = FindListElem(l, nRandElem);

#if DEBUG_PRINT
	cout << "nRandElem : " << nRandElem << " strElem: " << strElem << endl;
#endif

	return strElem;
}

std::string SelectElemFromMap(map_category_all_lines Map, std::string strCategory)
{
	bool bRes = false;
	map_category_all_lines::iterator	itMap;
	std::string strRandLine;

	itMap = gMapCategoryAllLines.find(strCategory);
	if (itMap != gMapCategoryAllLines.end())
	{
		strRandLine = randomChoice(itMap->second);
	}

	return strRandLine;
}

std::tuple<torch::Tensor, torch::Tensor> randomTrainingPair(std::string &category, std::string &line/*, torch::Tensor &incategory_tensor, torch::Tensor &inline_tensor*/)
{
	std::string	strRandCategory, strRandLine;
	strRandCategory = randomChoice(gAllCategorysList);
	strRandLine = SelectElemFromMap(gMapCategoryAllLines, strRandCategory);

	category = strRandCategory;
	line = strRandLine;

	int nCategoryIndex = FindListElemSeq(gAllCategorysList, strRandCategory);
	torch::Tensor category_tensor = torch::zeros({1}, torch::kLong);
	category_tensor[0] = nCategoryIndex;
	torch::Tensor line_tensor = lineToTensor(strRandLine.c_str());

	return { category_tensor , line_tensor };
}

std::tuple<torch::Tensor, torch::Tensor>  train(RNN& rnn, torch::Device device, torch::Tensor category_tensor, torch::Tensor line_tensor, torch::optim::Optimizer& optimizer, torch::nn::NLLLoss& criterion)
{
	rnn.to(device);
	rnn.train(true);

	torch::Tensor hidden = rnn.initHidden();
	optimizer.zero_grad();

	int nline_tensor_size = 0;
	nline_tensor_size = line_tensor.sizes()[0];

	torch::Tensor output;

	for (int i = 0; i < nline_tensor_size; i++)
	{
		std::tuple<torch::Tensor, torch::Tensor> ret = rnn.forward(line_tensor[i], hidden);
		output = std::get<0>(ret);
		hidden = std::get<1>(ret);
	}

	auto loss = criterion(output, category_tensor);
	loss.backward();

	optimizer.step();

	return { output, loss };

}

std::string categoryFromOutput(torch::Tensor output)
{
	std::tuple<torch::Tensor, torch::Tensor> topRet = output.topk(1);	//# Tensor out of Variable with.data
	torch::Tensor top_n = std::get<0>(topRet);
	torch::Tensor top_i = std::get<1>(topRet);

	int category_i = top_i[0][0].item<int>();

	std::string strCategory = FindListElem(gAllCategorysList, category_i);

	return strCategory;
}

#endif // !_TRAIN_H
