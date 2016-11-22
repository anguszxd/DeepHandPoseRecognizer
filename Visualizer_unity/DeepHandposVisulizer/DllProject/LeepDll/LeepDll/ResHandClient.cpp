#include "ResHandClient.h"


ResHandClient::ResHandClient()
{
}


ResHandClient::~ResHandClient()
{
}

void ResHandClient::GetIPAddress(char *ip){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	PHOSTENT hostinfo;
	char hostname[50];
	char ipaddr[50];
	memset(hostname, 0, sizeof(hostname));
	memset(ipaddr, 0, sizeof(ipaddr));

	int nError = gethostname(hostname, sizeof(hostname));
	if (nError == 0)
	{
		hostinfo = gethostbyname(hostname);
		// ip address �ľ�
		strcpy(ipaddr, inet_ntoa(*(struct in_addr*)hostinfo->h_addr_list[0]));
	}

	WSACleanup();

	strcpy(ip, ipaddr);
}

void ResHandClient::Init(char *ip, int portNum){
	_portNum = portNum;

	if (ip == NULL){
		char temp[256];
		GetIPAddress(temp);
		strcpy(_IP, temp);
	}
	else{
		strcpy(_IP, ip);
	}

	//���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup(), error");
	}

	// ���� ������ ���� ���� ����
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("hSocketet(), error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(_IP);
	servAddr.sin_port = htons(_portNum);

	// ������ ���� ��û
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("Connect() error");
		//���� ����
		DeInit();
	}


}

void ResHandClient::ErrorHandling(char *message)
{
	wchar_t text1[100];
	mbstowcs(text1, message, strlen(message) + 1);
	LPCWSTR test = text1;

	MessageBox(NULL, test, L"DLL Error", MB_OK);
}

void ResHandClient::DeInit(){
	// ���� ����
	closesocket(hSocket);
	WSACleanup();
}

void ResHandClient::Recv(HandClass *dst){								//���� ����ü
	char message[1024];

	// ������ ���� 
	int strLen = recv(hSocket, message, sizeof(message) - 1, 0);
	if (strLen == -1)
	{
		ErrorHandling("read() error");
	}
	message[strLen] = 0;
	memcpy(dst, message, sizeof(HandClass));
}