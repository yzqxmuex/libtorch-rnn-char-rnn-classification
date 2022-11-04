#ifndef _DATA_H
#define	_DATA_H

#include<torch/script.h>
#include <torch/torch.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <corecrt_io.h>

#include <iostream>
#include <cstring>        // for strcpy(), strcat()
#include <io.h>
#include <stdio.h>

#include"tools.hpp"

using namespace std;
using namespace torch;
using namespace cv;

//#define	DIRECTORYPATH	L"..\\data\\names"

#define		DEBUG_PRINT		0
//��������Ŀ¼:../data/names
/*1:���ȶ������е�����names�������category(����)		*/
/*2:Ȼ��������������µ���������						*/
/*3:���һ��tensor��״Ϊ{[���ֳ���],[0],[�����ַ���]}	*/
/*{[lens��], [0��], [��Ӧ�ַ�one-hot]}					*/

//�ṹ�嶨��
/*typedef struct ALL_CATEGORYS;		���������������							*/
/*tpedef map CATEGORY_ALL_LINES;	��������µ���������(LINES,һ��һ������)	*/
/*Ҫ�������ַ�����unicode-ansiת��												*/
/*tensor size {[lens], [0], [all_letters]}										*/

typedef struct stu_all_categorys
{
	std::string		strCommonType;
}STUALLCATEGORYS, *PSTUALLCATEGORYS;

typedef std::list< STUALLCATEGORYS > allCategorysList_t;

allCategorysList_t gAllCategorysList;

typedef std::map<std::string, enumerateLinesList_t> map_category_all_lines;
map_category_all_lines gMapCategoryAllLines;

int getAllCategorys(wchar_t *pwszDirectoryPath, allCategorysList_t &List)
{
	EnumerateFileInDirectory(pwszDirectoryPath);

	enumerateFilesList_t::iterator iter;
	for (iter = enumerateFilesList.begin(); iter != enumerateFilesList.end(); iter++)
	{
		int sublen = iter->strCommonType.find(".");
		std::string strCategory = iter->strCommonType.substr(0, sublen);
		STUALLCATEGORYS categorys;
		categorys.strCommonType = strCategory;
		ListAppend(List, categorys);
	}

#if DEBUG_PRINT
	ListForEachElem(List);
#endif

	return 0;
}

int getAllLinesForCategorys(wchar_t *pwszFilesPath)
{
	enumerateFilesList_t::iterator iter;
	for (iter = enumerateFilesList.begin(); iter != enumerateFilesList.end(); iter++)
	{
		std::wstring wstrFilesPath = DIRECTORYPATH;
		wstrFilesPath += L"\\";
		std::wstring wstr = Acsi2WideByteEx(iter->strCommonType);
		wstrFilesPath += wstr;
		STUENUMERATELINES linsInfo;
		enumerateLinesList_t eLinesList;
		ReadContentFromFiles(eLinesList, linsInfo, wstrFilesPath.c_str());
		int sublen = iter->strCommonType.find(".");
		std::string strCategory = iter->strCommonType.substr(0, sublen);

		gMapCategoryAllLines.insert(map_category_all_lines::value_type(strCategory, eLinesList));

	}

#if DEBUG_PRINT
	printf("verify category all lines map:\n");
	map_category_all_lines::iterator itMap;
	enumerateLinesList_t::iterator iterList;
	itMap = gMapCategoryAllLines.find("Dutch");
	if (itMap != gMapCategoryAllLines.end())
	{
		for (iterList = itMap->second.begin(); iterList != itMap->second.end(); iterList++)
		{
			std::cout << iterList->strCommonType << std::endl;
		}
	}
#endif

}

/*����:������											*/
/*����:2022-10-31										*/
/*����:letterToIndex										*/
/*����:��ô�СдӢ����ĸ��Ӧ��˳����������one-hot����*/
/*����:��Ӧ����ĸ										*/
/*���:��Ӧ��ĸ����ĸ�����˳����						*/
	
char gszAll_Letters[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
						'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
						'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
						'w', 'x', 'y', 'z', ' ', '.', ',', ';', '\'', '-' };

int gn_letters = sizeof(gszAll_Letters);

int letterToIndex(char szLetter)
{
	int seq = 0;
	for (seq = 0; seq < sizeof(gszAll_Letters); seq++)
	{
		if (gszAll_Letters[seq] == szLetter)
			break;
	}

	if (seq == sizeof(gszAll_Letters))
		return -1;

	return seq;
}

/*����:������											*/
/*����:2022-11-1										*/
/*����:lineToTensor										*/
/*����:��line��one-hot�ķ�ʽת����tensor				*/
/*<line_length x 1 x n_letters>							*/
/*����:line												*/
/*���:line��Ӧ��one-hot tensor							*/

torch::Tensor lineToTensor(const char* szLine)
{
	LONGLONG lLineLen = strlen(szLine);
	torch::Tensor tensor = torch::zeros({ lLineLen, 1, sizeof(gszAll_Letters) });

	for (int i = 0; i < lLineLen; i++)
	{
		tensor[i][0][letterToIndex(szLine[i])] = 1;
	}

#if DEBUG_PRINT
	cout << "one-hot line tensor size:\n"<< tensor.data().sizes() << endl;
	cout << "one-hot line tensor:\n" << tensor.data() << endl;
#endif

	return tensor;
}
#endif // _DATA_H
