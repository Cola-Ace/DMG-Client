#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctime>
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
        cout << "��ӭ��, " << DMG::GetNickName(userid) << endl;
    }
	cout << "��ӭʹ��DMG������������" << endl;
	cout << "1) ���ټ��������" << endl;
    if (isLogin) {
        cout << "2) �˳���¼" << endl;
        cout << "3) �û�����" << endl;
    }
    else {
        cout << "2) ��¼" << endl;
    }
	cout << "0) �˳�" << endl;
	cout << "�������ѡ��: ";
	int input;
	cin >> input;
    switch (input) {
    case 1:
        FastJoin();
        break;
    case 2:
        Login();
        break;
    case 3:
        if (isLogin) {
            VIPCenter();
        }
        break;
    }
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
        cout << "��ȡVIP����ʧ��!" << endl;
        isVIP = false;
    }
    else {
        isVIP = true;
    }
    time_t time = document["TimeStamp"].GetInt();
    string year;
    switch (document["Year"].GetInt()) {
    case 0:
        year = "��";
    case 1:
        year = "��";
    }
    cout << "��ӭ��, " << DMG::GetNickName(userid) << endl;
    if (isVIP) {
        cout << "VIP����ʱ��: " << DMG::TimeToDate(time) << "\n�Ƿ�Ϊ��VIP: " << year << endl;
    }
    cout << "1) �һ�����" << endl;
    cout << "2) �޸�����" << endl;
    cout << "0) ����������" << endl;
    int input;
    cin >> input;
    switch (input) {
    case 1:
        ExchangeKey();
        break;
    case 2:
        ChangePass();
        break;
    default:
        MainPage();
        break;
    }
}

void ChangePass() {
    DMG::PrintLogo();
    string response, oldpass, newpass, temp, keys[64], value[64];
    keys[0] = "uid";
    keys[1] = "oldpass";
    keys[2] = "newpass";
    cout << "���������: ";
    oldpass = DMG::getPassword("*");
    cout << "����������: ";
    temp = DMG::getPassword("*");
    cout << "�ٴ�����������: ";
    newpass = DMG::getPassword("*");
    if (temp != newpass) {
        cout << "�����������벻һ��!" << endl;
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
        cout << "��������! " << status << endl;
    }
    else {
        cout << "���ĳɹ�!" << endl;
        isLogin = false;
        isVIP = false;
        userid = 0;
    }
    system("pause");
    MainPage();
}

void ExchangeKey() {
    DMG::PrintLogo();
    string key, response, url, status;
    cout << "���뿨��: ";
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
        cout << "�һ��ɹ�! ��������: " << document["Days"].GetInt() << endl;
    }
    else {
        cout << "�һ�ʧ��" << endl;
    }
    system("pause");
    VIPCenter();
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
    cout << "�����û���: ";
    cin >> user;
    cout << "��������: ";
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
        cout << "��¼�ɹ�! ��ӭ����, " << DMG::GetNickName(userid) << endl;
    }
    else {
        cout << "�û������������!" << endl;
    }
    system("pause");
    MainPage();
}

void FastJoin() {
	DMG::PrintLogo();
    string info;
    http::Get("http://api.dmgclub.cn/server.php?count", info, 10);
    int count = DMG::GetCount(info);
    string keys[64], value[64];
    cout << "��ǰ����������: " << count << endl;
    keys[0] = "sid";
    for (int i = 1; i <= count; i++) {
        info = "";
        value[0] = to_string(i);
        http::Post("http://api.dmgclub.cn/server.php", DMG::CreateJSON(keys, value, 1), info, 10);
        cout << i << ") " << DMG::GetHostName(info) << " " << DMG::GetPlayers(info) << "/" << DMG::GetMaxPlayers(info) << " | ��ͼ: " << DMG::GetMap(info) << endl;
    }
    cout << "0) ����������" << endl;
    cout << "���������������������: ";
    int input;
    cin >> input;
    if (input > count || input == 0) {
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
    string response, url, keys[64], value[64];
    keys[0] = "sid";
    value[0] = to_string(sid);
    http::Post("http://api.dmgclub.cn/server.php", DMG::CreateJSON(keys, value, 1), response, 10);
    url = DMG::format("start steam://connect/%s:%i", DMG::GetIP(response), DMG::GetPort(response));
    system(url.data());
}