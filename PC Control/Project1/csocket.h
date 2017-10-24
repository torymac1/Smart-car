#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define BUFSIZE 512

#define MAX_SIZE 40000
#define SOCKSIZE 4000

class CSocket
{
private:
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	int iResult;
	char tmp[BUFSIZE];
	const char *PORT = "2001";
	const char *HOST = "192.168.1.1";
	int char2int(char input);
public:
	char buffer[BUFSIZE];
	CSocket();
	int Send(const char *buf);
	void Close();
	void Hexstring(const char *src, char* target);
	void SendByte(const char *src);
};

CSocket::CSocket()
{
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failure: %d\n", iResult);
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo(HOST, PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return;
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return;
	}

	//addrServer.sin_family = AF_INET;
	//addrServer.sin_port = PORT;
	//inet_pton(AF_INET, HOST, &(addrServer.sin_addr));

	//iResult = connect(ConnectSocket, (sockaddr*)&addrServer, sizeof(addrServer));
	//if (iResult < 0) {
	//	printf("Connect failure: %d\n", WSAGetLastError());
	//	return;
	//}
}

int CSocket::Send(const char *buf)
{
	iResult = send(ConnectSocket, buf, 5, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);
	return 0;
}

void CSocket::Close()
{
	closesocket(ConnectSocket);
}

int CSocket::char2int(char input)
{
	if (input >= '0' && input <= '9') return input - '0';
	else if (input >= 'A' && input <= 'F') return input - 'A' + 10;
}

void CSocket::Hexstring(const char *src, char* target)
{
	while (*src && src[1])
	{
		*(target++) = char2int(*src) * 16 + char2int(src[1]);
		src += 2;
	}
}

void CSocket::SendByte(const char *src)
{
	char *t = tmp;
	Hexstring(src, t);
	Send(tmp);
}


