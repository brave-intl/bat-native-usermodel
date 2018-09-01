
#ifndef USERMODEL_BAG_OF_WORDS_H
#define USERMODEL_BAG_OF_WORDS_H

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
    bool process(const std::string& data);
    std::map<std::string, long> getFrequencies();
    
    private:
    std::vector<std::string> words;
    std::map<std::string, long> frequencies;
};

}

#endif  // USERMODEL_BAG_OF_WORDS_H
