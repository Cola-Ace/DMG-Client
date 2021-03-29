#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctime>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sys/stat.h>
#include <conio.h>
#include "include/dmg.h"
#include "include/base64.h"
#include "include/md5.h"
#include "include/rapidjson/document.h"
#include "include/rapidjson/stringbuffer.h"
#include "include/rapidjson/writer.h"

using namespace std;
using namespace rapidjson;
using namespace md5;

bool isLogin = false;
bool isVIP = false;
int userid = 0;

//Is Isset File
bool isFileExists_ifstream(string& name) {
    ifstream f(name.c_str());
    return f.good();
}

//Read Local File
string readFile(const char* filename) {
    FILE* fp;
    fopen_s(&fp, filename, "rb");
    if (!fp) {
        printf("open failed! file: %s", filename);
        return "";
    }

    char* buf = new char[1024 * 16];
    size_t n = fread(buf, 1, 1024 * 16, fp);
    fclose(fp);

    string result;
    if (n >= 0) {
        result.append(buf, 0, n);
    }
    delete[]buf;
    return result;
}


//==============
//== Function ==
//==============

void MainPage();
void FastJoin();
void Login();
void VIPCenter();
void ChangePass();
void ExchangeKey();
void ChangeName();
void Stats();
void Players(int);
void JoinGame(int);

//Get Client Info
bool IsValidUser(string, string);

int main() {
    isLogin = false;
    userid = 0;
    isVIP = false;
    MainPage();
}

bool IsValidUser(string user, string pass) {
    return true;
}

void MainPage() {
	DMG::PrintLogo();
    if (isLogin) {
        cout << "欢迎您, " << DMG::GetNickName(userid) << endl;
    }
	cout << "欢迎使用DMG社区启动器！" << endl;
	cout << "1) 快速加入服务器" << endl;
    cout << "2) 战绩统计" << endl;
    if (isLogin) {
        cout << "3) 退出登录" << endl;
        cout << "4) 用户中心" << endl;
    }
    else {
        cout << "3) 登录" << endl;
    }
	cout << "0) 退出" << endl;
	cout << "输入操作序号: ";
	int input;
	cin >> input;
    switch (input) {
    case 1:
        FastJoin();
        break;
    case 2:
        Stats();
        break;
    case 3:
        Login();
        break;
    case 4:
        if (isLogin) {
            VIPCenter();
        }
        break;
    }
    return;
}

void VIPCenter() {
    DMG::PrintLogo();
    string response, url, status;
    string keys[64], values[64];
    keys[0] = "uid";
    values[0] = to_string(userid);
    http::Post("http://api.dmgclub.cn/vip.php", DMG::CreateJSON(keys, values, 1), response, 10);
    Document document;
    document.Parse(response.data());
    status = document["Status"].GetString();
    if (status != "Success") {
        cout << "获取VIP数据失败!" << endl;
        isVIP = false;
    }
    else {
        isVIP = true;
    }
    time_t time = document["TimeStamp"].GetInt();
    string year;
    switch (document["Year"].GetInt()) {
    case 0:
        year = "否";
    case 1:
        year = "是";
    }
    cout << "欢迎您, " << DMG::GetNickName(userid) << endl;
    if (isVIP) {
        cout << "VIP过期时间: " << DMG::TimeToDate(time) << "\n是否为年VIP: " << year << endl;
    }
    cout << "1) 兑换卡密" << endl;
    cout << "2) 修改密码" << endl;
    cout << "3) 修改名字" << endl;
    cout << "0) 返回主界面" << endl;
    cout << "输入操作序号: ";
    int input;
    cin >> input;
    switch (input) {
    case 1:
        ExchangeKey();
        break;
    case 2:
        ChangePass();
        break;
    case 3:
        ChangeName();
        break;
    default:
        MainPage();
        break;
    }
    return;
}

void Stats() {
    DMG::PrintLogo();
}

void ChangeName() {
    DMG::PrintLogo();
    string response, keys[64], value[64];
    keys[0] = "uid";
    value[0] = to_string(userid);
    http::Post("http://api.dmgclub.cn/user.php", DMG::CreateJSON(keys, value, 1), response, 10);
    Document document;
    document.Parse(response.data());
    long long temp = document["Renaming_Time"].GetInt();
    if (temp != 0) {
        time_t last_renaming = temp;
        time_t renaming_time = temp + 2592000;
        time_t nowtime = time(NULL);
        cout << "上次改名时间: " << DMG::TimeToDate(last_renaming) << endl;
        cout << "当前时间: " << DMG::TimeToDate(nowtime) << endl;
        if (renaming_time > nowtime) {
            cout << "距离上次改名未超过30天, 无法改名! " << endl;
        }
    }
    else {
        cout << "你还没改过名字呢" << endl;
        string newname, temp1;
        cout << "输入新名称: ";
        cin >> newname;
        cout << "确定要改成 " << newname << " 吗? 输入 yes 确定, 输入 no 取消改名: ";
        cin >> temp1;
        if (temp1 == "yes") {
            keys[1] = "name";
            value[1] = newname;
            http::Post("http://api.dmgclub.cn/changename.php", DMG::CreateJSON(keys, value, 2), response, 10);
            cout << "改名成功! 你的新名字为 " << newname << endl;
        }
        else {
            cout << "你已经取消改名" << endl;
        }
    }
    system("pause");
    MainPage();
    return;
}

