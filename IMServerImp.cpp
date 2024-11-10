#include "IMServerImp.h"
#include <unistd.h>

string GetPath()
{
    char *path_temp = getcwd(nullptr, 0);
    if (path_temp == nullptr)
    {
        return "";
    }
    std::string path = path_temp;
    free(path_temp);
    return path;
}

void IMServerImp::Init(NewMsgCallBack newMsgCallBack) {
    //this->hwnd = hwnd;
    this->newMsgCallBack = newMsgCallBack;
    // ����Ϣ�ص�ʱ��
    TIMAddRecvNewMsgCallback([](const char* json_msg_array, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        ths->Logf("Message", kTIMLog_Info, "New Message:\n%s", json_msg_array);

        ths->ParseMsg(json_msg_array);
        }, this);

    //��Ϣ���»ص��¼�
    TIMSetMsgUpdateCallback([](const char* json_msg_array, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        ths->Logf("Message", kTIMLog_Info, "Update Message:\n%s", json_msg_array);
        }, this);

    //��Ϣ�Ѷ���ִ�¼�
    TIMSetMsgReadedReceiptCallback([](const char* json_msg_readed_receipt_array, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;

        ths->Logf("Message", kTIMLog_Info, "Message ReadedReceipt:\n%s", json_msg_readed_receipt_array);

        Json::Value json_value_receipts;
        Json::Reader reader;
        if (!reader.parse(json_msg_readed_receipt_array, json_value_receipts)) {
            // Json ����ʧ��
            return;
        }

        for (Json::ArrayIndex i = 0; i < json_value_receipts.size(); i++) {
            Json::Value& json_value_receipt = json_value_receipts[i];

            std::string convid = json_value_receipt[kTIMMsgReceiptConvId].asString();
            uint32_t conv_type = json_value_receipt[kTIMMsgReceiptConvType].asUInt();
            uint64_t timestamp = json_value_receipt[kTIMMsgReceiptTimeStamp].asUInt64();

            // ��Ϣ�Ѷ��߼�
        }

        }, this);

    TIMSetMsgRevokeCallback([](const char* json_msg_locator_array, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        ths->Logf("Message", kTIMLog_Info, "Message Revoke:\n%s", json_msg_locator_array);
        Json::Value json_value_locators;
        Json::Reader reader;
        if (!reader.parse(json_msg_locator_array, json_value_locators)) {
            // Json ����ʧ��
            return;
        }
        for (Json::ArrayIndex i = 0; i < json_value_locators.size(); i++) {
            Json::Value& json_value_locator = json_value_locators[i];

            std::string convid = json_value_locator[kTIMMsgLocatorConvId].asString();
            uint32_t conv_type = json_value_locator[kTIMMsgLocatorConvType].asUInt();
            bool isrevoke = json_value_locator[kTIMMsgLocatorIsRevoked].asBool();
            uint64_t time = json_value_locator[kTIMMsgLocatorTime].asUInt64();
            uint64_t seq_ = json_value_locator[kTIMMsgLocatorSeq].asUInt64();
            uint64_t rand_ = json_value_locator[kTIMMsgLocatorRand].asUInt64();
            bool isself = json_value_locator[kTIMMsgLocatorIsSelf].asBool();

            // ��д ��Ϣ�����߼�
        }
        }, this);

    TIMSetMsgElemUploadProgressCallback([](const char* json_msg, uint32_t index, uint32_t cur_size, uint32_t total_size, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;

        Json::Value json_value_msg;
        Json::Reader reader;
        if (!reader.parse(json_msg, json_value_msg)) {
            // Json ����ʧ��
            return;
        }
        Json::Value& elems = json_value_msg[kTIMMsgElemArray];
        if (index >= elems.size()) {
            // index ������ϢԪ�ظ�����Χ
            return;
        }
        uint32_t elem_type = elems[index][kTIMElemType].asUInt();
        if (kTIMElem_File == elem_type) {

        }
        else if (kTIMElem_Sound == elem_type) {

        }
        else if (kTIMElem_Video == elem_type) {

        }
        else if (kTIMElem_Image == elem_type) {

        }
        else {
            // ��������Ԫ�ز������ϴ�Ҫ��
        }



        ths->Logf("UploadProgress", kTIMLog_Info, "index:%u cur_size:%u total_size:%u", index, cur_size, total_size);
        }, this);

    TIMSetGroupTipsEventCallback([](const char* json_group_tips, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        ths->Logf("GroupTips", kTIMLog_Info, "GroupTips Event:%s", json_group_tips);
        }, this);

    TIMSetConvEventCallback([](TIMConvEvent conv_event, const char* json_conv_array, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        Json::Reader reader;
        Json::Value json_value;
        if (strlen(json_conv_array) == 0) {
            return;
        }
        if (!reader.parse(json_conv_array, json_value)) {
            ths->Logf("ConvEvent", kTIMLog_Error, "Parse Json json_conv_array Failure!error:%s", reader.getFormattedErrorMessages().c_str());
            return;
        }
        for (Json::ArrayIndex i = 0; i < json_value.size(); i++) {
            Json::Value& convinfo = json_value[i];
            std::string conv_id = convinfo[kTIMConvId].asCString();
            uint32_t conv_type = convinfo[kTIMConvType].asUInt();
            ths->Logf("ConvEvent", kTIMLog_Info, "%02u event:%u id:%s type:%u", i, conv_event, conv_id.c_str(), conv_type);
            if (conv_event == kTIMConvEvent_Add) {
                //IMServerImp::GetInst().AddConv(conv_id, conv_type);
            }
            else if (conv_event == kTIMConvEvent_Del) {
                //IMServerImp::GetInst().DelConv(conv_id, conv_type);
            }
            else if (conv_event == kTIMConvEvent_Update) {
                //IMServerImp::GetInst().UpdateConv(conv_id, conv_type);
            }
        }
        }, this);

    TIMSetNetworkStatusListenerCallback([](TIMNetworkStatus status, int32_t code, const char* desc, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        ths->Logf("Network", kTIMLog_Info, "status %u code:%u desc:%s", status, code, desc);
        }, this);

    TIMSetKickedOfflineCallback([](const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        ths->Logf("KickedOff", kTIMLog_Info, "Kicked Offline");
        }, this);

    TIMSetUserSigExpiredCallback([](const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        ths->Logf("UserSig", kTIMLog_Info, "Expired");
        }, this);

    TIMSetOnAddFriendCallback([](const char* json_identifier_array, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        ths->Logf("OnAddFriend", kTIMLog_Info, json_identifier_array);
        }, this);

    TIMSetOnDeleteFriendCallback([](const char* json_identifier_array, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        ths->Logf("OnDeleteFriend", kTIMLog_Info, json_identifier_array);
        }, this);

    TIMSetUpdateFriendProfileCallback([](const char* json_friend_profile_update_array, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        ths->Logf("UpdateFriend", kTIMLog_Info, json_friend_profile_update_array);
        }, this);

    TIMSetFriendAddRequestCallback([](const char* json_friend_add_request_pendency_array, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        ths->Logf("FriendAddRequest", kTIMLog_Info, json_friend_add_request_pendency_array);
        }, this);

    TIMSetLogCallback([](TIMLogLevel level, const char* log, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        //ths->Log("SdkLog", level, log, false);
        }, this);
}

