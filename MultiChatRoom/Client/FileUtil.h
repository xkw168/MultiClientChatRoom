#pragma once
#include <stdio.h>
#include <WINSOCK2.H>
#include <io.h> 
#include <string>
#pragma  comment(lib,"ws2_32.lib")
using namespace std;

static class FileUtil
{
private:
	FILE *f;
public:
	FILE * openFile(CString filename)
	{
		char* name = CString2Char(filename);
		if (f = fopen(name, "r"))
		{
			printf("�ļ��򿪳ɹ�\n");
			return f;
		}
		else
		{
			printf("�ļ�������\n");
			return NULL;
		}
	}

	FILE * createFile(char *name)
	{
		//����һ���յ��ļ�������ͬ���ļ�����������ɾ��
		if (f = fopen(name, "w"))
		{
			printf("�ļ������ɹ�\n");
		}
		else
		{
			printf("�ļ�����ʧ��\n");
		}
		return f;
	}

	bool createDir(char *dir)
	{
		char head[MAX_PATH] = "md ";
		return system(strcat(head, dir));
	}

	char* CString2Char(CString string) 
	{
		int n = string.GetLength(); //��ȡstr���ַ���  
		char *pChar = new char[n + 1]; //���ֽ�Ϊ��λ  
		WideCharToMultiByte(CP_ACP, 0, string, n, pChar, n + 1, NULL, NULL); //���ֽڱ���ת���ɶ��ֽڱ���  
		pChar[n] = '\0'; //���ֽ��ַ���'\0'����
		return pChar;
	}

	char* string2Char(string string)
	{
		return (char *)string.data();
	}
};