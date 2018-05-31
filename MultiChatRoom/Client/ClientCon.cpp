/*
Coded by Robel Sharma
Date: 20-08-2013
If you use in any product please
make sure to write my credits

*/

#include "stdafx.h"
#include "ClientCon.h"
#include "ClientDlg.h"
#include "FileUtil.h"
 
#pragma comment(lib,"ws2_32.lib") //Winsock Library

ClientCon::ClientCon(CClientDlg *dlg)
{
	m_pClient = dlg;
	isFile = false;
	isReady = false;
	sClient = 1;
}


ClientCon::~ClientCon(void)
{
	closesocket(sClient);
	WSACleanup();
}

void ClientCon::StartConnect(string sAddress, int iPort, string sUsername)
{
	const int REC_BUFERSIZE = 2048;
    struct sockaddr_in server;
    char *message , server_reply[REC_BUFERSIZE + 1];
    int recv_size;
	m_pUser = sUsername;

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return;
    }
     
    printf("Initialised.\n");
     
    //Create a socket(SOCK_STREAM: connection-oriented)
    if((sClient = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
 
    printf("Socket created.\n");
     
     
	server.sin_addr.s_addr = inet_addr(sAddress.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons( iPort );
 
    //Connect to remote server
    if (connect(sClient, (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return;
    }
    
    //Receive data from the server
    while((recv_size = recv(sClient, server_reply , REC_BUFERSIZE, 0)) != SOCKET_ERROR)
    { 
		//Add a NULL terminating character to make it a proper string before printing
		server_reply[recv_size] = '\0';
		
		string sTempMsg1 = string(server_reply);
		string sTempMsg ="\n" + string(server_reply) + "\n";
		//�ж��Ƿ�����ļ�.txt��־
		if (!(isFile) && (sTempMsg1.find(".txt") != string::npos))
		//if(isFile)
		{
			isFile = true;
			file = fu.createFile(server_reply);//�����ļ�׼���������
			if (file != NULL)//�ɹ������ļ�
			{
				bool isOver = false;
				int i;
				while (true)
				{
					/*fd_set	recvfd;
					struct timeval tv = { 3, 0 };

					FD_ZERO(&recvfd);
					FD_SET(sClient, &recvfd);

					//�����û�����ݵ���socket,��->�������ݣ���->break
					int result = select(0, &recvfd, NULL, NULL, &tv);*/
					//��������
					recv_size = recv(sClient, server_reply, 2048, 0);
					server_reply[recv_size] = '\0';//����ļ���β��־
					Sleep(500);
					//�ж��Ƿ��й涨���ļ�������־
					for (i = 0; i < (recv_size - 4); i++)
					{
						if (server_reply[i] == 'O' &&
							server_reply[i + 1] == 'V' &&
							server_reply[i + 2] == 'E' &&
							server_reply[i + 3] == 'R' &&
							server_reply[i + 4] == '!')
						{
							isOver = true;
							break;
						}

					}
					if (isOver)
					{
						recv_size = (i == recv_size - 4) ? recv_size : (i - 1);
						recv_size = (recv_size < 0) ? 0 : recv_size;
						fwrite(server_reply, 1, recv_size, file);
						break;
					}
					else
					{
						//ѭ��һֱд���ļ�
						fwrite(server_reply, 1, recv_size, file);
					}
				}
				fclose(file);
				isFile = false;
			}
		}
		else
		{
			if (sTempMsg1 == "READY!")
			{
				isReady = true;
			}
			else//������Ϣ����ӡ
			{
				m_pClient->ShowServerInfo(sTempMsg);
			}
		}
    }
}

void ClientCon::SendData(string sMessage, bool showName)
{
	isReady = false;
	if (sMessage.length() > 0)
	{
		string sTemp = m_pUser + ">>" + sMessage + "\n";

		if (send(sClient, sTemp.c_str(), sTemp.size(), 0) < 0)
		{
			puts("Send failed");
			return;
		}
		else
		{
			sTemp = "����" + m_pUser + ")>>" + sMessage + "\n";
			//��ʾ�Լ����͵���Ϣ
			m_pClient->ShowServerInfo(sTemp);
		}
	}
}

void ClientCon::SendFile(FILE* file, CString filename) 
{
	FileUtil fu;
	CString a = CString(".txt");
	filename.Append(a);//���Ϻ�׺
	char *data = fu.CString2Char(filename);
	isReady = false;
	send(sClient, data, filename.GetLength(), 0);//�ȷ����ļ������ý��ն����ý���׼��
	while (!isReady);
	isReady = false;
	Sleep(100);
	const int SEND_BUFSIZE = 512;
	char sendData[SEND_BUFSIZE + 1];
	char recData[20] = {'0'};
	int nCount;//nCount��ʵ�ʶ�ȡ���ַ�����
	while ((nCount = fread(sendData, 1, SEND_BUFSIZE, file)) > 0)//ÿ�ζ�ȡ512���ַ�һ����
	{
		//sendData[nCount] = '\0';
		send(sClient, sendData, nCount, 0);
		while (!isReady);//�����ȴ����շ�����ȷ��
		isReady = false;
	}
	Sleep(500);
	send(sClient, "OVER!", 5, 0);//�����ļ�������ϱ�־
	fclose(file);
}