void IMServerImp::InitImSDK() { //��ʼ��ImSDK
    //��ȡ����
    Json::Value json_user_config;
    json_user_config[kTIMUserConfigIsReadReceipt] = true;  // �����Ѷ���ִ

    Json::Value json_socks5_value;
    json_socks5_value[kTIMSocks5ProxyInfoIp] = "111.222.333.444";
    json_socks5_value[kTIMSocks5ProxyInfoPort] = 8888;
    json_socks5_value[kTIMSocks5ProxyInfoUserName] = "";
    json_socks5_value[kTIMSocks5ProxyInfoPassword] = "";
    Json::Value json_config;
    json_config[kTIMSetConfigUserConfig] = json_user_config;
    //json_config[kTIMSetConfigSocks5ProxyInfo] = json_socks5_value;
    json_config[kTIMSetConfigUserDefineData] = "1.3.4.5.6.7";

    TIMSetConfig(json_config.toStyledString().c_str(), [](int32_t code, const char* desc, const char* json_param, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        ths->Logf("config", kTIMLog_Info, json_param);
        }, this);

    // ��ʼ��
    //std::string sdkappid = SDKAPPID//m_SdkAppIdCombo->GetText().GetStringA();
    std::string path = GetPath();// "D:\\code\\chat\\TIMSDK\\Windows\\Demo\\Bin\\Win64\\Debug";
    uint64_t sdk_app_id = GenerateTestUserSig::instance().getSDKAppID();

    std::string json_init_cfg;

    Json::Value json_value_init;
    json_value_init[kTIMSdkConfigLogFilePath] = path;
    json_value_init[kTIMSdkConfigConfigFilePath] = path;

    if (TIM_SUCC != TIMInit(sdk_app_id, json_value_init.toStyledString().c_str()))
    {
        Logf("InitSdk", kTIMLog_Info, "TIMInit fail");
    }
    //ChangeMainView(INITSDK_VIEW, LOGIN_VIEW);
    Logf("InitSdk", kTIMLog_Info, "sdkappid:%d Log&Cfg path:%s", sdk_app_id, path.c_str());
}

