// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "naive_bayes.h"
#include "deps/bat-native-rapidjson/include/rapidjson/document.h"
#include "deps/bat-native-rapidjson/include/rapidjson/writer.h"
#include "deps/bat-native-rapidjson/include/rapidjson/stringbuffer.h"

using namespace rapidjson;

namespace usermodel {
  NaiveBayes::NaiveBayes() {

  }

  NaiveBayes::~NaiveBayes() {

  }

  std::vector<std::string> NaiveBayes::classes() {
    return _classes;
  }

  std::vector<double> NaiveBayes::predict(std::map<std::string, double> features) {
    std::vector<double> results;

    // initialize with the priors
    for(size_t i=0; i<_classes.size(); i++) {
      results.push_back(_priors[i]);
    }

    for(auto feature : features) {
      if ( _features.find(feature.first) != _features.end() ) {
        int cl = 0;
        for(auto v: _features[feature.first]) {
          results.at(cl++) += v*features[feature.first];
          //std::cout << results.at(cl-1) << std::endl;
        }
      }
    }

    // logLikToProb
    auto max_val = *std::max_element(results.begin(), results.end());
    double sum = 0.0;
    for (size_t i=0; i<results.size(); i++) {
      results.at(i) = std::exp(results.at(i) - max_val);
      sum += results.at(i);
    }

    for (size_t i=0; i<results.size(); i++) {
      results.at(i) /= sum;
    }

    return results;
  }

  bool NaiveBayes::loadModel(const std::string& json) {
    Document d;
    d.Parse(json.c_str());

    const Value& classes = d["classes"]; // Using a reference for consecutive access is handy and faster.
    assert(classes.IsArray());
    for (SizeType i = 0; i < classes.Size(); i++) {
      _classes.push_back(classes[i].GetString());
    }

    const Value& priors = d["priors"]; // Using a reference for consecutive access is handy and faster.
    assert(priors.IsArray());
    for (SizeType i = 0; i < priors.Size(); i++) {
      _priors.push_back(priors[i].GetDouble());
    }

    const Value& features = d["logProbs"]; // Using a reference for consecutive access is handy and faster.
    for (Value::ConstMemberIterator itr = features.MemberBegin(); itr != features.MemberEnd(); ++itr) {
      std::vector<double> v;
      const Value& probs = features[itr->name.GetString()];
      for (SizeType i = 0; i < classes.Size(); i++) {
        v.push_back(probs[i].GetDouble());
      }
      _features[itr->name.GetString()] = v;
    }
    
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    //std::cout << buffer.GetString() << std::endl;
    return true;
  }

}