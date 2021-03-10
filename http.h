#include <iostream>
#include "curl/include/curl/curl.h"
#include "zlib/zlib.h"
#include <vector>
#include <string>
#include <windows.h>
#include <memory.h>
#include "json/json.h"
#include <sstream>
using namespace std;

class http {
public:
    http();
    ~http();
public:
    static size_t receive_data(void* contents, size_t size, size_t nmemb, void* stream);
    // HTTP �����ļ��Ļص�����
    static size_t writedata2file(void* ptr, size_t size, size_t nmemb, FILE* stream);
    // �ļ����ؽӿ�
    static int download_file(const char* url, const char outfilename[FILENAME_MAX]);
    // http get ����
    static CURLcode HttpGet(const std::string& strUrl, std::string& strResponse, int nTimeout);
    // htpp post ����
    static CURLcode HttpPost(const std::string& strUrl, std::string szJson, std::string& strResponse, int nTimeout);
};