void IMServerImp::Login(string userid) {
    std::string usersig = GenerateTestUserSig::instance().genTestUserSig(userid);
    std::cout << "usersig=" << usersig;
    login_id = userid;
    if (TIM_SUCC != TIMLogin(userid.c_str(), usersig.c_str(), [](int32_t code, const char* desc, const char* json_param, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        if (code != ERR_SUCC) { 
            ths->Logf("Login", kTIMLog_Error, "Failure!code:%d desc", code, desc);
            return;
        }
        ths->Log("Login", kTIMLog_Info, "Success!");
        //ths->ChangeMainView(LOGIN_VIEW, MAIN_LOGIC_VIEW);
        //ths->SetControlVisible(_T("logout_btn"), true);
        //ths->SetControlVisible(_T("test_btn"), true);

        //ths->SetControlVisible(_T("cur_login_info"), true);
        //ths->SetControlText(_T("cur_login_info"), UTF82Wide(ths->login_id).c_str());

        //::PostMessage(ths->hwnd, WM_USER + 1, NULL, NULL);
        ths->InitConvList();
        ths->GetGroupJoinedList();
        }, this))
    {
        Logf("InitSdk", kTIMLog_Info, "TIMLogin fail");
    }

    Logf("Login", kTIMLog_Info, "User Id:%s Sig:%s", userid.c_str(), usersig.c_str());
    //SetControlText(_T("cur_loginid_lbl"), UTF82Wide(userid).c_str());
}

void IMServerImp::InitConvList() {
    int ret = TIMConvGetConvList([](int32_t code, const char* desc, const char* json_param, const void* user_data) {
        Json::Value json_conv_list;
        Json::Reader reader;
        if (!reader.parse(json_param, json_conv_list)) {
            IMServerImp::GetInst().Logf("GetConvList", kTIMLog_Error, "Json Parse Conv List Failure! error:%s", reader.getFormattedErrorMessages().c_str());
            return;
        }
        IMServerImp::GetInst().Logf("ConvList", kTIMLog_Info, json_param);
        for (Json::ArrayIndex i = 0; i < json_conv_list.size(); i++) {
            Json::Value& conv = json_conv_list[i];

            std::string id = conv[kTIMConvId].asString();
            uint32_t type = conv[kTIMConvType].asUInt();
            IMServerImp::GetInst().AddConv(id, type);
        }
        }, this);
    Logf("Init", kTIMLog_Info, "TIMConvGetConvList ret %d", ret);

}

