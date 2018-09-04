/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bag_of_words_extractor.h"

namespace usermodel {
BagOfWords::~BagOfWords() {
}

BagOfWords::BagOfWords() : to_lower_(true) {
}

std::map<std::string, double> BagOfWords::GetFrequencies() {
    return frequencies_;
}

bool BagOfWords::Process(const std::string& data) {
    std::regex e1 = std::regex("[^\\w\\s]|_");
    std::regex e2 = std::regex("\\s+");
    std::string str = std::regex_replace(
            std::regex_replace(data, e1, ""),
            e2,
            " ");

    std::string buf;                 // Have a buffer string
    std::stringstream ss(str);       // Insert the string into a stream
    std::vector<std::string> words;  // Create vector to hold our words
    while (ss >> buf) {
        if (to_lower_) {
            std::transform(buf.begin(), buf.end(), buf.begin(), ::tolower);
        }

        char* word = new char[buf.length()+1];
        memcpy(word, buf.c_str(), buf.length()+1);
        int end = stem(word, 0, strlen(word) - 1);
        word[end + 1] = 0;
        std::string word_str = std::string(word);
        words.push_back(word_str);
    }

    for (auto word : words) {
        frequencies_[word]++;
    }

    return true;
}
}  // namespace usermodel
