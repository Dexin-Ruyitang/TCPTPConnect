#include"stdafx.h"

#define PORT 65432
#define LOCALHOST "127.0.0.1"

using namespace std;

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
            //�����̣߳���������clientͨѶ���׽���
            HANDLE hThread = CreateThread(NULL, 0, ThreadFun, (LPVOID)con, 0, NULL);
            //�رն��̵߳�����
            CloseHandle(hThread);
        }
    }
    closesocket(socketServer);
    WSACleanup();
    cout << "�����������..." << endl;
    cin.get();
    return 0;
}

//�߳�ͨѶ����
DWORD WINAPI ThreadFun(LPVOID lpThreadParameter)
{
    //��ͻ���ͨѶ���ȷ����ٽ�������
    SOCKET sock = (SOCKET)lpThreadParameter;
    cout << "�ɹ���" << sock << "�������ӣ�" << endl;
    while (1)
    {
        //�ַ�������
        char msgBuffer[1024];
        cout << "��������" << sock << "�������ݣ�";
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