void IMServerImp::AddConv(std::string id, uint32_t type) {
    if (type == TIMConvType::kTIMConv_C2C) {
        ConvInfo conv;
        conv.id = id;
        for (std::size_t i = 0; i < convs.size(); i++) {
            if (convs[i].id == id) {
                return;
            }
        }
        convs.push_back(conv);
        GetConvMsgs(id, (TIMConvType)type);
        //UpdateNodeView();
    }
    else if (type == TIMConvType::kTIMConv_Group) {
        GroupInfo group;
        group.id = id;
        for (std::size_t i = 0; i < groups.size(); i++) {
            if (groups[i].id == id) {
                return;
            }
        }
        groups.push_back(group);
        GetConvMsgs(id, (TIMConvType)type);
        //UpdateNodeView();
    }
    else if (type == TIMConvType::kTIMConv_System) {

    }
}

void IMServerImp::GetConvMsgs(std::string userid, TIMConvType type) {
    // �����Ự֮���Ȼ�ȡ�Ự��Ϣ
    Json::Value json_msg(Json::objectValue);
    Json::Value json_msgget_param;
    json_msgget_param[kTIMMsgGetMsgListParamLastMsg] = json_msg;
    json_msgget_param[kTIMMsgGetMsgListParamIsRamble] = true;
    json_msgget_param[kTIMMsgGetMsgListParamIsForward] = false;
    json_msgget_param[kTIMMsgGetMsgListParamCount] = 100;
    std::string json = json_msgget_param.toStyledString();
    int ret = TIMMsgGetMsgList(userid.c_str(), type, json.c_str(), [](int32_t code, const char* desc, const char* json_params, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        if (code != ERR_SUCC) { // ʧ��
            ths->Logf("Message", kTIMLog_Error, "GetMsg Failure! code:%d desc:%s", code, desc);
            return;
        }
        // �ɹ�
        ths->ParseMsg(json_params);
        }, this);

    if (ret != TIM_SUCC) {
        Logf("Conv", kTIMLog_Error, "Conv Get Msgs %s Failure!ret %d", userid.c_str(), ret);
    }
}

void IMServerImp::GetGroupJoinedList() { // �Ѽ���Ⱥ�б�
    // ��ȡȺ�б�
    int ret = TIMGroupGetJoinedGroupList([](int32_t code, const char* desc, const char* json_param, const void* user_data) {
        if (strlen(json_param) == 0) {
            return;
        }
        Json::Value json_group_list;
        Json::Reader reader;
        if (!reader.parse(json_param, json_group_list)) {
            IMServerImp::GetInst().Logf("GetJoinedGroup", kTIMLog_Error, "Json Parse Joined Group List!Failure! error:%s", reader.getFormattedErrorMessages().c_str());
            return;
        }
        IMServerImp::GetInst().Logf("GroupList", kTIMLog_Info, json_param);
        for (Json::ArrayIndex i = 0; i < json_group_list.size(); i++) {
            Json::Value& group = json_group_list[i];

            // ��ȡ�Ѽ���Ⱥ�Ļ�����Ϣ
            std::string groupid = group[kTIMGroupBaseInfoGroupId].asString();
            std::string groupName = group[kTIMGroupBaseInfoGroupName].asString();
            bool flag = false;
            for (std::size_t i = 0; i < IMServerImp::GetInst().groups.size(); i++) {
                if (IMServerImp::GetInst().groups[i].id == groupid) {
                    IMServerImp::GetInst().groups[i].name = groupName;
                    flag = true;
                    break;
                }
            }
            if (false == flag) { // ��ȡȺ���Ա
                GroupInfo group;
                group.id = groupid;
                group.name = groupName;
                IMServerImp::GetInst().groups.push_back(group);
            }
        }
        IMServerImp::GetInst().GetGroupInfoList();
        }, this);
    Logf("Init", kTIMLog_Info, "TIMGroupGetJoinedGroupList ret %d", ret);
}

