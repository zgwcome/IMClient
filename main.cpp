#include <iostream>
#include "include/TIMCloud.h"
#include "include/TIMCloudCallback.h"
#include "include/TIMCloudComm.h"
#include "include/TIMCloudDef.h"
#include "IMServerImp.h"
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "HttpServer.h"

std::queue<map<string, string>> msgQueue;
std::mutex mtx;
std::condition_variable cv;

bool OnNewMessgae(string user, string msg)
{
    printf("New message:%s -> %s\r\n", user.c_str(), msg.c_str());
    return true;
}

unsigned char FromHex(unsigned char x)
{
    unsigned char y;
    if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
    else if (x >= '0' && x <= '9') y = x - '0';
    return y;
}

std::string UrlDecode(const std::string& str)
{
    std::string strTemp;
    size_t length = str.length();
    for (size_t i = 0; i < length; i++) {
        if (str[i] == '+') {
            strTemp += ' ';
        }
        else if (str[i] == '%') {
            unsigned char high = FromHex((unsigned char)str[++i]);
            unsigned char low = FromHex((unsigned char)str[++i]);
            strTemp += high * 16 + low;
        }
        else strTemp += str[i];
    }
    return strTemp;
}

void onGetRequest(const string& absPath)
{
    map<string, string> msg;
    if (absPath.find(("/rpc?")) == 0)
    {
        string absPathTemp = absPath.substr(5);
        //& =
        while (!absPathTemp.empty())
        {
            int keyEndPos = absPathTemp.find(("="));
            int valueEndPos = absPathTemp.find(("&"));

            string key = absPathTemp.substr(0, keyEndPos);
            string value = absPathTemp.substr(keyEndPos+1, valueEndPos- keyEndPos - 1);
            msg[key] = UrlDecode(value);
            if (keyEndPos == wstring::npos || valueEndPos == wstring::npos)
            {
                break;
            }
            absPathTemp = absPathTemp.substr(valueEndPos + 1);
        } 
    }

    if (msg.find(("user")) != msg.end() && msg.find(("msg")) != msg.end())
    {
        std::unique_lock<std::mutex> lock(mtx);
        msgQueue.push(msg);
        cv.notify_all();
    }
}

int main()
{
    thread http(HttpServer, 8800, onGetRequest);
    http.detach();

    IMServerImp& imServer = IMServerImp::GetInst();
    imServer.Init(OnNewMessgae);
    imServer.InitImSDK();
    imServer.Login("控制中心");
    while(true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        if(msgQueue.empty()){
            cv.wait(lock);
        }else{
            map<string, string> httpMsg = ::move(msgQueue.front());
            msgQueue.pop();
            lock.unlock();
            std::cout << httpMsg[("user")] << "<-" << httpMsg[("msg")] <<std::endl;
            imServer.SendMsg(httpMsg[("user")], httpMsg[("msg")]);
        }
    }
    return 0;
}