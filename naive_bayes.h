// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <vector>
#include "bag_of_words_extractor.h"
#include "deps/bat-native-rapidjson/include/rapidjson/document.h"
#include "deps/bat-native-rapidjson/include/rapidjson/writer.h"
#include "deps/bat-native-rapidjson/include/rapidjson/stringbuffer.h"

namespace usermodel {

  class NaiveBayes {

    public:
    NaiveBayes();
    ~NaiveBayes();
    bool loadModel(const std::string& model);
    std::vector<double> predict(std::map<std::string, long> features);
    std::vector<std::string> classes();

    private:
    std::vector<std::string> _classes;
    std::vector<double> _priors;
    std::map<std::string, std::vector<double>> _features;
  };
  
}