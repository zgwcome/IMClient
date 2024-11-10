#pragma once
#include <string>
typedef void (*OnGetRequest)(const std::string& absPath);
int HttpServer(int port, OnGetRequest onNewRequestCallBack);
