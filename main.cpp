/*
#define BUILDING_LIBCURL
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "libcurl.lib")
*/
#include <iostream>
#include <string>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include "http.h"
using namespace std;
/*
#define CURL_GLOBAL_SSL (1<<0)
#define CURL_GLOBAL_WIN32 (1<<1)
#define CURL_GLOBAL_ALL (CURL_GLOBAL_SSL|CURL_GLOBAL_WIN32)

//====== Curl Start ======
size_t receive_data(void* contents, size_t size, size_t nmemb, void* stream) {
    string* str = (string*)stream;
    (*str).append((char*)contents, size * nmemb);
    return size * nmemb;
}

size_t writedata2file(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int download_file(const char* url, const char outfilename[FILENAME_MAX]) {
    CURL* curl;
    FILE* fp;
    CURLcode res;
    //   调用curl_global_init()初始化libcurl  
    res = curl_global_init(CURL_GLOBAL_ALL);
    if (CURLE_OK != res)
    {
        printf("init libcurl failed.");
        curl_global_cleanup();
        return -1;
    }
    //  调用curl_easy_init()函数得到 easy interface型指针  
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outfilename, "wb");

        /*  调用curl_easy_setopt()设置传输选项 
        res = curl_easy_setopt(curl, CURLOPT_URL, url);
        if (res != CURLE_OK)
        {
            fclose(fp);
            curl_easy_cleanup(curl);
            return -1;
        }
        /*  根据curl_easy_setopt()设置的传输选项，实现回调函数以完成用户特定任务  
        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writedata2file);
        if (res != CURLE_OK) {
            fclose(fp);
            curl_easy_cleanup(curl);
            return -1;
        }
        /*  根据curl_easy_setopt()设置的传输选项，实现回调函数以完成用户特定任务  
        res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        if (res != CURLE_OK)
        {
            fclose(fp);
            curl_easy_cleanup(curl);
            return -1;
        }

        res = curl_easy_perform(curl);
        // 调用curl_easy_perform()函数完成传输任务
        fclose(fp);
        /* Check for errors 
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            return -1;
        }

        /* always cleanup 
        curl_easy_cleanup(curl);
        // 调用curl_easy_cleanup()释放内存

    }
    curl_global_cleanup();
    return 0;
}

CURLcode HttpGet(const std::string& strUrl, std::string& strResponse, int nTimeout) {
    CURLcode res;
    CURL* pCURL = curl_easy_init();

    if (pCURL == NULL) {
        return CURLE_FAILED_INIT;
    }

    struct curl_slist* head = NULL;
    head = curl_slist_append(head, "Content-Type:text/plain;charset=UTF-8");
    curl_easy_setopt(pCURL, CURLOPT_HTTPHEADER, head);

    curl_easy_setopt(pCURL, CURLOPT_URL, strUrl.c_str());
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(pCURL, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(pCURL, CURLOPT_TIMEOUT, nTimeout);
    curl_easy_setopt(pCURL, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(pCURL, CURLOPT_WRITEFUNCTION, receive_data);
    curl_easy_setopt(pCURL, CURLOPT_WRITEDATA, (void*)&strResponse);
    res = curl_easy_perform(pCURL);
    curl_slist_free_all(head);
    curl_easy_cleanup(pCURL);
    return res;
}

CURLcode HttpPost(const std::string& strUrl, std::string szJson, std::string& strResponse, int nTimeout) {
    CURLcode res;
    char szJsonData[1024];
    memset(szJsonData, 0, sizeof(szJsonData));
    strcpy(szJsonData, szJson.c_str());
    CURL* pCURL = curl_easy_init();
    struct curl_slist* headers = NULL;
    if (pCURL == NULL) {
        return CURLE_FAILED_INIT;
    }

    CURLcode ret;
    ret = curl_easy_setopt(pCURL, CURLOPT_URL, strUrl.c_str());
    //    std::cout << ret << std::endl;

    ret = curl_easy_setopt(pCURL, CURLOPT_POST, 1L);
    headers = curl_slist_append(headers, "content-type:application/json");

    ret = curl_easy_setopt(pCURL, CURLOPT_HTTPHEADER, headers);

    ret = curl_easy_setopt(pCURL, CURLOPT_POSTFIELDS, szJsonData);
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    ret = curl_easy_setopt(pCURL, CURLOPT_TIMEOUT, nTimeout);

    ret = curl_easy_setopt(pCURL, CURLOPT_WRITEFUNCTION, receive_data);

    ret = curl_easy_setopt(pCURL, CURLOPT_WRITEDATA, (void*)&strResponse);

    res = curl_easy_perform(pCURL);
    curl_easy_cleanup(pCURL);
    return res;
}
//====== Curl End ======
*/
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
void JoinGame(string, int);

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
    string response, url;
    url = format("http://api.dmgclub.cn/info.php?ip=%s&port=%s&info=hostname", ip, port);
    http::HttpGet(url, response, 10);
    const char* temp = new char[response.length() + 1];
    temp = response.data();
    return UTF8ToGBK(temp);
}

string GetMap(string info) {

}

string GetIP(string info){
    
}

int GetPort(string info) {

}

int GetCount(string info) {

}

int GetPlayers(string info) {

}

int GetMaxPlayers(string info) {

}

void MainPage() {
	system("cls");
	PrintLogo();
	cout << "欢迎使用DMG社区启动器！" << endl;
	cout << "1) 服务器列表(开发中)" << endl;
	cout << "2) 快速加入服务器" << endl;
	cout << "0) 退出" << endl;
	cout << "输入你的选择: ";
	int input;
	cin >> input;
	switch (input) {
	case 2:
		FastJoin();
		break;
	default:
		break;
	}
}

void FastJoin() {
	system("cls");
	PrintLogo();
    string response;
    http::HttpGet("http://api.dmgclub.cn/server.php?method=server_count", response, 10);
    int count = atoi(response.c_str());
    cout << "当前服务器数量: " << count << endl;
    response = "";
    string hostname, url, ip, port, map, maxplayers, players;
    for (int i = 1; i <= count; i++) {
        url = format("http://api.dmgclub.cn/server.php?method=server_ip&sid=%i", i);
        http::HttpGet(url, ip, 10);
        url = format("http://api.dmgclub.cn/server.php?method=server_port&sid=%i", i);
        http::HttpGet(url, port, 10);
        url = format("http://api.dmgclub.cn/info.php?ip=%s&port=%s&info=hostname", ip, port);
        http::HttpGet(url, response, 10);
        const char* temp = new char[response.length() + 1];
        temp = response.data();
        hostname = UTF8ToGBK(temp);
        url = format("http://api.dmgclub.cn/info.php?ip=%s&port=%s&info=map", ip, port);
        http::HttpGet(url, map, 10);
        url = format("http://api.dmgclub.cn/info.php?ip=%s&port=%s&info=players", ip, port);
        http::HttpGet(url, players, 10);
        url = format("http://api.dmgclub.cn/info.php?ip=%s&port=%s&info=maxplayers", ip, port);
        http::HttpGet(url, maxplayers, 10);
        cout << i << ") " << hostname << " " << players << "/" << maxplayers << " | 地图: " << map << endl;
        response = hostname = url = ip = port = map = maxplayers = players = "";
        delete []temp;
        Delay(100);
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

void JoinGame(string ip, int port) {
    string url, response;
    url = format("steam://connect/%s:%i", ip, port);
    http::HttpGet(url, response, 10);
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