#include"stdafx.h"

using namespace std;

#define PORT 65432
#define LOCALHOST "127.0.0.1"

volatile bool iThreadReceive = false;
volatile bool iThreadSend = false;

HANDLE hMainThread;

DWORD WINAPI ThreadReceive(LPVOID lpThreadParameter);
DWORD WINAPI ThreadSend(LPVOID lpThreadParameter);

int main()
{
    hMainThread = GetCurrentThread();
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
    SOCKET socketClient;
    if ((socketClient = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
    {
        cout << "�����׽���ʧ�ܣ�������룺" << WSAGetLastError() << endl;
        WSACleanup();
        cout << "�����������..." << endl;
        cin.get();
        return 0;
    }
    //���ӷ�����
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    //�󶨱����Ļ��ص�ַ
    inet_pton(AF_INET, LOCALHOST, &addr.sin_addr);
    int len = sizeof(sockaddr_in);
    if (connect(socketClient, (SOCKADDR*)&addr, len) == SOCKET_ERROR)
    {
        cout << "���Ӵ��󣡴�����룺" << WSAGetLastError() << endl;
        cout << "�����������..." << endl;
        cin.get();
        return 0;
    }
    //ʵ�ֽ������֣��ͻ����Ƚ��պ���
    //�����߳�
    HANDLE hThreadSend = CreateThread(NULL, 0, ThreadSend, (LPVOID)socketClient, 0, NULL);
    iThreadSend = true;
    HANDLE hThreadReceive = CreateThread(NULL, 0, ThreadReceive, (LPVOID)socketClient, 0, NULL);
    iThreadReceive = true;
    //�رն��̵߳�����
    CloseHandle(hThreadSend);
    CloseHandle(hThreadReceive);
    SuspendThread(hMainThread);
    closesocket(socketClient);
    WSACleanup();
    cout << "�����������..." << endl;
    cin.get();
    return 0;
}

DWORD WINAPI ThreadReceive(LPVOID lpThreadParameter)
{
    SOCKET socketClient = (SOCKET)lpThreadParameter;
    while (1)
    {
        if (!iThreadSend)
        {
            ResumeThread(hMainThread);
            return 0;
        }
        char msgBuffer[1024];
        //���շ������Ϣ
        msgBuffer[1023] = { 0 };
        int size = recv(socketClient, msgBuffer, sizeof(msgBuffer), 0);
        if (strcmp(msgBuffer, "end\0") == 0)
        {
            cout << "���������ѹر����ӣ�" << endl;
            return 0;
        }
        if (size < 0)
        {
            cout << "������Ϣʧ�ܣ�������룺" << WSAGetLastError() << endl;
            return 0;
        }
        else if (size == 0)
        {
            cout << "�Է��ѹر�����" << endl;
            return 0;
        }
        else
            cout << "Server ˵��" << msgBuffer << endl;
    }
}

DWORD WINAPI ThreadSend(LPVOID lpThreadParameter)
{
    SOCKET socketClient = (SOCKET)lpThreadParameter;
    while (1)
    {
        if (!iThreadReceive)
        {
            ResumeThread(hMainThread);
            return 0;
        }
        char msgBuffer[1024];
        //�Ӽ�������һ�����ַ��͸�������
        msgBuffer[1023] = 0;
        cin.getline(msgBuffer, sizeof(msgBuffer));
        if (strcmp(msgBuffer, "end\0") == 0)
        {
            cout << "�ر����ӣ�" << endl;
            return 0;
        }
        int ret = send(socketClient, msgBuffer, sizeof(msgBuffer), 0);
        if (ret == SOCKET_ERROR || ret == 0)
        {
            cout << "������Ϣʧ�ܣ�������룺" << WSAGetLastError() << endl;
            return 0;
        }
        /*else
            cout << "��Ϣ���ͳɹ���" << endl;*/
    }
}
