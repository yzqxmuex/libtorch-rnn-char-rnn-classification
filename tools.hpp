#ifndef _TOOS_H
#define	_TOOS_H

#include <windows.h>
#include<iostream>
#include<sstream>
#include<fstream>

using namespace std;

//unicode תΪ ascii 
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

//ascii ת Unicode
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

/*����:������															*/
/*����:2022-10-31														*/
/*����:List_Append��ListForEachElem										*/
/*GetListElemCount��ClearAllListElem									*/
/*����:���Ԫ�ص�List�С�����List�е�����Ԫ��							*/
/*���List������Ԫ������,�������List									*/
/*����:listģ��,elemģ��												*/
/*���:��μ��ǳ���														*/

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

/*����:������															*/
/*����:2022-10-31														*/
/*����:FindListElem														*/
/*����:����Index��ȡList�ж�ӦԪ��										*/
/*����:listģ��,�����													*/
/*����:��ȡList�ж�ӦԪ��												*/
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

/*����:������															*/
/*����:2022-10-31														*/
/*����:EnumerateFileInDirectory											*/
/*����:����Ŀ¼�µ��ļ�													*/
/*����:Ŀ¼·��,UNICODE													*/
/*���:�Ƿ�ɹ�															*/

BOOL EnumerateFileInDirectory(wchar_t *pwszDirectoryPath)
{
	WIN32_FIND_DATA		FindFileData;
	HANDLE				hListFile;
	WCHAR				szFilePath[MAX_PATH];

	// ���������Ŀ¼���ļ���·�����ַ�����ʹ��ͨ���"*"
	lstrcpy(szFilePath, pwszDirectoryPath);
	// ע�͵Ĵ���������ڲ��������ԡ�.txt����β���ļ�
	lstrcat(szFilePath, L"\\*.txt");
	//lstrcat(szFilePath, L"\\*");

	// ���ҵ�һ���ļ�/Ŀ¼����ò��Ҿ��
	hListFile = FindFirstFile(szFilePath, &FindFileData);
	// �жϾ��
	if (hListFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	else
	{
		do
		{
			/* ���������ʾ������Ŀ¼���ϼ�Ŀ¼�ġ�.���͡�..����
			����ʹ��ע�Ͳ��ֵĴ������*/
			if (lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 ||
				lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)
			{
				continue;
			}

			// ��ӡ�ļ�����Ŀ¼��
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

/*����:������						*/
/*����:2022-10-31					*/
/*����:ReadContentFromFiles			*/
/*����:��ȡ�ļ���ÿ�е�����			*/
/*����:�ļ�·��,UNICODE				*/
/*���:�Ƿ�ɹ�						*/

BOOL ReadContentFromFiles(enumerateLinesList_t &list, STUENUMERATELINES &content, const wchar_t * pwszFilePath)
{
	ifstream fin;
	fin.open(pwszFilePath, ios::in);
	if (!fin.is_open())
	{
		cout << "�޷��ҵ�����ļ���" << endl;
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
