/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bag_of_words_extractor.h"

#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <regex>

#include "stmr.h"
#include "static_values.h"

namespace usermodel {

BagOfWords::~BagOfWords() = default;

BagOfWords::BagOfWords() :
    to_lower_(true) {
}

std::map<std::string, double> BagOfWords::GetFrequencies() {
  return frequencies_;
}

bool BagOfWords::Process(
    const std::string& html) {
  std::string data = html;

  if (data.length() > kMaximumHtmlLengthToClassify) {
    data = data.substr(0, kMaximumHtmlLengthToClassify - 1);
  }

  std::regex e1 = std::regex("[^\\w\\s]|_");
  std::regex e2 = std::regex("\\s+");
  std::string str = std::regex_replace(
    std::regex_replace(data, e1, ""), e2, " ");

  std::string buf;
  std::stringstream ss(str);
  std::vector<std::string> words;

  int word_count = 0;

  while (ss >> buf && word_count < kMaximumWordsToClassify) {
    if (buf.empty()) {
      continue;
    }

    if (to_lower_) {
      std::transform(buf.begin(), buf.end(), buf.begin(), ::tolower);
    }

    char* word = new char[buf.length() + 1];
    if (word == nullptr) {
      continue;
    }

    memcpy(word, buf.c_str(), buf.length() + 1);
    int end = stem(word, 0, strlen(word) - 1);
    if (end == -1) {
      continue;
    }

    word[end + 1] = 0;
    std::string word_str = std::string(word);
    words.push_back(word_str);

    word_count++;

    delete[] word;
    word = nullptr;
  }

  if (word_count < kMinimumWordsToClassify) {
    return false;
  }

  for (auto word : words) {
    frequencies_[word]++;
  }

  return true;
}

}  // namespace usermodel
