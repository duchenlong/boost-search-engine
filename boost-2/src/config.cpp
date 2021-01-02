#include "config.hpp"

size_t  _size = 0;
list<node*>  _info = list<node*> ();

// 读取配置文件中的信息
bool  Read(const string fileName = "my.conf") {
    cout << " loading " << fileName << endl;
    if(fileName.size() == 0) return false;

    ifstream fp;
    fp.open(fileName,std::ios::in);         // 只读
    if(fp.is_open() == false) return false; // 文件打开失败
    
    while(fp.eof() == false) {
        string buf("");
        getline(fp,buf);
        //std::cout<<buf<<std::endl;
        if(buf.size() == 0) continue;
        
        // 排除注释，[]标题，不合法的key(不是_，字母开头数据)
        if(buf[0] != '_' &&  !is_char(buf[0])) {
            continue;   // 继续下一行数据读取
        }

        // 截取key value， = 分割
        size_t p = buf.find("=");
        string key = buf.substr(0,p);
        string value = buf.substr(p+1);
        //std::cout<<key<<" "<<value<<std::endl;
        
        ltrim(key);
        rtrim(key);
        ltrim(value);
        rtrim(value);
        node* info = new node(key,value);
        _info.push_back(info);
        _size++;
    }
    
    cout << fileName << " load sucess" << endl;
    fp.close();
    return true;
}   

// 得到字符串类型的 value
const string  GetString(const string key) {
    for(auto& info : _info) {
        if(info->_key == key) return info->_value;
    }
    return "";
}

// 得到int 类型的数据，默认值为 del
int  GetInt(const string key,const int del) {
    for(auto& info : _info)
        if(info->_key == key) return std::atoi(info->_value.c_str());
    return del;
}

// 清除字符串左边空格
void  ltrim(string& str) {
    int p = 0;
    while(p < str.size() && (str[p] == 10 || str[p] == 13 || str[p] == 32)) p++;
    string tmp = str.substr(p);
    str = tmp;
}

// 清除字符串右边空格           
void  rtrim(string& str) {
    int p = str.size() - 1;
    while(p >= 0 && (str[p] == 10 || str[p] == 13 || str[p] == 32)) 
        str.erase(p--);
}

inline bool  is_char(char ch) {
    if(ch >= 'a' && ch <= 'z') return true;
    if(ch >= 'A' && ch <= 'Z') return true;
    return false;
}
