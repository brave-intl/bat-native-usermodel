// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "logistic_regression.h"
#include "deps/bat-native-rapidjson/include/rapidjson/document.h"
#include "deps/bat-native-rapidjson/include/rapidjson/writer.h"
#include "deps/bat-native-rapidjson/include/rapidjson/stringbuffer.h"

namespace usermodel {
  LogisticRegression::LogisticRegression() {
  }

  LogisticRegression::~LogisticRegression() {
  }

  std::vector<double> LogisticRegression::Predict(std::map<std::string, double> features) {
    std::vector<double> results;

    double sum = bias_;
    for (auto feature : features) {
      if ( weights_.find(feature.first) != weights_.end() ) {
        sum += weights_[feature.first]*features[feature.first];
      }
    }

    results.push_back(1.0 / (1.0 + std::exp(-sum)));
    return results;
  }

  bool LogisticRegression::LoadModel(const std::string& json) {
    rapidjson::Document d;
    d.Parse(json.c_str());

    const rapidjson::Value& features = d["features"];  // Using a reference for consecutive access is handy and faster.
    assert(features.IsArray());
    for (rapidjson::SizeType i = 0; i < features.Size(); i++) {
      features_.push_back(features[i].GetString());
    }

    const rapidjson::Value& weights = d["weights"];  // Using a reference for consecutive access is handy and faster.
    assert(weights.IsArray());
    for (rapidjson::SizeType i = 0; i < weights.Size(); i++) {
      weights_[features_.at(i)] = weights[i].GetDouble();
    }

    const rapidjson::Value& bias = d["bias"];  // Using a reference for consecutive access is handy and faster.
    bias_ = bias.GetDouble();

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    return true;
  }
}  // namespace usermodel