void IMServerImp::GetGroupInfoList() {
    // ��ȡȺ����ϸ��Ϣ
    Json::Value groupids;
    for (std::size_t i = 0; i < IMServerImp::GetInst().groups.size(); i++) {
        GroupInfo& group = IMServerImp::GetInst().groups[i];
        groupids.append(group.id);
    }
    int ret = TIMGroupGetGroupInfoList(groupids.toStyledString().c_str(), [](int32_t code, const char* desc, const char* json_param, const void* user_data) {
        Json::Value json_groupmem_list;
        Json::Reader reader;
        if (!reader.parse(json_param, json_groupmem_list)) {
            IMServerImp::GetInst().Logf("GetGroupInfoList", kTIMLog_Error, "Parse Group Info List Failure!error:%s", reader.getFormattedErrorMessages().c_str());
            return;
        }
        //���Ի�ȡ������ϸ��Ⱥ��Ϣ
        for (Json::ArrayIndex i = 0; i < json_groupmem_list.size(); i++) {
            Json::Value& group_detail = json_groupmem_list[i][kTIMGetGroupInfoResultInfo];
            std::string groupid = group_detail[kTIMGroupDetialInfoGroupId].asString();
            IMServerImp::GetInst().GetGroupMemberInfoList(groupid);
        }
        //IMServerImp::GetInst().UpdateNodeView();
        }, this);
    Logf("Init", kTIMLog_Info, "TIMGroupGetJoinedGroupList ret %d", ret);
}


void IMServerImp::GetGroupMemberInfoList(std::string groupid) {
    // ��ȡ��Ա��Ϣ
    typedef struct {
        std::string groupid;
        IMServerImp* ths;
    }GetMemberInfoUserData;
    GetMemberInfoUserData* ud = new GetMemberInfoUserData;
    ud->ths = this;;
    ud->groupid = groupid;

    Json::Value json_group_getmeminfos_param;
    json_group_getmeminfos_param[kTIMGroupGetMemberInfoListParamGroupId] = groupid;
    Json::Value identifiers(Json::arrayValue);
    json_group_getmeminfos_param[kTIMGroupGetMemberInfoListParamIdentifierArray] = identifiers;
    Json::Value option;
    option[kTIMGroupMemberGetInfoOptionInfoFlag] = kTIMGroupMemberInfoFlag_None;
    option[kTIMGroupMemberGetInfoOptionRoleFlag] = kTIMGroupMemberRoleFlag_All;
    Json::Value customs(Json::arrayValue);
    option[kTIMGroupMemberGetInfoOptionCustomArray] = customs;
    json_group_getmeminfos_param[kTIMGroupGetMemberInfoListParamOption] = option;
    json_group_getmeminfos_param[kTIMGroupGetMemberInfoListParamNextSeq] = 0;

    int ret = TIMGroupGetMemberInfoList(json_group_getmeminfos_param.toStyledString().c_str(), [](int32_t code, const char* desc, const char* json_param, const void* user_data) {
        GetMemberInfoUserData* ud = (GetMemberInfoUserData*)user_data;
        if (strlen(json_param) == 0) {
            return;
        }
        Json::Value json_groupmem_res;
        Json::Reader reader;
        if (!reader.parse(json_param, json_groupmem_res)) {
            IMServerImp::GetInst().Logf("GetGroupMemberList", kTIMLog_Error, "Parse Group Member Info List Failure!error:%s", reader.getFormattedErrorMessages().c_str());
            delete ud;
            return;
        }
        Json::Value& json_groupmem_list = json_groupmem_res[kTIMGroupGetMemberInfoListResultInfoArray];
        for (std::size_t i = 0; i < IMServerImp::GetInst().groups.size(); i++) { // ���Ҷ�ӦȺ��
            GroupInfo& group = IMServerImp::GetInst().groups[i];
            if (group.id != ud->groupid) {
                continue;
            }
            for (Json::ArrayIndex m = 0; m < json_groupmem_list.size(); m++) {
                std::string memid = json_groupmem_list[m][kTIMGroupMemberInfoIdentifier].asString();
                GroupMemberInfo info;
                info.id = memid;
                group.mems.push_back(info);
            }
        }
        //IMServerImp::GetInst().UpdateNodeView();
        delete ud;
        }, ud);

    Logf("Init", kTIMLog_Info, "TIMGroupGetJoinedGroupList groupid:%s ret %d", groupid.c_str(), ret);
}


