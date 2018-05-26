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
}


ClientCon::~ClientCon(void)
{
	closesocket(sClient);
	WSACleanup();
}

void ClientCon::StartConnect(string sAddress, int iPort, string sUsername)
{
    struct sockaddr_in server;
    char *message , server_reply[2048];
    int recv_size;
	m_pUser = sUsername;

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return;
    }
     
    printf("Initialised.\n");
     
    //Create a socket
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
    while((recv_size = recv(sClient, server_reply , 2048, 0)) != SOCKET_ERROR)
    { 
		//Add a NULL terminating character to make it a proper string before printing
		server_reply[recv_size] = '\0';

		string sTempMsg1 = string(server_reply);
		string sTempMsg ="\n" + string(server_reply) + "\n";
		if (!(isFile) && (sTempMsg1.find(".txt", 4) < 100))
		{
			isFile = true;
			file = fu.createFile(server_reply);//创建文件准备存放数据
			//filename = server_reply;//保存文件名
			if (file != NULL)//成功创建文件
			{
				while (true)
				{
					fd_set	recvfd;
					struct timeval tv = { 3, 0 };

					FD_ZERO(&recvfd);
					FD_SET(sClient, &recvfd);

					//检测有没有数据到达socket,有->接收数据，无->break
					int result = select(0, &recvfd, NULL, NULL, &tv);
					if (result > 0)
					{
						//接受数据
						recv_size = recv(sClient, server_reply, 2048, 0);
						server_reply[recv_size] = '\0';//添加文件结尾标志
						string sTemp = string(server_reply);
						if ((sTemp == "over") || (recv_size == 0))
						{
							break;
						}
						else
						{
							//循环一直写入文件
							fwrite(server_reply, 1, recv_size, file);
							//发送确认信息(防止数据发送过快来不及接收)
							//send(sClient, "ready", 5, 0);
						}
					}
					else
					{
						break;
					}
				}
				fclose(file);
				isFile = false;
			}
		}
		/*else if (sTempMsg1 == "ready")
		{
			isReady = true;
		}*/
		else
		{
			m_pClient->ShowServerInfo(sTempMsg);
		}
    }
}

void ClientCon::SendData(string sMessage)
{
	string sTemp = m_pUser + ">>" + sMessage + "\n";

	if( send(sClient, sTemp.c_str(), sTemp.size() , 0) < 0)
    {
        puts("Send failed");
        return;
    }
}

void ClientCon::SendFile(FILE* file, CString filename) 
{
	FileUtil fu;
	CString a = CString(".txt");
	filename.Append(a);//加上后缀
	char *data = fu.CString2Char(filename);
	send(sClient, data, filename.GetLength(), 0);//先发送文件名，让接收端做好接受准备

	const int BUFSIZE = 512;
	char sendData[BUFSIZE];
	char recData[20] = {'0'};
	int nCount;//nCount：实际读取的字符数量
	while ((nCount = fread(sendData, 1, BUFSIZ, file)) > 0)//每次读取2048个字符一起发送
	{
		send(sClient, sendData, nCount, 0);
		//while (!isReady);//阻塞等待接收方发送确认
		/*int ret = recv(sClient, recData, 20, 0);//阻塞等待，用于控制数据流量
		if (ret > 0)
		{
			recData[ret] = '\0';
		}*/
	}
	//Sleep(100);
	//send(sClient, "over", 4, 0);//发送文件发送完毕标志
}