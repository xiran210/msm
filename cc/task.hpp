#include <iostream>
#include <string>
#include <vector>
#include <time.h>

namespace task{

    //全局任务列表
    std::vector<xiran_task> task_vec;

    struct xiran_task{
        std::string name;//任务名
        int Interval_time;//任务多久启动一次,单位秒,若只运行一次设置为-1
        std::string start_cmd;//任务命令
        int last_runtime=0;//上次运行时间

        xiran_task(std::string name,int Interval_time,std::string start_cmd){
            this->name = name;
            this->Interval_time = Interval_time;
            this->start_cmd = start_cmd;
        }

        void start_task()
        {
            int cur_time = time(0);
            if(cur_time - last_runtime < Interval_time)
                return;
            last_runtime = cur_time;
            system(start_cmd.c_str());
        }
    };
};

