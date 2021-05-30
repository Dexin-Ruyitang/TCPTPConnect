#include"stdafx.h"

using namespace std;

#define PORT 65432
#define LOCALHOST "127.0.0.1"

volatile bool iThreadReceive = false;
volatile bool iThreadSend = false;

DWORD WINAPI ThreadReceive(LPVOID lpThreadParameter);
DWORD WINAPI ThreadSend(LPVOID lpThreadParameter);

int main()
{
    //��ʼ��winsock2.dll
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsaData) != 0)
    {
        cout << "����winsock.dllʧ�ܣ�" << endl;
        cout << "�����������..." << endl;
        cin.get();
        return 0;
    }
    //�����׽���
    SOCKET socketServer;
    if ((socketServer = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
    {
        cout << "�����׽���ʧ�ܣ�������룺" << WSAGetLastError() << endl;
        WSACleanup();
        cout << "�����������..." << endl;
        cin.get();
        return 0;
    }
    //�󶨶˿ں�IP
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    //�󶨱����Ļ��ص�ַ
    inet_pton(AF_INET, LOCALHOST, &addr.sin_addr);
    if (SOCKET_ERROR == bind(socketServer, (SOCKADDR*)&addr, sizeof(sockaddr_in)))
    {
        cout << "��ַ��ʧ�ܣ�������룺" << WSAGetLastError() << endl;
        closesocket(socketServer);
        WSACleanup();
        cout << "�����������..." << endl;
        cin.get();
        return 0;
    }
    //���׽�����Ϊ����״̬
    listen(socketServer, 0);
    //���߳�ѭ�����տͻ��˵�����
    while (1)
    {
        sockaddr_in addrClient;
        int len = sizeof(sockaddr_in);
        //���ճɹ�������clientͨѶ��socket
        SOCKET con = accept(socketServer, (SOCKADDR*)&addrClient, &len);
        if (con != INVALID_SOCKET)
        {
            cout << "�ɹ���" << con << "�������ӣ�" << endl;
            //�����̣߳���������clientͨѶ���׽���
            HANDLE hThreadSend = CreateThread(NULL, 0, ThreadSend, (LPVOID)con, 0, NULL);
            iThreadSend = true;
            HANDLE hThreadReceive = CreateThread(NULL, 0, ThreadReceive, (LPVOID)con, 0, NULL);
            iThreadReceive = true;
            //�رն��̵߳�����
            CloseHandle(hThreadSend);
            CloseHandle(hThreadReceive);
        }
    }
    closesocket(socketServer);
    WSACleanup();
    cout << "�����������..." << endl;
    cin.get();
    return 0;
}

//�߳�ͨѶ����

//��������
DWORD WINAPI ThreadSend(LPVOID lpThreadParameter)
{
    //��ͻ���ͨѶ
    SOCKET sock = (SOCKET)lpThreadParameter;
    while (1)
    {
        if (!iThreadReceive)
        {
            return 0;
        }
        //�ַ�������
        char msgBuffer[1024];
        cin.getline(msgBuffer, sizeof(msgBuffer));
        //���ͻ��˷���һ����Ϣ
        int size = send(sock, msgBuffer, sizeof(msgBuffer), 0);
        if (strcmp(msgBuffer, "end\0") == 0)
        {
            cout << "�رպ�" << sock << "�����ӣ�" << endl;
            return 0;
        }
        if (size == SOCKET_ERROR || size == 0)
        {
            cout << "������Ϣʧ�ܣ�������룺" << WSAGetLastError() << endl;
            return 0;
        }
        /*else
            cout << "��Ϣ���ͳɹ���" << endl;*/
    }
}

//��������
DWORD WINAPI ThreadReceive(LPVOID lpThreadParameter)
{
    //��ͻ���ͨѶ
    SOCKET sock = (SOCKET)lpThreadParameter;
    while (1)
    {
        if (!iThreadSend)
        {
            return 0;
        }
        //�ַ�������
        char msgBuffer[1024];
        //���տͻ�������
        msgBuffer[1023] = { 0 };
        int ret = recv(sock, msgBuffer, sizeof(msgBuffer), 0);
        if (ret == SOCKET_ERROR || ret == 0)
        {
            cout << sock << "�Ͽ������ӣ�" << endl;
            break;
        }
        else
            cout << sock << " ˵��" << msgBuffer << endl;
    }
    return 0;
}
