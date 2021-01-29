# boost search engine
> 博客地址：[https://blog.csdn.net/duchenlong/article/details/108189922](https://blog.csdn.net/duchenlong/article/details/108189922)

一个基于boost 在线文档所实现的一个简单的搜索引擎，随后加入到博客系统的搜索框架中

## boost 2.0

在boost 2.0 中，将之前的一些细节进行完善，修改了项目的目录格式，加入了配置文件模块和设置进程名称模块。可以将一些相对的路径，和一个公共的参数以`key=value`的格式放置在`my.conf`中，然后调用函数进行读取

使用的时候，先将1.0 中的 data 文件拷贝到boost-2 中，因为data 文件里含有 已经下载好的boost在线文档（比较大）

> 运行 boost-2 目录下 （后台运行）

`make`

`./main > tmp.sh &`


