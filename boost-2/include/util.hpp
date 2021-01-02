#pragma once

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::vector;

namespace common{
    class Util{
        public:
            static bool Read(const string& input_path,string* output){
                std::ifstream file(input_path.c_str());
                if(file.is_open() == false){
                    cout<<input_path<<" file open error"<<endl;
                    return false;
                }

                string line;
                while(std::getline(file,line)){
                    *output += (line + "\n");
                }

                file.close();
                return true;
            }

            static void Split(const string& input,const string& delimiter,
                        vector<string>* output){
                boost::split(*output,input,boost::is_any_of(delimiter),
                            boost::token_compress_off);
            }
    };
}
