#include "searcher.hpp"
#include "util.hpp"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/json.h>
#include <boost/algorithm/string/case_conv.hpp>
#include <fstream>
#include <string>
#include <algorithm>


namespace searcher {
    //  索引模块

    //jieba分词词典的路径
    const char* const DICT_PATH = "./jieba_dict/jieba.dict.utf8";
    const char* const HMM_PATH = "./jieba_dict/hmm_model.utf8";
    const char* const USER_DICT_PATH = "./jieba_dict/user.dict.utf8";
    const char* const IDF_PATH = "./jieba_dict/idf.utf8";
    const char* const STOP_WORD_PATH = "./jieba_dict/stop_words.utf8";

    Index::Index()
        :jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH)
    {
        forward_index.clear();
        inverted_index.clear();
    }

    //查找正排索引
    const frontIdx* Index::GetFrontIdx(const int64_t doc_id){
        if(doc_id < 0 || doc_id >= forward_index.size()){
            return nullptr;
        }
        return &forward_index[doc_id];
    }

    //查倒排索引
    const vector<backwardIdx>* Index::GetBackwardIdx(const string& key){
        auto it = inverted_index.find(key);
        if(it == inverted_index.end()){
            return nullptr;
        }

        return &(it->second);
    }

    // 建立索引
    bool Index::Build(const string& input_path){
        //  按行读取 存放预处理中解析出来的数据的文件 
        cout<<input_path<<" build index begin "<<endl;
        std::ifstream file(input_path.c_str());
        if(file.is_open() == false){
            cout<<input_path<< " file open error " <<endl;
            return false;
        }

        string line;
        int idx = 0;
        static string progess("|/-\\");

        while(std::getline(file,line)){
            //  针对当前行数据，进行正排索引
            frontIdx* doc_info = BuildForward(line);
            if(doc_info == nullptr){
                cout<< " forward build error "<<endl;
                continue;
            }

            //根据正排索引的节点，构建倒排索引
            BuildInverted(*doc_info);

            // 打印部分构建结果 防止过多cout影响时间复杂度
            if(doc_info->_docId % 100 == 0){
                //cout<< doc_info->_docId << " sucessed "<<endl;
                cout<<"\r"<<progess[idx % 4]<< doc_info->_docId << " sucessed " <<std::flush;
                idx++;
            }
        }
        cout << endl << input_path << " index build sucessed "<<endl;
        file.close();
        return true;
    }

    // jieba分词 对语句进行分词
    void Index::CutWord(const string& input,vector<string>* output){
        jieba.CutForSearch(input,*output);
        if(input.size() < 100) output->push_back(input);
    }

    //根据一行 预处理 解析的文件，得到一个正排索引的节点,并插入到正排数组中
    frontIdx* Index::BuildForward(const string& line){
        // 对一行数据进行拆分 \3 为分割点，依次为 title url content
        vector<string> nums;
        common::Util::Split(line,"\3",&nums);
        if(nums.size() != 3){
            cout<<" file num error "<< nums.size()<<endl;
            return nullptr;
        }
        frontIdx doc_info;
        doc_info._docId     = forward_index.size();
        doc_info._title     = nums[0];
        doc_info._url       = nums[1];
        doc_info._content   = nums[2];
        forward_index.push_back(std::move(doc_info)); 

        return &forward_index.back();
    }

    //根据正排索引节点，构造倒排索引节点
    void Index::BuildInverted(const frontIdx& doc_info){
        //统计关键字作为 标题 和正文的出现次数
        struct WordCnt {
            int _titleCnt;
            int _contentCnt;
            WordCnt()
                :_titleCnt(0),_contentCnt(0)
                {} 
        };

        unordered_map<string,WordCnt> wordMap;

        //针对标题进行分词
        vector<string> titleWord;
        CutWord(doc_info._title,&titleWord);
        for(string word : titleWord){
            // 排除单独符号的影响
            if(word.size() == 0 || (!(word[0] >=  'a' && word[0] <= 'z')\
                    &&  !(word[0] <= 'Z' && word[0] >= 'A') ) )
                continue;
            
            //全部转为小写
            boost::to_lower(word);
            wordMap[word]._titleCnt++;
        }

        //针对正文进行分词
        vector<string> contentWord;
        CutWord(doc_info._content,&contentWord);
        for(string word : contentWord){
            if(word.size() == 0 || !(word[0] >=  'a' && word[0] <= 'z')\
                    &&  !(word[0] <= 'Z' && word[0] >= 'A'))
                continue;
            boost::to_lower(word);
            wordMap[word]._contentCnt++;
        }

        //统计结果，插入到倒排索引中
        for(const auto& word_pair : wordMap){
            backwardIdx backIdx;
            backIdx._docId  = doc_info._docId;

            //自定义 权值 = 10 * titleCnt + contentCnt
            backIdx._weight = 100 * word_pair.second._titleCnt + word_pair.second._contentCnt;
            backIdx._word   = word_pair.first;

            vector<backwardIdx>& back_vector = inverted_index[word_pair.first];
            back_vector.push_back(std::move(backIdx));
        }
    }


    //  搜索模块
    const int lengthText = 160;
    //  初始化，构建指定文档的索引
    bool Searcher::Init(const string& input_path){
        return index->Build(input_path);
    }

    //  指定文本进行搜索
    bool Searcher::Search(const string& query,string* output){
        //  分词
        vector<string> words;
        index->CutWord(query,&words);

        //  触发，根据分词的结果，进行倒排索引，得到相关文档
        vector<backwardIdx> wordsResult;
        for(string word : words){
            if(word.size() == 0 || !(word[0] >=  'a' && word[0] <= 'z')\
                &&  !(word[0] <= 'Z' && word[0] >= 'A'))
                continue;
            boost::to_lower(word);
            //const vector<backwardIdx>*
            auto* backList = index->GetBackwardIdx(word);
            if(backList == nullptr){
                //  没有这个关键词
                continue;
            }
            //插入多个数据
            wordsResult.insert(wordsResult.end(),backList->begin(),backList->end());
        }

        //  排序
        std::sort(wordsResult.begin(),wordsResult.end(),
                [](const backwardIdx& le,const backwardIdx& ri){
                    return le._weight > ri._weight;
                });

        //  包装
        Json::Value value;
        int cnt = 0;
        vector<bool> st(7000,false); 
        for(const auto& backidx : wordsResult){
            //cout << cnt << "-";
            if(cnt > 100) break;
            //  根据 id 查找正排索引
            const frontIdx* doc_info = index->GetFrontIdx(backidx._docId);
            if(st[doc_info->_docId]) continue;
            cnt ++;
            st[doc_info->_docId] = true;

            Json::Value tmp;
            tmp["title"]    = doc_info->_title;
            tmp["url"]      = doc_info->_url;
            tmp["desc"]     = GetShowContent(doc_info->_content,backidx._word);
            value.append(tmp); 
        }
        Json::FastWriter writer;
        *output = writer.write(value);
        return true;
    }
    //得到关键字前后的数据，在前端页面显示的文本
    string Searcher::GetShowContent(const string& content,const string& word){
        size_t idx = content.find(word);
        string ans("");
        int pos = 0;    //  显示文本开始的位置
        int len = lengthText;   //  截取显示文本的长度
        
        if(idx != string::npos){
            pos = std::max(0,(int)((int)idx-lengthText/2));
        }
        //关键字不存在,说明关键字只存在于标题中
        ans = content.substr(pos,len);
        ans += "...";
        return ans;
    }
}
