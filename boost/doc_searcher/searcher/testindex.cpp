#include "searcher.hpp"
#include <ostream>

int main() {
    searcher::Index index;
    bool ret = index.Build("../data/tmp/raw_input.txt");
    if (!ret) {
        std::cout << "file error, create index error " << std::endl;
        return 1;
    }

    // 索引构建成功, 就调用索引中的相关函数. (查正排+查倒排)
    auto* inverted_list = index.GetBackwardIdx("filesystem");
    for (const auto& weight : *inverted_list) {
        std::cout << "doc_id:" << weight._docId << "weight:" << weight._weight << std::endl;
        auto* doc_info = index.GetFrontIdx(weight._docId);
        std::cout << "title:" << doc_info->_title << std::endl;
        std::cout << "url:" << doc_info->_url << std::endl;
        std::cout << "content:" << doc_info->_content << std::endl;
        std::cout << "================================================================" << std::endl;
    }
    return 0;
}