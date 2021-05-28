#include"stdafx.h"

#define PORT 65432
#define LOCALHOST "127.0.0.1"

using namespace std;

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
    SOCKET socketServer;
    if ((socketServer = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
    {
        cout << "创建套接字失败！错误代码：" << WSAGetLastError() << endl;
        WSACleanup();
        cout << "按任意键继续..." << endl;
        cin.get();
        return 0;
    }
    //绑定端口和IP
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    //绑定本机的环回地址
    inet_pton(AF_INET, LOCALHOST, &addr.sin_addr);
    if (SOCKET_ERROR == bind(socketServer, (SOCKADDR*)&addr, sizeof(sockaddr_in)))
    {
        cout << "地址绑定失败！错误代码：" << WSAGetLastError() << endl;
        closesocket(socketServer);
        WSACleanup();
        cout << "按任意键继续..." << endl;
        cin.get();
        return 0;
    }
    //将套接字设为监听状态
    listen(socketServer, 0);
    //主线程循环接收客户端的链接
    while (1)
    {
        sockaddr_in addrClient;
        int len = sizeof(sockaddr_in);
        //接收成功返回与client通讯的socket
        SOCKET con = accept(socketServer, (SOCKADDR*)&addrClient, &len);
        if (con != INVALID_SOCKET)
        {
            //创建线程，并传入与client通讯的套接字
            HANDLE hThread = CreateThread(NULL, 0, ThreadFun, (LPVOID)con, 0, NULL);
            //关闭对线程的引用
            CloseHandle(hThread);
        }
    }
    closesocket(socketServer);
    WSACleanup();
    cout << "按任意键继续..." << endl;
    cin.get();
    return 0;
}

//线程通讯部分
DWORD WINAPI ThreadFun(LPVOID lpThreadParameter)
{
    //与客户端通讯，先发送再接收数据
    SOCKET sock = (SOCKET)lpThreadParameter;
    cout << "成功和" << sock << "建立连接！" << endl;
    while (1)
    {
        //字符缓冲区
        char msgBuffer[1024];
        cout << "服务器向" << sock << "发送数据：";
        cin.getline(msgBuffer, sizeof(msgBuffer));
        //给客户端发送一条消息
        int size = send(sock, msgBuffer, sizeof(msgBuffer), 0);
        if (strcmp(msgBuffer, "end\0") == 0)
        {
            cout << "关闭和" << sock << "的连接！" << endl;
            return 0;
        }
        if (size == SOCKET_ERROR || size == 0)
        {
            cout << "发送信息失败！错误代码：" << WSAGetLastError() << endl;
            return 0;
        }
        /*else
            cout << "信息发送成功！" << endl;*/

        //接收客户端数据
        msgBuffer[1023] = { 0 };
        int ret = recv(sock, msgBuffer, sizeof(msgBuffer), 0);
        if (ret == SOCKET_ERROR || ret == 0)
        {
            cout << sock << "断开了连接！" << endl;
            break;
        }
        else
            cout << sock << " 说：" << msgBuffer << endl;
    }
    return 0;
}
