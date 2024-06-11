#include "iostream"
#include "mutex"
#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"

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
    assert(d.GetType()==3);
 
    // 版本检查
    Value& version = d["version"];
    assert(version.GetType()==5);
    assert(strcmp(version.GetString(),"1.0")==0);
}

const char* get_shell_host()
{
    std::lock_guard<std::mutex> guard(d_mtx);
    Value& shell_host = d["shell_host"];
    assert(shell_host.GetType()==5);
    return shell_host.GetString();
}

const char* get_shell_port()
{
    std::lock_guard<std::mutex> guard(d_mtx);
    Value& shell_port = d["shell_port"];
    assert(shell_port.GetType()==5);
    return shell_port.GetString();
}

const char* get_shell_protocol()
{
    std::lock_guard<std::mutex> guard(d_mtx);
    Value& shell_protocol = d["shell_protocol"];
    assert(shell_protocol.GetType()==5);
    return shell_protocol.GetString();
}

bool get_enable_shell()
{
    std::lock_guard<std::mutex> guard(d_mtx);
    Value& enable_shell = d["enable_shell"];
    assert(enable_shell.GetType()==1 || enable_shell.GetType()==2);
    return enable_shell.GetType()==2 ? true : false;
}

int get_retry_time()
{
    std::lock_guard<std::mutex> guard(d_mtx);
    Value& retry_time = d["retry_time"];
    assert(retry_time.GetType()==6);
    return retry_time.GetInt();
}