#include <iostream>
#include <string>
#include "httplib.h"
#include "../searcher/searcher.hpp"

using std::cout;
using std::endl;
using std::string;

const string g_input_path = "../data/tmp/raw_input.txt";

const string g_root_path = "./www";

searcher::Searcher search;

void GetWebData(const httplib::Request& req,httplib::Response& resp){
    if(!req.has_param("query")){
        resp.set_content("请求参数错误","text/plain;charset=utf-8");
        return ;
    }
    
    string query = req.get_param_value("query");
    string results;
    search.Search(query,&results);
    resp.set_content(results,"application/json;charset=utf-8");
    cout<<results<<endl;
}

int main(){
    using namespace httplib;
    bool ret = search.Init(g_input_path);
    if(ret == false){
        cout<<"searcher init error"<<endl;
        return 1;
    }

    Server server;
    server.set_base_dir(g_root_path.c_str());

    server.Get("/searcher",GetWebData);
    server.listen("0.0.0.0",19998);
    return 0;
}