#pragma once

#include <fstream>
#include <list>
#include <iostream>

using namespace std;

#define  FILENAME "my.conf"


struct node {
    string _key;
    string _value;
    node(const string& key,const string& value)
        :_key(key),_value(value)
        {}
};

extern  list<node*>      _info;
extern  size_t           _size;
    
const string GetString(const string key);        // 得到字符串类型的 value
int GetInt(const string key,const int del);      // 得到int 类型的数据，默认值为 del
bool Read(const string fileName);           // 读取配置文件中的信息
void ltrim(string& str);                    // 清除字符串左边空格
void rtrim(string& str);                    // 清除字符串右边空格
inline bool is_char(char ch);               // 判断是否为字母
