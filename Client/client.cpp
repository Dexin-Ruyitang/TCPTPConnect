#include"stdafx.h"

using namespace std;

#define PORT 65432
#define LOCALHOST "127.0.0.1"

int main()
{
    //初始化winsock2.dll
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsaData) != 0)
    {
        cout << "加载winsock.dll失败！" << endl;
        cout << "按任意键继续..." << endl;
        cin.get();
        return 0;
    }
    //创建套接字
    SOCKET socketClient;
    if ((socketClient = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
    {
        cout << "创建套接字失败！错误代码：" << WSAGetLastError() << endl;
        WSACleanup();
        cout << "按任意键继续..." << endl;
        cin.get();
        return 0;
    }
    //连接服务器
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    //绑定本机的环回地址
    inet_pton(AF_INET, LOCALHOST, &addr.sin_addr);
    int len = sizeof(sockaddr_in);
    if (connect(socketClient, (SOCKADDR*)&addr, len) == SOCKET_ERROR)
    {
        cout << "连接错误！错误代码：" << WSAGetLastError() << endl;
        cout << "按任意键继续..." << endl;
        cin.get();
        return 0;
    }
    //实现交互部分，客户端先接收后发送
    while (1)
    {
        //接收服务端消息
        char msgBuffer[1024] = { 0 };
        int size = recv(socketClient, msgBuffer, sizeof(msgBuffer), 0);
        if (strcmp(msgBuffer, "end\0") == 0)
        {
            cout << "服务器端已关闭连接！" << endl;
            break;
        }
        if (size < 0)
        {
            cout << "接收信息失败！错误代码：" << WSAGetLastError() << endl;
            break;
        }
        else if (size == 0)
        {
            cout << "对方已关闭连接" << endl;
            break;
        }
        else
            cout << "Server 说：" << msgBuffer << endl;
        //从键盘输入一行文字发送给服务器
        msgBuffer[1023] = 0;
        cout << "从键盘输入发给服务器的信息：";
        cin.getline(msgBuffer, sizeof(msgBuffer));
        if (strcmp(msgBuffer, "end\0") == 0)
        {
            cout << "关闭连接！" << endl;
            break;
        }
        int ret = send(socketClient, msgBuffer, sizeof(msgBuffer), 0);
        if (ret == SOCKET_ERROR || ret == 0)
        {
            cout << "发送信息失败！错误代码：" << WSAGetLastError() << endl;
            break;
        }
        /*else
            cout << "信息发送成功！" << endl;*/
    }
    closesocket(socketClient);
    WSACleanup();
    cout << "按任意键继续..." << endl;
    cin.get();
    return 0;
}
