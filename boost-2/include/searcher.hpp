#pragma once

#include "cppjieba/Jieba.hpp"
#include "config.hpp"

#include <stdint.h> // int16_t 、 uint32_t 、 int64_t
#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>


using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::vector;


namespace searcher {
    /*
     *  正排索引的存储结构体
     *  根据文档 id 定位到文档的内容 
     *  防止文档过多，直接使用64位的 int 来存储
    */
    struct frontIdx{
        int64_t _docId;
        string  _title;
        string  _url;
        string  _content;
    };

    /*
     *  倒排索引存储的结构体
     *  根据文本的关键字 定位到 所属的文档Id
     *  为了后面根据权值排序，再加一个关键字的权值
    */
    struct backwardIdx{
        int64_t _docId;
        int     _weight;
        string  _word;
    };

    class Index{
        public:
            Index();
            //查找正排索引
            const frontIdx* GetFrontIdx(const int64_t doc_id);
            //查倒排索引
            const vector<backwardIdx>* GetBackwardIdx(const string& key);
            // 建立倒排索引 与 正排索引
            bool Build(const string& input_path);
            // jieba分词 对语句进行分词
            void CutWord(const string& input,vector<string>* output);
        private:
            //根据一行 预处理 解析的文件，得到一个正排索引的节点
            frontIdx* BuildForward(const string& line);
            //根据正排索引节点，构造倒排索引节点
            void BuildInverted(const frontIdx& doc_info);
        private:
            //正排索引
            vector<frontIdx> forward_index;
            //倒排索引  哈希表
            unordered_map<string,vector<backwardIdx> > inverted_index;
            // jieba分词
            cppjieba::Jieba jieba;
    };

    // 搜索模块
    class Searcher {
        public:
            Searcher()
                : index(new Index())
            {}
            //  初始化，构建指定文档的索引
            bool Init(const string& input_path);
            //  指定文本进行搜索
            bool Search(const string& query,string* output);
        private:
            //得到关键字前后的数据，在前端页面显示的文本
            string GetShowContent(const string& content,const string& word);
        private:
            //需要索引进行搜索
            Index* index;
    };
}
