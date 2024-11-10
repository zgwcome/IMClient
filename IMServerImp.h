#pragma once
#include <vector>
#include <string>
#include <cstdarg>
#include <iostream>
#include <cstring>

#include "jsoncpp/json.h"
#include "include/TIMCloud.h"
#include "include/TIMCloudCallback.h"
#include "include/TIMCloudComm.h"
#include "include/TIMCloudDef.h"

#include "GenerateTestUserSig.h"
using namespace std;
typedef void* HWND;
typedef char TCHAR;
const int MAX_PATH = 255;

typedef bool (*NewMsgCallBack)(string user, string msg);

using namespace std;
class IMServerImp
{

public:
	static IMServerImp& GetInst() {
		static IMServerImp inst;
		return inst;
	}
	void Init(NewMsgCallBack newMsgCallBack);
	void InitImSDK();
	void Login(string userid);
	void InitConvList();
	void AddConv(std::string id, uint32_t type);
	void GetConvMsgs(std::string userid, TIMConvType type);
	void GetGroupJoinedList();
	void GetGroupInfoList();
	void GetGroupMemberInfoList(std::string groupid);
	void ParseMsg(const char* json_msg_array);
	void CreateConv(string userid);
	bool SendMsg(string user_id, string send_text);
	void Logf(const char* module, TIMLogLevel level, const char* fmt, ...);
	void Log(const char* module, TIMLogLevel level, const char* log, bool msgbox = true);

private:
	typedef struct {
		std::string id;
	}ConvInfo;
	vector<ConvInfo> convs;

	typedef struct {
		std::string id;
	}GroupMemberInfo;
	typedef struct {
		std::string id;
		std::string name;
		std::vector<GroupMemberInfo> mems;
	}GroupInfo;
	std::vector<GroupInfo> groups;

	vector<string> msgs;
	std::string login_id;
	//HWND hwnd;
	NewMsgCallBack newMsgCallBack;
};