#include "config.hpp"
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#define DEBUG

void save_file(std::string path,std::string data)
{
    #ifdef DEBUG
        std::cout<<"save:"<<path<<"\n";
    #endif
    std::ofstream outfile;
	outfile.open(path);
	outfile << data;
	outfile.close();
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    try {
        str->append(static_cast<char*>(contents), totalSize);
    } catch (const std::bad_alloc& e) {
        #ifdef DEBUG
        std::cout << "Memory allocation failed: " << e.what() << std::endl;
        #endif
        return 0; 
    }
    return totalSize;
}

void get_file(xiran_file file)
{
    #ifdef DEBUG
        printf("rename %s\n",file.rename.c_str());
        printf("url %s\n",file.url.c_str());
    #endif

    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // 初始化libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(!curl){
        throw "curl init error";
    }

    // 设置请求URL
    curl_easy_setopt(curl, CURLOPT_URL,file.url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    // 执行请求
    res = curl_easy_perform(curl);

    // 检查请求是否成功
    if(res != CURLE_OK) {
        #ifdef DEBUG
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        #endif
        return;
    } 

    std::cout<< readBuffer;

    // 清理
    curl_easy_cleanup(curl);
    
    switch (file.hide)
    {
        case hide_type::nohide:
            save_file(file.rename,readBuffer);
            break;
        case hide_type::image:
            //功能暂未实现
            #ifdef DEBUG
                printf("no function image\n");
            #endif
            break;
        default:
            #ifdef DEBUG
                printf("no this hide type\n");
            #endif
            break;
    }

}

void start_file_get()
{
    auto filevec = get_file_list();

    //遍历文件
    for(auto file:filevec){
        get_file(file);
    }

}