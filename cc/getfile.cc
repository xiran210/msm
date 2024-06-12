#include "config.hpp"
#include "include/httplib/httplib.h"
#include <fstream>

void save_file(std::string path,std::string data)
{
    std::ofstream outfile;
	outfile.open(path);
	outfile << data;
	outfile.close();
}

void get_file(xiran_file file)
{
    #ifdef DEBUG
        printf("rename %s\n",file.rename);
        printf("url %s\n",file.url);
    #endif

    httplib::Client cli(file.url);
    auto res = cli.Get("/");

    #ifdef DEBUG
        std::cout<<res->status<<"\n";
        std::cout<<res->body<<"\n";
    #endif

    if(res->status!=200) return;
    
    switch (file.hide)
    {
        case hide_type::nohide:
            save_file(file.rename,res->body);
            break;
        case hide_type::image:
            //功能暂未实现
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