void IMServerImp::ParseMsg(const char* json_msg_array) {  //������Ϣ�ҵ���Ӧ�ĻỰ��Ϣ
    if (strlen(json_msg_array) == 0) {
        return;
    }
    msgs.push_back(json_msg_array);

    Json::Value json_value_msgs; // ��ʾ��Ϣ
    Json::Reader reader;
    if (!reader.parse(json_msg_array, json_value_msgs)) {
        Logf("Parse", kTIMLog_Error, reader.getFormattedErrorMessages().c_str());
        return;
    }
    for (Json::ArrayIndex i = 0; i < json_value_msgs.size(); i++) {  // ����Message
        Json::Value& json_value_msg = json_value_msgs[i];

        std::string id = json_value_msg[kTIMMsgConvId].asString();
        TIMConvType type = (TIMConvType)json_value_msg[kTIMMsgConvType].asUInt();
        if (false == json_value_msg[kTIMMsgIsRead].asBool()) {
            TIMMsgReportReaded(id.c_str(), type, json_value_msg.toStyledString().c_str(), [](int32_t code, const char* desc, const char* json_param, const void* user_data) {
                IMServerImp* ths = (IMServerImp*)user_data;

                }, this);
        }
        if (kTIMConv_C2C != type) {
            continue;
        }
        time_t time = json_value_msg[kTIMMsgClientTime].asUInt64();
        struct tm local;
        localtime_r(&time, &local);
        std::string sender = json_value_msg[kTIMMsgSender].asString();
        std::string content;
        Json::Value& json_value_elems = json_value_msg[kTIMMsgElemArray];
        for (Json::ArrayIndex i = 0; i < json_value_elems.size(); i++) { //���� elem array
            Json::Value& json_value_elem = json_value_elems[i];
            uint32_t elem_type = json_value_elem[kTIMElemType].asUInt();
            switch (elem_type) {
                case kTIMElem_Text: 
                    content += json_value_elem[kTIMTextElemContent].asString() + "\r\n";
                    break;
                default:
                    break;
            }
        }
        //std::string tmp = Fmt("%10s : %s %d-%d-%d %02d:%02d:%02d\r\n", "SystemMsg", sender.c_str(), (local.tm_year + 1900), (local.tm_mon + 1), local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);
        //m_LogData->AppendText(UTF82Wide(tmp).c_str());
        newMsgCallBack(sender, content);
    }
    //UpdateNodeView();
}

void IMServerImp::CreateConv(string userid) {  //�����Ự ��ť
    if (TIM_SUCC != TIMConvCreate(userid.c_str(), kTIMConv_C2C, [](int32_t code, const char* desc, const char* json_param, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;

        }, this)) {
        Logf("Conv", kTIMLog_Error, "�����Ựʧ��!userId:%s", userid.c_str());
        return;
    }
    ConvInfo conv;
    conv.id = userid;
    convs.push_back(conv);



    Json::Value json_value_text;  // ������Ϣ
    json_value_text[kTIMElemType] = kTIMElem_Text;
    json_value_text[kTIMTextElemContent] = "this draft";
    Json::Value json_value_msg;
    json_value_msg[kTIMMsgElemArray].append(json_value_text);

    Json::Value json_value_draft; // ����ݸ�
    json_value_draft[kTIMDraftEditTime] = time(NULL);
    json_value_draft[kTIMDraftUserDefine] = "this is userdefine";
    json_value_draft[kTIMDraftMsg] = json_value_msg;

    TIMConvSetDraft(userid.c_str(), TIMConvType::kTIMConv_C2C, json_value_draft.toStyledString().c_str());

    //UpdateNodeView();
}

