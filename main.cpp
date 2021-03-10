#include <iostream>
#include <string>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include "http.h"
#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

string UTF8ToGBK(const char* src_str)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
    wchar_t* wszGBK = new wchar_t[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char* szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
    string strTemp(szGBK);
    if (wszGBK) delete[] wszGBK;
    if (szGBK) delete[] szGBK;
    return strTemp;
}

template<typename ... Args>
static std::string format(const std::string& format, Args ... args)
{
    auto size_buf = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
    std::unique_ptr<char[]> buf(new(std::nothrow) char[size_buf]);

    if (!buf)
        return std::string("");

    std::snprintf(buf.get(), size_buf, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size_buf - 1);
}

//==============
//== Function ==
//==============

void Delay(int);
void MainPage();
void PrintLogo();
void FastJoin();
void JoinGame(int);

//Get Server Info
string GetServerInfo(int);
string GetHostName(string);
string GetMap(string);
string GetIP(string);
int GetPort(string);
int GetCount(string);
int GetPlayers(string);
int GetMaxPlayers(string);

int main() {
    MainPage();
}

//Get Server Info
string GetServerInfo(int sid) {
    string response, url;
    url = format("http://api.dmgclub.cn/server.php?sid=%i", sid);
    http::HttpGet(url, response, 10);
    const char* temp = new char[response.length() + 1];
    temp = response.data();
    return UTF8ToGBK(temp);
}

string GetHostName(string info) {
    Document document;
    const char* temp = info.data();
    document.Parse(temp);
    const char* hostname = document["HostName"].GetString();
    return UTF8ToGBK(hostname);
}

string GetMap(string info) {
    Document document;
    const char* temp = info.data();
    document.Parse(temp);
    const char* map = document["Map"].GetString();
    string temp1 = map;
    return temp1;
}

string GetIP(string info){
    Document document;
    const char* temp = info.data();
    document.Parse(temp);
    const char* IP = document["IP"].GetString();
    string temp1 = IP;
    return temp1;
}

int GetPort(string info) {
    Document document;
    const char* temp = info.data();
    document.Parse(temp);
    const char* map = document["Port"].GetString();
    string temp1 = map;
    return atoi(temp1.c_str());
}

int GetCount(string info) {
    Document document;
    const char* temp = info.data();
    document.Parse(temp);
    const char* count = document["Count"].GetString();
    string temp1 = count;
    return atoi(temp1.c_str());
}

int GetPlayers(string info) {
    Document document;
    const char* temp = info.data();
    document.Parse(temp);
    return document["Players"].GetInt();
}

int GetMaxPlayers(string info) {
    Document document;
    const char* temp = info.data();
    document.Parse(temp);
    return document["MaxPlayers"].GetInt();
}

void MainPage() {
	system("cls");
	PrintLogo();
	cout << "欢迎使用DMG社区启动器！" << endl;
	cout << "1) 快速加入服务器" << endl;
	cout << "0) 退出" << endl;
	cout << "输入你的选择: ";
	int input;
	cin >> input;
	switch (input) {
	case 1:
		FastJoin();
		break;
	default:
		break;
	}
}

void FastJoin() {
	system("cls");
	PrintLogo();
    string info, temp, url;
    http::HttpGet("http://api.dmgclub.cn/server.php?sid=1", info, 10);
    int count = GetCount(info);
    cout << "当前服务器数量: " << count << endl;
    for (int i = 1; i <= count; i++) {
        info = "";
        url = "";
        url = format("http://api.dmgclub.cn/server.php?sid=%i", i);
        http::HttpGet(url, info, 10);
        cout << i << ") " << GetHostName(info) << " " << GetPlayers(info) << "/" << GetMaxPlayers(info) << " | 地图: " << GetMap(info) << endl;
    }
    cout << "0) 返回主界面" << endl;
    cout << "输入你想加入服务器的序号: ";
    int input;
    cin >> input;
    if (input > count) {
        MainPage();
    }
    for (int i = 1; i <= count; i++) {
        if (input == i) {
            JoinGame(i);
            break;
        }
    }
    MainPage();
}

void JoinGame(int sid) {
    string response, url;
    url = format("http://api.dmgclub.cn/server.php?sid=%i", sid);
    http::HttpGet(url, response, 10);
    string IP = GetIP(response);
    int port = GetPort(response);
    url = format("start steam://connect/%s:%i", IP, port);
    const char* cmd = url.data();
    system(cmd);
    return;
}

void Delay(int time) {
	clock_t now = clock();
	while (clock() - now < time);
}

void PrintLogo() {
    /*
     ____  __  __  ____
    |  _ \|  \/  |/ ___|
    | | | | |\/| | |  _
    | |_| | |  | | |_| |
    |____/|_|  |_|\____|
    */
	string logo[5];
	string border = "***========================***\n";
	logo[0] = "      ____  __  __  ____\n";
	logo[1] = "*    |  _ \\|  \\/  |/ ___|    *\n";
	logo[2] = "*    | | | | |\\/| | |  _     *\n";
	logo[3] = "*    | |_| | |  | | |_| |    *\n";
	logo[4] = "*    |____/|_|  |_|\\____|    *\n\n";
	cout << border << logo[0] << logo[1] << logo[2] << logo[3] << logo[4] << border << endl;
}