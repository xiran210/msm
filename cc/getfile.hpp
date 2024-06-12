#include <string>
#include "hide.hpp"

struct xiran_file{

    std::string rename;//保存的文件名以及地址
    std::string url;//文件获得地址
    hide_type hide;//是否使用隐写

    //构造函数
    xiran_file(std::string rename,std::string url,hide_type hide){
        this->rename=rename;
        this->url=url;
        this->hide=hide;
    }

};

void start_file_get();