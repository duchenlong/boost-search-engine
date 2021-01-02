#include "SetTitle.hpp"

// 初始化，环境变量搬移
ProTitle::ProTitle(const int argc,char** argv) {
    _osArgv = argv;     // 得到命令行参数的位置
    _osArgc = argc;     // 得到命令行参数的数量
    _envMoveLen = 0;
    _envMove = nullptr;
}

bool ProTitle::MoveOsEnv() {
    // 得到环境变量的大小
    for (int i = 0; environ[i] != nullptr; i++)
        _envMoveLen += strlen(environ[i]) + 1;  // '\0'

    // 为环境变量分配空间
    _envMove = (char*)malloc(sizeof(char) * _envMoveLen);
    memset(_envMove,0x00,_envMoveLen);
    if(_envMove == nullptr) return false;   // 空间分配出错
    
    char* tmp = _envMove;
    for(int i = 0; environ[i] != nullptr; i++) {
        int len = strlen(environ[i]) + 1;
        strcpy(tmp,environ[i]);
        environ[i] = tmp;       // 顺便更新系统环境变量的位置
        tmp += len;
    }

    return true;
}

// 设置进程标题
bool ProTitle::SetProcTitle(const char* name) {
    if(name == nullptr) return false;
    printf("set title ing is [%s]\n" , name);
    int titleLen = strlen(name) + 1;
    
    int argLen = 0;     // 命令行参数的长度
    for(int i = 0; _osArgv[i] != nullptr; i++)
        argLen += strlen(_osArgv[i]) + 1;
    
    int titleArgLen = strlen(_osArgv[0]) + 1;   // 原本标题的命令行参数大小
    int othreArgLen = argLen - titleArgLen;     // 除了标题外，其他命令行参数的大小
    
    if(titleArgLen + _envMoveLen < titleLen) return false;   // 名字太长，放不下

    // 命令行参数 下标为0 的位置为进程的标题，本项目中默认是 ./main
    char* tmp = _osArgv[0];
    printf("%s\n",_osArgv[0]);
    memmove(tmp+titleLen,tmp+titleArgLen,othreArgLen);
    strcpy(tmp,name);   // 设置新标题

    // 后面的位置都清零, 命令行参数后的环境变量
    tmp += othreArgLen + titleLen;
    memset(tmp,0x00,_envMoveLen);
    printf("%s\n",_osArgv[0]);
    return true;
}

// 获取指定位置的命令行参数
char* ProTitle::GetArgv(const int idx) {
    if(idx < 0 || idx >= _osArgc) return nullptr;   // 不合法的数据

    return _osArgv[idx];
}

// 设置主进程以守护进程运行
bool ProTitle::SetDaemon() {
    int pid = fork();
    if(pid < 0){            // fork error
        ERR_EXIT("fork");
        return false;
    } else if(pid > 0)  {  // 父进程退出
        exit(0);
    }
    // pid > 0 子进程 
    // 子进程正常执行
    printf("pid %d",getpid());
    // 子进程担任进程组组长
    if(setsid() == -1) {    // 进程分离失败
        perror("setsid");
        return false;
    }

    umask(0);   // 重设文件权限掩码 mode & (~cmask)
    chdir("/"); // 改变当前目录为根目录

    // 关闭文件描述符
    int fd = open("/dev/null", O_RDWR); // 读写方式打开
    if(fd == -1) {
        perror("open");
        return false;
    }
	dup2(fd, STDIN_FILENO);     // 关闭写
	dup2(fd, STDOUT_FILENO);    // 关闭读
    dup2(fd,STDERR_FILENO);
    return true;
}