class mypicture{
private:
	WSADATA wsaData;
	SOCKET hsocket;
	struct sockaddr_in saServer;
public:
	mypicture()
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsaData))
			cout << "初始化SOCKET出错1" << endl;

		hsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);        
		saServer.sin_family = AF_INET;
		saServer.sin_port = htons(8080);  //端口      
		saServer.sin_addr.s_addr = inet_addr("192.168.1.1");  
	}

	bool initial()
	{
		int nRet;
		nRet = connect(hsocket, (LPSOCKADDR)&saServer, sizeof(SOCKADDR_IN));
		if (nRet == SOCKET_ERROR)
		{
			cout << "初始化SOCKET出错2" << endl;
			closesocket(hsocket);
			return FALSE;
		}
		char*   req21 =
			"GET /?action=stream\r\n\r\n"
			"Host: 192.168.1.1:8080\r\n";

		char*   req22 =
			"User-Agent: vlc/1.1.5\r\n"
			"Range: bytes=0-\r\n"
			"Authorization: Basic YWRtaW46\r\n" //认证信息
			"Icy-MetaData: 1\r\n\r\n";
		nRet = send(hsocket, req21, strlen(req21), 0);
		Sleep(500);

		nRet = send(hsocket, req22, strlen(req22), 0);
		Sleep(500);

		if (nRet == SOCKET_ERROR)
		{
			cout << "初始化SOCKET出错3" << endl;
			closesocket(hsocket);
			return FALSE;
		}
		return TRUE;
	}

	

	bool receiveImageOneTime(const char *filename){
		int nRet;
		char Dest[SOCKSIZE] = { 0 }; //存储接收到的数据包      
		nRet = 0; //接收到的数据包长度

		//FFD8和FFD9之间的数据包写入dest.jpg文件
		FILE* fpWRJPEGFILE = NULL;
		fopen_s(&fpWRJPEGFILE, filename, "wb+");
		if (fpWRJPEGFILE == NULL)
		{
			cout << "Open file Wrong! \n";
			return false;
		}

		unsigned char chBuf[MAX_SIZE] = { 0 };    //存储接收到的Socket数据包
		unsigned char tmpBuf[MAX_SIZE] = { 0 };   //临时变量
		unsigned char chBegin[2] = { 0xFF, 0xD8 }; //JPEG开始标记
		unsigned char chEnd[2] = { 0xFF, 0xD9 }; //JPEG结束标记 

		int BufLen = 0; //缓冲区内数据包长度
		int iBegin = 0; //FFD8索引
		int iEnd = 0;    //FFD9索引 
		
	/***********************取一次图************************/
		bool flag = 1;
		while (flag)
		{
			memset(Dest, 0, SOCKSIZE); //char数组初始化
			nRet = recv(hsocket, (LPSTR)Dest, SOCKSIZE, 0);
			//收到Socket数据包,存储到chBuf  
			if (BufLen + nRet > MAX_SIZE)
			{
				memcpy(chBuf, chBuf + BufLen - SOCKSIZE / 4, SOCKSIZE / 4);
				memcpy(chBuf + SOCKSIZE / 4, Dest, nRet);
				BufLen = nRet + SOCKSIZE / 4;
				memset(chBuf + BufLen, 0, MAX_SIZE - BufLen);  //初始化

			}
			else{
				memcpy(chBuf + BufLen, Dest, nRet);
				BufLen = BufLen + nRet; //缓冲区内数据包长度
			}
			//在缓冲区内 查找FFD8和FFD9
			bool ff = 0;
			for (int i = 0; i < BufLen - 8; i++)
			{
				
				if (!memcmp(chBuf + i, chBegin, 2) && ! ff)
				{   //找到FFD8
					ff = 1;
					iBegin = i;
				}
				if (!memcmp(chBuf + i, chEnd, 2) && ff)
				{   //找到FFD9
					iEnd = i;
					fwrite(chBuf + iBegin, 1, iEnd - iBegin + 2, fpWRJPEGFILE);
					memset(chBuf, '\0', MAX_SIZE);  //初始化
					flag = 0;   //找到，退出循环；
					fclose(fpWRJPEGFILE); //关闭JPEG文件
					break; //找到一张图片就退出For循环
				}
			}
		}
		return TRUE;
	}

	/***************************连续取图******************************/
	bool receiveImageContinuous(const char *filename){
		//SOCKET+TCP连接网络摄相机
		int nRet;

		char Dest[SOCKSIZE] = { 0 }; //存储接收到的数据包      
		nRet = 0; //接收到的数据包长度

		//FFD8和FFD9之间的数据包写入dest.jpg文件
		FILE* fpWRJPEGFILE = NULL;
		fopen_s(&fpWRJPEGFILE, filename, "wb+");
		if (fpWRJPEGFILE == NULL)
		{
			cout << "Open file Wrong! \n";
			return false;
		}

		unsigned char chBuf[MAX_SIZE] = { 0 };    //存储接收到的Socket数据包
		unsigned char tmpBuf[MAX_SIZE] = { 0 };   //临时变量
		unsigned char chBegin[2] = { 0xFF, 0xD8 }; //JPEG开始标记
		unsigned char chEnd[2] = { 0xFF, 0xD9 }; //JPEG结束标记 

		int BufLen = 0; //缓冲区内数据包长度
		int iBegin = 0; //FFD8索引
		int iEnd = 0;    //FFD9索引
		while (1)
		{
			memset(Dest, 0, SOCKSIZE); //char数组初始化
			nRet = recv(hsocket, (LPSTR)Dest, SOCKSIZE, 0);
			//收到Socket数据包,存储到chBuf  
			if (BufLen + nRet > MAX_SIZE)
			{
				memcpy(chBuf, chBuf + BufLen - SOCKSIZE / 4, SOCKSIZE / 4);
				memcpy(chBuf + SOCKSIZE / 4, Dest, nRet);
				BufLen = nRet + SOCKSIZE / 4;
				memset(chBuf + BufLen, 0, MAX_SIZE - BufLen);  //初始化
				
			} 
			else{
				memcpy(chBuf + BufLen, Dest, nRet);
				BufLen = BufLen + nRet; //缓冲区内数据包长度
			}

			//在缓冲区内 查找FFD8和FFD9
			bool ff = 0;
			for (int i = 0; i < BufLen - 8; i++)
			{
				if (!memcmp(chBuf + i, chBegin, 2) && ! ff)
				{   //找到FFD8
					iBegin = i;
					ff = 1;
				}
				if (!memcmp(chBuf + i, chEnd, 2) && ff)
				{   //找到FFD9
					iEnd = i;
					if (fpWRJPEGFILE == NULL) fopen_s(&fpWRJPEGFILE, filename, "wb");
					if (NULL != fpWRJPEGFILE)
					{
						//找到FFD8和FFD9
						fwrite(chBuf + iBegin, 1, iEnd - iBegin + 2, fpWRJPEGFILE);
						//FF09之后的数据包长度
						int leaveMsgLen = BufLen - iEnd - 2;
						//缓冲区FF09之前的内容清空
						memset(tmpBuf, '\0', MAX_SIZE); //初始化
						memcpy(tmpBuf, chBuf + iEnd + 2, leaveMsgLen);
						memset(chBuf, '\0', MAX_SIZE);  //初始化
						memcpy(chBuf, tmpBuf, leaveMsgLen);

						//修改缓冲区数据包长度,方便向缓冲区追加数据
						BufLen = leaveMsgLen;
						fclose(fpWRJPEGFILE); //关闭JPEG文件
					} 
					fpWRJPEGFILE = NULL;
					Sleep(50);
					break; //找到一张图片就退出For循环
				}
			} 	
		} 
		return TRUE;
	}
};

