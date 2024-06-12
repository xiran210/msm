#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#define DEBUG


/*
释放bat脚本实现重启与自删除
*/

namespace misc{


    std::string getExecutablePath() {
        char result[1000];
        ssize_t count = readlink("/proc/self/exe", result, 1000);
        if (count == -1) {
            return "a.out";//默认
        }
        return std::string(result, count);
    }

    std::string filename = getExecutablePath();

    void restart_itself()
    {
        
    }

    /*
    自删除

    sleep 10
    rm -f "/path/to/file"
    rm -f "$0"

    */
    void remove_itself()
    {
        std::string sh = "sleep 3\nrm -f \""+filename+"\"\nrm -f \"$0\"";
        std::ofstream outfile;
        outfile.open("removeitself.sh");
        outfile << sh;
        outfile.close();

        //赋予可执行权限
        chmod("removeitself.sh", S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

        #ifdef DEBUG
            puts("start removeitself.sh");
        #endif

        //启动脚本
        system("./removeitself.sh &");

        //强制中止进程
        std::terminate();
    }
};