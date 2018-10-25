/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "../include/linear_regression.h"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace usermodel {
  LinearRegression::LinearRegression() {
  }

  LinearRegression::~LinearRegression() {
  }

  std::vector<double> LinearRegression::Predict(std::map<std::string, double> features) {
    std::vector<double> results;

    double sum = bias_;
    for (auto feature : features) {
      if ( weights_.find(feature.first) != weights_.end() ) {
        sum += weights_[feature.first]*features[feature.first];
      }
    }

    results.push_back(sum);
    return results;
  }

  bool LinearRegression::LoadModel(const std::string& json) {
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
