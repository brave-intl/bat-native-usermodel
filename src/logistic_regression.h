// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <vector>
#include "bag_of_words_extractor.h"
#include "deps/bat-native-rapidjson/include/rapidjson/document.h"
#include "deps/bat-native-rapidjson/include/rapidjson/writer.h"
#include "deps/bat-native-rapidjson/include/rapidjson/stringbuffer.h"

#ifndef USERMODEL_LOGISTIC_REGRESSION_H
#define USERMODEL_LOGISTIC_REGRESSION_H

namespace usermodel {
  class LogisticRegression {

    public:
    LogisticRegression();
    ~LogisticRegression();
    bool loadModel(const std::string& model);
    std::vector<double> predict(std::map<std::string, double> features);

    private:
    std::vector<std::string> _features;
    std::map<std::string, double> _weights;
    double _bias;
  };
}

#endif