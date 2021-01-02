#include <iostream>
#include <string>
#include "httplib.h"
#include "searcher.hpp"
#include "config.hpp"
#include "parser.hpp"
#include "SetTitle.hpp"

// 默认监听端口
#define LISTENPORT  19998
#define FILENAME    "my.conf"

searcher::Searcher searchf;
bool ret = true;

void GetWebData(const httplib::Request& req,httplib::Response& resp){
    if(!req.has_param("query")){
        resp.set_content("请求参数错误","text/plain;charset=utf-8");
        return ;
    }
    
    string query = req.get_param_value("query");
    string results;
    searchf.Search(query,&results);
    resp.set_content(results,"application/json;charset=utf-8");
    
    cout<< "search len : "<< results.size()<<endl;
}

bool init(int argc,char* argv[]) {
    Read(FILENAME);             // 初始化配置文件
    parser_init();              // 初始化 parser.hpp 文件中的全局变量

    ProTitle pt(argc,argv);
    ret = pt.MoveOsEnv();       // 环境变量搬家
    if(ret == false) {
        cout << "move os environ error" << endl;
        return false;
    }
    
    cout << GetString("pro_name") << endl;
    ret = pt.SetProcTitle(GetString("pro_name").c_str());
    if(ret == false) {
        cout << "set title error" << endl;
        return false;
    }

    if(GetInt("isDaemon",0) == 1) {
        cout << "daemon ing" << endl;
        if(!pt.SetDaemon())
            cout << "daemon error" << endl;
    }

    return true;
}

int main(int argc,char* argv[]){
    init(argc, argv);             // 设置进程标题，与守护进程方式运行

    // 输入文件的地址 [解析后文本存放的位置 title\3url\3text]
    ret = searchf.Init( GetString("g_raw_input_path"));
    if(ret == false){
        cout<<"searcher init error"<<endl;
        return 1;
    }
    
    using namespace httplib;
    Server server;
    // 设置前端文件的根目录
    ret = server.set_base_dir( GetString("wwwroot").c_str());
    
    // 建立 回调函数 与 网址 映射
    server.Get("/searcher",GetWebData);
    
    // 建立监听
    ret = server.listen( GetString("listenIp").c_str(),GetInt("listenPort",19998));
    if(!ret) cout << " listen error " << endl;
    
    return 0;
}
