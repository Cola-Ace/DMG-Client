#include <stdlib.h>
#include "post.h"

//====== Curl Start ======
size_t CommonTools::receive_data(void* contents, size_t size, size_t nmemb, void* stream) {
    string* str = (string*)stream;
    (*str).append((char*)contents, size * nmemb);
    return size * nmemb;
}

size_t CommonTools::writedata2file(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int CommonTools::download_file(const char* url, const char outfilename[FILENAME_MAX]) {
    CURL* curl;
    FILE* fp;
    CURLcode res;
    /*   ����curl_global_init()��ʼ��libcurl  */
    res = curl_global_init(CURL_GLOBAL_ALL);
    if (CURLE_OK != res)
    {
        printf("init libcurl failed.");
        curl_global_cleanup();
        return -1;
    }
    /*  ����curl_easy_init()�����õ� easy interface��ָ��  */
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outfilename, "wb");

        /*  ����curl_easy_setopt()���ô���ѡ�� */
        res = curl_easy_setopt(curl, CURLOPT_URL, url);
        if (res != CURLE_OK)
        {
            fclose(fp);
            curl_easy_cleanup(curl);
            return -1;
        }
        /*  ����curl_easy_setopt()���õĴ���ѡ�ʵ�ֻص�����������û��ض�����  */
        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CommonTools::writedata2file);
        if (res != CURLE_OK) {
            fclose(fp);
            curl_easy_cleanup(curl);
            return -1;
        }
        /*  ����curl_easy_setopt()���õĴ���ѡ�ʵ�ֻص�����������û��ض�����  */
        res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        if (res != CURLE_OK)
        {
            fclose(fp);
            curl_easy_cleanup(curl);
            return -1;
        }

        res = curl_easy_perform(curl);
        // ����curl_easy_perform()������ɴ�������
        fclose(fp);
        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            return -1;
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
        // ����curl_easy_cleanup()�ͷ��ڴ�

    }
    curl_global_cleanup();
    return 0;
}

CURLcode CommonTools::HttpGet(const std::string& strUrl, std::string& strResponse, int nTimeout) {
    CURLcode res;
    CURL* pCURL = curl_easy_init();

    if (pCURL == NULL) {
        return CURLE_FAILED_INIT;
    }

    curl_easy_setopt(pCURL, CURLOPT_URL, strUrl.c_str());
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(pCURL, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(pCURL, CURLOPT_TIMEOUT, nTimeout);
    curl_easy_setopt(pCURL, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(pCURL, CURLOPT_WRITEFUNCTION, CommonTools::receive_data);
    curl_easy_setopt(pCURL, CURLOPT_WRITEDATA, (void*)&strResponse);
    res = curl_easy_perform(pCURL);
    curl_easy_cleanup(pCURL);
    return res;
}

CURLcode CommonTools::HttpPost(const std::string& strUrl, std::string szJson, std::string& strResponse, int nTimeout) {
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

    ret = curl_easy_setopt(pCURL, CURLOPT_WRITEFUNCTION, CommonTools::receive_data);

    ret = curl_easy_setopt(pCURL, CURLOPT_WRITEDATA, (void*)&strResponse);

    res = curl_easy_perform(pCURL);
    curl_easy_cleanup(pCURL);
    return res;
}
//====== Curl End ======