#include "parser.hpp"

using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::vector;

//boost 中 .html文件的路径
string g_input_path = "../data/input/";

//解析后的每个html文件的内容，所存放的路径
string g_output_path = "../data/tmp/raw_input.txt";

// 在线文档的路径 公共前缀
string g_url_head = "https://www.boost.org/doc/libs/1_53_0/doc/";


bool GetFilePath(const string& input_path,vector<string>* file_list){
    namespace fs = boost::filesystem;
    fs::path root_path(input_path);
    if(fs::exists(root_path) == false){
        cout<<input_path<<" not exists"<<endl;
        return false;
    }

    fs::recursive_directory_iterator end_iter;
    for(fs::recursive_directory_iterator iter(root_path); 
        iter != end_iter; iter++){
            //当前路径为目录时，直接跳过
            if(fs::is_regular_file(*iter) == false){
                continue;
            }
            
            //当前文件不是 .html 文件，直接跳过
            if(iter->path().extension() != ".html"){
                continue;
            }

            //得到的路径加入到 vector 数组中
            file_list->push_back(iter->path().string());
        }
    
    return true;
}

//找到标题  <title> </title>
bool ParseTitle(const string& html,string* title){
    size_t begin = html.find("<title>");
    if(begin == string::npos){
        cout<<"title not find"<<endl;
        return false;
    }

    size_t end = html.find("</title>",begin);
    if(end == string::npos){
        cout<<"title not find"<<endl;
        return false;
    }

    begin += string("<title>").size();
    if(begin >= end){
        cout<<"title pos info error"<<endl;
        return false;
    }

    *title = html.substr(begin,end - begin);
    return true;
}

// 本地路径形如:
// ../data/input/html/thread.html
// 在线路径形如:
// https://www.boost.org/doc/libs/1_53_0/doc/html/thread.html
bool ParseUrl(const string& file_path,string* url){
    string url_tail = file_path.substr(g_input_path.size());
    *url = g_url_head + url_tail;

    return true;
}

bool ParseContent(const string& html,string* content){
    bool is_content = true;
    for(auto c : html){
        if(is_content == true){
            if(c == '<'){
                //之后对<>中的内容进行忽略处理
                is_content = false;
            }
            else{
                if(c == '\n'){
                    c = ' ';
                }
                content->push_back(c);
            }
        }
        else{
            if(c == '>'){
                is_content = true;
            }
            //忽略标签中的内容 <a> 
        }
    }
    return true;
}

bool ParseFile(const string& file_path,DocInfo* doc_info){
    string html;
    bool ret = common::Util::Read(file_path,&html);
    if(ret == false){
        cout<<file_path<< " file read error"<<endl;
        return false;
    }

    ret = ParseTitle(html,&doc_info->_title);
    if(ret == false){
        cout<<"title analysis error "<<endl;
        return false;
    }

    ret = ParseUrl(file_path,&doc_info->_url);
    if(ret == false){
        cout<<"Url analysis error "<<endl;
        return false;
    }

    ret = ParseContent(html,&doc_info->_content);
    if(ret == false){
        cout<<"content analysis error "<<endl;
        return false;
    }
    return true;
}

void WriteOutput(const DocInfo& doc_info,std::ofstream& ofstream){
    ofstream<<doc_info._title<<"\3"<<doc_info._url
            <<"\3"<<doc_info._content<<endl;
}

