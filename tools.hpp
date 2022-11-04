#ifndef _TOOS_H
#define	_TOOS_H

#include <windows.h>
#include<iostream>
#include<sstream>
#include<fstream>

using namespace std;

//unicode 转为 ascii 
std::string WideByte2AcsiEx(std::wstring& wstrcode)
{
	int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, NULL, 0, NULL, NULL);
	if (asciisize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (asciisize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<char> resultstring(asciisize);
	int convresult = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, &resultstring[0], asciisize, NULL, NULL);
	if (convresult != asciisize)
	{
		throw std::exception("La falla!");
	}
	return std::string(&resultstring[0]);
}

//ascii 转 Unicode
std::wstring Acsi2WideByteEx(std::string& strascii)
{
	int widesize = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<wchar_t> resultstring(widesize);
	int convresult = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, &resultstring[0], widesize);
	if (convresult != widesize)
	{
		throw std::exception("La falla!");
	}
	return std::wstring(&resultstring[0]);
}

typedef struct stu_enumerateFiles
{
	std::string		strCommonType;
}STUENUMERATEFILES, *PSTUENUMERATEFILES;

typedef std::list< STUENUMERATEFILES > enumerateFilesList_t;

enumerateFilesList_t enumerateFilesList;

typedef struct stu_enumreateLines
{
	std::string		strCommonType;
}STUENUMERATELINES, *PSTUENUMERATELINES;

typedef std::list< STUENUMERATELINES > enumerateLinesList_t;

/*作者:杨智勤															*/
/*日期:2022-10-31														*/
/*函数:List_Append、ListForEachElem										*/
/*GetListElemCount、ClearAllListElem									*/
/*功能:添加元素到List中、遍历List中的所有元素							*/
/*获得List中所有元素总数,清空整个List									*/
/*输入:list模板,elem模板												*/
/*输出:入参即是出参														*/

template<typename T1, typename T2>
void ListAppend(T1 &list, T2 &elem)
{
	list.push_back(elem);
}

template<typename T>
void ListForEachElem(T &List)
{
	T::iterator iter;
	for (iter = List.begin(); iter != List.end(); iter++)
	{
		std::cout << iter->strCommonType << std::endl;
	}
}

template<typename T>
int GetListElemCount(T &List)
{
	int nListElemCount = 0;
	T::iterator iter;
	for (iter = List.begin(); iter != List.end(); iter++)
	{
		
		nListElemCount++;
	}
	return nListElemCount;
}

template<typename T>
void ClearAllListElem(T &List)
{
	List.clear();
}

/*作者:杨智勤															*/
/*日期:2022-10-31														*/
/*函数:FindListElem														*/
/*功能:根据Index获取List中对应元素										*/
/*输入:list模板,随机数													*/
/*返回:获取List中对应元素												*/
template<typename T1, typename T2>
std::string FindListElem(T1 List, T2 nRandIndex)
{
	int nListElemCount = 0;
	T1::iterator iter;
	for (iter = List.begin(); iter != List.end(); iter++)
	{
		if (nListElemCount == nRandIndex)
			break;
		nListElemCount++;
	}
	return iter->strCommonType;
}

template<typename T1, typename T2>
int FindListElemSeq(T1 List, T2 param)
{
	int nListElemCount = 0;
	T1::iterator iter;
	for (iter = List.begin(); iter != List.end(); iter++)
	{
		if (0 == iter->strCommonType.compare(param))
			break;
		nListElemCount++;
	}
	return nListElemCount;
}

/*作者:杨智勤															*/
/*日期:2022-10-31														*/
/*函数:EnumerateFileInDirectory											*/
/*功能:遍历目录下的文件													*/
/*输入:目录路径,UNICODE													*/
/*输出:是否成功															*/

BOOL EnumerateFileInDirectory(wchar_t *pwszDirectoryPath)
{
	WIN32_FIND_DATA		FindFileData;
	HANDLE				hListFile;
	WCHAR				szFilePath[MAX_PATH];

	// 构造代表子目录和文件夹路径的字符串，使用通配符"*"
	lstrcpy(szFilePath, pwszDirectoryPath);
	// 注释的代码可以用于查找所有以“.txt”结尾的文件
	lstrcat(szFilePath, L"\\*.txt");
	//lstrcat(szFilePath, L"\\*");

	// 查找第一个文件/目录，获得查找句柄
	hListFile = FindFirstFile(szFilePath, &FindFileData);
	// 判断句柄
	if (hListFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	else
	{
		do
		{
			/* 如果不想显示代表本级目录和上级目录的“.”和“..”，
			可以使用注释部分的代码过滤*/
			if (lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 ||
				lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)
			{
				continue;
			}

			// 打印文件名、目录名
			//printf("%ws\t\t", FindFileData.cFileName);
			std::wstring unicode = FindFileData.cFileName;
			std::string ansi = WideByte2AcsiEx(unicode);
			STUENUMERATEFILES enumerateFile;
			enumerateFile.strCommonType = ansi;
			ListAppend(enumerateFilesList, enumerateFile);
		} while (FindNextFile(hListFile, &FindFileData));
	}
	return TRUE;
}

/*作者:杨智勤						*/
/*日期:2022-10-31					*/
/*函数:ReadContentFromFiles			*/
/*功能:读取文件中每行的内容			*/
/*输入:文件路径,UNICODE				*/
/*输出:是否成功						*/

BOOL ReadContentFromFiles(enumerateLinesList_t &list, STUENUMERATELINES &content, const wchar_t * pwszFilePath)
{
	ifstream fin;
	fin.open(pwszFilePath, ios::in);
	if (!fin.is_open())
	{
		cout << "无法找到这个文件！" << endl;
		return FALSE;
	}
	char buff[1024] = { 0 };
	while (fin >> buff)
	{
		content.strCommonType = buff;
		ListAppend(list, content);
	}
	fin.close();

	return TRUE;
}

#endif // !_TOOS_H
