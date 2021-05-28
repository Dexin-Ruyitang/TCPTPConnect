#include"stdafx.h"

using namespace std;

#define PORT 65432
#define LOCALHOST "127.0.0.1"

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
    while (1)
    {
        //���շ������Ϣ
        char msgBuffer[1024] = { 0 };
        int size = recv(socketClient, msgBuffer, sizeof(msgBuffer), 0);
        if (strcmp(msgBuffer, "end\0") == 0)
        {
            cout << "���������ѹر����ӣ�" << endl;
            break;
        }
        if (size < 0)
        {
            cout << "������Ϣʧ�ܣ�������룺" << WSAGetLastError() << endl;
            break;
        }
        else if (size == 0)
        {
            cout << "�Է��ѹر�����" << endl;
            break;
        }
        else
            cout << "Server ˵��" << msgBuffer << endl;
        //�Ӽ�������һ�����ַ��͸�������
        msgBuffer[1023] = 0;
        cout << "�Ӽ������뷢������������Ϣ��";
        cin.getline(msgBuffer, sizeof(msgBuffer));
        if (strcmp(msgBuffer, "end\0") == 0)
        {
            cout << "�ر����ӣ�" << endl;
            break;
        }
        int ret = send(socketClient, msgBuffer, sizeof(msgBuffer), 0);
        if (ret == SOCKET_ERROR || ret == 0)
        {
            cout << "������Ϣʧ�ܣ�������룺" << WSAGetLastError() << endl;
            break;
        }
        /*else
            cout << "��Ϣ���ͳɹ���" << endl;*/
    }
    closesocket(socketClient);
    WSACleanup();
    cout << "�����������..." << endl;
    cin.get();
    return 0;
}