bool IMServerImp::SendMsg(string user_id, string send_text) { 

    TIMCommCallback commcb = [](int32_t code, const char* desc, const char* json_param, const void* user_data) {
        IMServerImp* ths = (IMServerImp*)user_data;
        if (code != ERR_SUCC) { 
            ths->Logf("SendMsg", kTIMLog_Error, "code:%d desc:%s", code, desc);
            return;
        }

        ths->Log("SendMsg", kTIMLog_Info, "SendMsg Success");
        //ths->UpdateNodeView();
    };

    Json::Value json_value_text;
    json_value_text[kTIMElemType] = kTIMElem_Text;
    json_value_text[kTIMTextElemContent] = send_text;
    Json::Value json_value_msg;
    json_value_msg[kTIMMsgElemArray].append(json_value_text);
    json_value_msg[kTIMMsgSender] = login_id;
    json_value_msg[kTIMMsgClientTime] = time(NULL);
    json_value_msg[kTIMMsgServerTime] = time(NULL);

    for (std::size_t i = 0; i < convs.size(); i++) {
        ConvInfo& conv = convs[i];
        if (user_id != conv.id) {
            continue;
        }
        json_value_msg[kTIMMsgConvId] = conv.id;
        json_value_msg[kTIMMsgConvType] = kTIMConv_C2C;
        Json::Value json_value_msgs;
        json_value_msgs.append(json_value_msg);
        msgs.push_back(json_value_msgs.toStyledString());
        return TIMMsgSendNewMsg(conv.id.c_str(), kTIMConv_C2C, json_value_msg.toStyledString().c_str(), commcb, this) == TIM_SUCC;
    }
    for (std::size_t i = 0; i < groups.size(); i++) {
        GroupInfo& group = groups[i];
        if (user_id != group.id) {
            continue;
        }
        json_value_msg[kTIMMsgConvId] = group.id;
        json_value_msg[kTIMMsgConvType] = kTIMConv_Group;
        Json::Value json_value_msgs;
        json_value_msgs.append(json_value_msg);
        msgs.push_back(json_value_msgs.toStyledString());
        return TIMMsgSendNewMsg(group.id.c_str(), kTIMConv_Group, json_value_msg.toStyledString().c_str(), commcb, this) == TIM_SUCC;
    }
    Log("SendMsg", kTIMLog_Error, "Err2");
    return false;

}

#define STRING_FMT_MAX_LENGHT 0x100000
void IMServerImp::Logf(const char* module, TIMLogLevel level, const char* fmt, ...) {
    std::string tmp(STRING_FMT_MAX_LENGHT, 0);
    va_list ap;
    va_start(ap, fmt);
    vsnprintf((char*)tmp.c_str(), STRING_FMT_MAX_LENGHT - 1, fmt, ap);
    va_end(ap);
    Log(module, level, tmp.c_str());
}

void IMServerImp::Log(const char* module, TIMLogLevel level, const char* log, bool msgbox) {
    if (!module || !log) {
        return;
    }
    if (msgbox && ((level == kTIMLog_Error) || (level == kTIMLog_Assert))) {
        //MsgBox("Error", "module : %s\n %s ", module, log);
    }

    std::vector<std::string> stV;
    std::string str_log = log;
    std::string stSub;
    for (std::string::size_type i = 0; i < str_log.length();) {
        if (str_log[i] == 0) {
            break;
        }
        if (('\r' == str_log[i]) && ('\n' == str_log[i + 1])) {
            stV.push_back(stSub);
            i = i + 2;
            stSub = "";
            continue;
        }
        if ('\n' == str_log[i]) {
            stV.push_back(stSub);
            i = i + 1;
            stSub = "";
            continue;
        }
        stSub.push_back(str_log[i]);
        i = i + 1;
    }
    if (stSub != "") {
        stV.push_back(stSub);
    }
    for (std::size_t i = 0; i < stV.size(); i++) {
        cout << module << " : " << stV[i].c_str() << endl;
        //std::string tmp = Fmt("%10s : %s\r\n", module, stV[i].c_str());
        //m_LogData->AppendText(UTF82Wide(tmp).c_str());
    }
}