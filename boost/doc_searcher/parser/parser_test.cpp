#include "parser.hpp"

int main(){
    // 1. 得到html文件路径
    vector<string> file_list;
    bool ret = GetFilePath(g_input_path,&file_list);
    if(ret == false){
        cout<<"get html file path error"<<endl;
        return 1;
    }

    // for(auto& str : file_list){
    //     cout<<str<<endl;
    // }
    // cout<<file_list.size()<<endl;

    // 2. 遍历枚举的路径，针对每个文件进行单独处理
    std::ofstream output_file(g_output_path.c_str());
    if(output_file.is_open() == false){
        cout<<g_output_path<<" file open error"<<endl;
        return 1;
    }
    for(const auto& file_path : file_list){
        DocInfo doc_info;
        ret = ParseFile(file_path,&doc_info);
        if(ret == false){
            cout<<file_path<<" file analysis error"<<endl;
            continue;
        }
        //cout<<doc_info._title<<' '<<doc_info._url<<endl;
        // 3. 解析的文件写入到 指定的输出文件中
        WriteOutput(doc_info,output_file);
    }

    output_file.close();
    return 0;
}