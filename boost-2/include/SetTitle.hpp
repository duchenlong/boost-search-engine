#pragma once 

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <cstring>
#include <cstdio>
#include <cstdlib>

extern char** environ;

// #define EXIT_SUCCESS 0  正常退出
// #define EXIT_FAILURE 1  异常退出
#define ERR_EXIT(err) \
do {\
    perror(err);\
    exit(EXIT_FAILURE); \
}while (0) ;

class ProTitle {
public:
    ProTitle(const int argc,char** argv);   // 初始化
    bool MoveOsEnv();                       // 环境变量搬移
    bool SetProcTitle(const char* name);    // 设置进程标题
    char* GetArgv(const int idx);           // 获取指定位置的命令行参数
    bool SetDaemon();                       // 设置主进程以守护进程运行
private:
    char**  _osArgv;        // 系统命令行参数的开始位置
    int     _osArgc;        // 系统命令行参数的数量
    char*   _envMove;       // 搬移后环境变量的位置
    int     _envMoveLen;    // 搬移后环境变量的大小
};
