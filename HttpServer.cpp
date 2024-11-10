#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include "HttpServer.h"

using namespace std;
OnGetRequest onNewRequestCallBack = nullptr;

const char* GetRespone(char* request) {
    string req = request;

    if (req.substr(0, 9).compare("GET /rpc?") == 0) {
        int pos1 = req.find(" ") + 1;
        int pos2 = req.find(" ", pos1);
        string url = req.substr(pos1, pos2 - pos1);

        onNewRequestCallBack(url);
        return  "HTTP/1.0 200 OK\r\nServer:Linux Web Server \r\nContent-length:2048 \r\nContent-type:text/html;charset=utf-8\r\n\r\n<html>ok</html>";
    }
    return  "HTTP/1.0 200 OK\r\nServer:Linux Web Server \r\nContent-length:2048 \r\nContent-type:text/html;charset=utf-8\r\n\r\n<html>error</html>";
}


void request_handler(int clnt_sock) {
    char reqBuffer[4096];

    //接收数据
    int ret = recv(clnt_sock, reqBuffer, 4096, 0);
    if (ret > 0) {
        reqBuffer[ret] = 0x0;
    }
    //发送数据
    string sendBuffer = GetRespone(reqBuffer);
    send(clnt_sock, sendBuffer.c_str(), sendBuffer.length(), 0);
    close(clnt_sock);
}


int HttpServer(int port, OnGetRequest requestCallBack)
{
    onNewRequestCallBack = requestCallBack;

    int serv_sock, client_sock;
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_adr, clnt_adr;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_port = htons(port);
    if (bind(serv_sock, (sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        return 0;
    }

    if (listen(serv_sock, 10) == -1) {
        return 0;
    }

    while (true) {
        socklen_t client_len = sizeof(clnt_adr);
        client_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &client_len);
        thread thread(request_handler, client_sock);
        thread.detach();
    }
    close(serv_sock);
    return 0;
}

