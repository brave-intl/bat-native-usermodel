/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

# include <iostream>
# include <map>
# include <vector>
# include <sstream>
# include <regex>

#include "bigram_extractor.h"
#include "static_values.h"

namespace usermodel {

BiGram::~BiGram() = default;

BiGram::BiGram() :
    token_frequencies({}) {
}

std::map<std::string, double> BiGram::GetFrequencies() {
  return token_frequencies;
}

bool BiGram::Process(const std::string& html) {
  // TODO(Moritz Haller): Remove overwrite
  std::string data = html;

  if (data.length() > kMaximumHtmlLengthToClassify) {
    data = data.substr(0, kMaximumHtmlLengthToClassify - 1);
  }

  // Clean data: filter out tags and to lower
  std::regex e1 = std::regex("[^\\w\\s]|_");
  std::regex e2 = std::regex("\\s+");
  std::string rr_data = std::regex_replace(std::regex_replace(data, e1, ""), e2, " ");

  std::for_each(rr_data.begin(), rr_data.end(), [](char & c) {
    c = ::tolower(c);
  });

  // Read words from stream
  // TODO(Moritz Haller): While reading stream match against key-word lists to
  // avoid reading in unused tokens
  std::stringstream sstream(rr_data);

  // Create unigrams
  std::vector<std::string> ngrams;
  std::string word;
  auto word_count = 0;
  while (sstream >> word && word_count < kMaximumWordsToClassify) {
    ngrams.push_back(word);
    word_count++;
  }

  // Create bigrams
  std::vector<std::string> bigrams;
  std::string prev_token = "";

  for (auto token : ngrams) {
    // Skip first unigram
    if (prev_token != "") {
        bigrams.push_back(prev_token + " " + token);
    }
    prev_token = token;
  }

  // Create combined ngrams vector
  ngrams.insert(ngrams.end(), bigrams.begin(), bigrams.end());

  // Count token frequencies
  for (auto token : ngrams) {
    token_frequencies[token]++;
  }

  return true;
}

}  // namespace usermodel
