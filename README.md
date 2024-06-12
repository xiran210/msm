# To Do List
- shell 反代连接
    - [x] 执行shell命令
    - [x] 等待指定时间重连尝试
    - [ ] 修改配置文件
    - [ ] 双向传输文件
  
- 配置文件获取与解析
    - [x] 配置文件解析(部分解析) 
    - [x] 远程获得配置文件
    - [ ] 定时请求更新配置文件

- 通信协议
    - [x] 无加密TCP
    - [ ] 固定密钥的对称加密TCP
    - [ ] TLS加密通信
    - [ ] 基于ICMP协议的通信

- 监听端口
    - [ ] 开启端口监听
    - [ ] 监听后向服务器注册

- 服务端
    - [x] 简易shell反连交互器
    - [ ] 配置文件获取服务器
    - [ ] 监听注册器以及连接器

- 释放文件
    - [x] 远程下载文件

- 定时任务
    - [ ] 定时执行命令
    - [ ] 定时启动某程序

- 隐写技术
    - [ ] 图片隐写，使用图片隐写

- 执行shellcode
    - [ ] 远程获得shellcode加载到内存执行

- 反逆向
    - [x] 字符串加密(功能代码已实现,c.cc中xiran_str，为方便后续功能开发，大部分字符串暂未接入此功能)
    - [ ] 伪装功能（将程序入口伪装为ls或其他正常程序的功能）
    - [ ] 启动时需指定密钥，利用密钥实现SMC解密

  
# how to use

主体程序位于文件夹 `cc` 中，主程序为`c.cc`

`server`文件夹存放测试使用的服务端，正式的服务端还在计划中

编译参数,需要c++11或者更高

```shell
g++ c.cc config.cc getfile.cc shell.cc -pthread -lcurl
```

配置文件设置[请看此文件](./配置文件设计.md)

# 使用的第三方库

- httplib(用于发送http请求) 已经包含文件无需安装
  - 2024年6月12日 移除
- rapidjson(用于解析json) 已经包含文件无需安装
- boost(用于TCP连接) 需提前安装
- libcurl 需提前安装

# 工作进度

- 2024年6月11日 v1.0版本提交

- 2024年6月12日 filedown测试结束

# 测试

## 字符串加密测试

字符串初始化时使用`init_xiran_string`函数进行初始，使用了编译时运行技术，编译后的程序中将不会出现明文的字符串

```cpp
xiran_string xiran_str[xiran_string_count]={
    init_xiran_string("http://47.109.186.209:8000"),
    init_xiran_string("test")
};
```

使用字符串时调用函数 `std::unique_ptr<char[]> get_xiran_string(int index)`，通过字符串索引返回包含解密后的字符串的智能指针

## 文件下载测试

需要下载的文件需要在[配置文件](./配置文件设计.md)中配置

测试使用的服务端为 `./server/test/filedownload/server.bat`，请提取准备python3环境

测试文件同时在其目录下

## shell 反连测试

shell反连的地址需要在[配置文件](./配置文件设计.md)中配置

测试使用的服务端为 `./server/kz/server.py`，请提取准备python3环境

## 远程拉取配置文件测试

若本地保存有配置文件，则优先使用同一目录下的`config.json`

远程地址固定编码在可执行程序中，如需修改请修改源码之后重新编译

测试使用的服务端为 `./server/test/filedownload/server.bat`，请提取准备python3环境

测试使用的`config.json`在其子目录下

测试使用的远程地址为`http://127.0.0.1:8080/testfile/config.json`