void ChangePass() {
    DMG::PrintLogo();
    string response, oldpass, newpass, temp, keys[64], value[64];
    keys[0] = "uid";
    keys[1] = "oldpass";
    keys[2] = "newpass";
    cout << "输入旧密码: ";
    oldpass = DMG::getPassword("*");
    cout << "输入新密码: ";
    temp = DMG::getPassword("*");
    cout << "再次输入新密码: ";
    newpass = DMG::getPassword("*");
    if (temp != newpass) {
        cout << "两次密码输入不一致!" << endl;
        system("pause");
        VIPCenter();
        return;
    }
    value[0] = to_string(userid);
    value[1] = oldpass;
    value[2] = newpass;
    http::Post("http://api.dmgclub.cn/changepass.php", DMG::CreateJSON(keys, value, 3), response, 10);
    Document document;
    document.Parse(response.data());
    string status = document["Status"].GetString();
    if (status != "Success") {
        cout << "发生错误! " << status << endl;
    }
    else {
        cout << "更改成功!" << endl;
        isLogin = false;
        isVIP = false;
        userid = 0;
    }
    system("pause");
    MainPage();
    return;
}

void ExchangeKey() {
    DMG::PrintLogo();
    string key, response, url, status;
    cout << "输入卡密: ";
    cin >> key;
    string keys[64], value[64];
    keys[0] = "uid";
    keys[1] = "key";
    value[0] = to_string(userid);
    value[1] = key;
    http::Post("http://api.dmgclub.cn/key.php", DMG::CreateJSON(keys, value, 2), response, 10);
    Document document;
    document.Parse(response.data());
    status = document["Status"].GetString();
    if (status == "Success") {
        cout << "兑换成功! 卡密天数: " << document["Days"].GetInt() << endl;
    }
    else {
        cout << "兑换失败" << endl;
    }
    system("pause");
    VIPCenter();
    return;
}

void Login() {
    if (isLogin) {
        isLogin = false;
        isVIP = false;
        userid = 0;
        MainPage();
        return;
    }
    DMG::PrintLogo();
    string user, pass, url, base64, response, sql;
    cout << "输入用户名: ";
    cin >> user;
    cout << "输入密码: ";
    pass = DMG::getPassword("*");
    string keys[64], value[64];
    keys[0] = "user";
    keys[1] = "pass";
    value[0] = user;
    value[1] = pass;
    http::Post("http://api.dmgclub.cn/check.php", DMG::CreateJSON(keys, value, 2), response, 10);
    Document document;
    document.Parse(response.data());
    string status = document["Status"].GetString();
    if (status == "Success") {
        userid = atoi(document["uid"].GetString());
        isLogin = true;
        cout << "登录成功! 欢迎回来, " << DMG::GetNickName(userid) << endl;
    }
    else {
        cout << "用户名或密码错误!" << endl;
    }
    system("pause");
    MainPage();
    return;
}

void FastJoin() {
	DMG::PrintLogo();
    string info = DMG::GetServerInfo();
    int count = DMG::GetCount(info);
    cout << "当前服务器数量: " << count << endl;
    for (int i = 0; i < count; i++) {
        cout << i << ") " << DMG::GetHostName(info, i) << " " << DMG::GetPlayers(info, i) << "/" << DMG::GetMaxPlayers(info, i) << " | 地图: " << DMG::GetMap(info, i) << endl;
    }
    cout << "0) 返回主界面" << endl;
    cout << "输入序号查看服务器详细信息: ";
    int input;
    cin >> input;
    if (input > count || input == 0) {
        MainPage();
    }
    for (int i = 1; i <= count; i++) {
        if (input == i) {
            Players(i);
            break;
        }
    }
    return;
}

void Players(int sid) {
    DMG::PrintLogo();
    string response, url, keys[64], value[64];
    keys[0] = "sid";
    value[0] = to_string(sid);
    http::Post("http://api.dmgclub.cn/players.php", DMG::CreateJSON(keys, value, 1), response, 10);
    Document document;
    document.Parse(response.data());
    int count = document["Count"].GetInt();
    cout << "当前服务器共有 " << count << " 名玩家" << endl;
    if (count > 0) {
        for (int i = 0; i < count; i++) {
            cout << DMG::UTF8ToGBK(document["Players"][i]["Name"].GetString()) << " 得分:" << document["Players"][i]["Frags"].GetInt() << " 时长:" << document["Players"][i]["TimeF"].GetString() << endl;
        }
    }
    cout << "输入 1 加入该服务器, 输入其他返回服务器列表: ";
    int input;
    cin >> input;
    switch (input) {
    case 1:
        JoinGame(sid);
        MainPage();
        break;
    default:
        FastJoin();
        break;
    }
    return;
}

void JoinGame(int index) {
    string response, url;
    response = DMG::GetServerInfo();
    url = DMG::format("start steam://connect/%s:%i", DMG::GetIP(response, index), DMG::GetPort(response, index));
    system(url.data());
    return;
}