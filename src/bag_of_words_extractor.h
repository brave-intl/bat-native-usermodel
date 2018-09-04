
#ifndef SRC_BAG_OF_WORDS_EXTRACTOR_H_
#define SRC_BAG_OF_WORDS_EXTRACTOR_H_

#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <regex>
#include <iterator>
#include "stmr.h"

namespace usermodel {
class BagOfWords {
 public:
    BagOfWords();
    ~BagOfWords();
    bool Process(const std::string& data);
    std::map<std::string, double> GetFrequencies();

 private:
    std::vector<std::string> words_;
    std::map<std::string, double> frequencies_;
    bool to_lower_;
};
}  // namespace usermodel

#endif  // SRC_BAG_OF_WORDS_EXTRACTOR_H_
