#include "cppjieba/Jieba.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

using namespace std;

const char* const DICT_PATH = "../cppjieba-master/dict/jieba.dict.utf8";
const char* const HMM_PATH = "../cppjieba-master/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../cppjieba-master/dict/user.dict.utf8";
const char* const IDF_PATH = "../cppjieba-master/dict/idf.utf8";
const char* const STOP_WORD_PATH = "../cppjieba-master/dict/stop_words.utf8";

int main(int argc, char** argv) {
  cppjieba::Jieba jieba(DICT_PATH,
        HMM_PATH,
        USER_DICT_PATH,
        IDF_PATH,
        STOP_WORD_PATH);
  vector<string> words;
  vector<cppjieba::Word> jiebawords;
  string s;
  string result;

  s = "我来到北京清华大学";
  cout << s << endl;
  cout << "[demo] CutAll" << endl;
  jieba.CutAll(s, words);
  cout << limonp::Join(words.begin(), words.end(), "/") << endl;

  s = "小明硕士毕业于中国科学院计算所，后在日本京都大学深造";
  cout << s << endl;
  cout << "[demo] CutForSearch" << endl;
  jieba.CutForSearch(s, words);
  cout << limonp::Join(words.begin(), words.end(), "/") << endl;

  return EXIT_SUCCESS;
}
