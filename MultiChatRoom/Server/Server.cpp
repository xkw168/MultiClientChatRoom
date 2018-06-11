// Serve_win32.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ServeManager.h"
#include <Windows.h>
#include <iostream>
using namespace std;

DWORD WINAPI ThreadFunc(LPVOID lpParameter);

ServeManager *m_pServer;
BOOL CtrlHandler(DWORD fdwCtrlType);

int main()
{
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE))
	{
		//printf("����WINDOWS API����-->SetConsoleCtrlHandler����.\n");
		int iPort = 1680;
		m_pServer = new ServeManager();
		m_pServer->StartListening(iPort);
	}
	else
	{
		printf("ERROR: could not set control handler.\n");
	}
		
    return 0;
}

BOOL CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	/* handle the CTRL-CLOSE signal */
	case CTRL_CLOSE_EVENT:
		m_pServer->ClearServer();
		return TRUE;

	default:
		return FALSE;
	}
}