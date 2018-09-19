/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "naive_bayes.h"

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace usermodel {
  NaiveBayes::NaiveBayes() {
  }

  NaiveBayes::~NaiveBayes() {
  }

  std::vector<std::string> NaiveBayes::Classes() {
    return classes_;
  }

  std::vector<double> NaiveBayes::Predict(std::map<std::string, double> features) {
    std::vector<double> results;

    // initialize with the priors
    for (size_t i = 0; i < classes_.size(); i++) {
      results.push_back(priors_[i]);
    }

    for (auto feature : features) {
      if ( features_.find(feature.first) != features_.end() ) {
        int cl = 0;
        for (auto v : features_[feature.first]) {
          results.at(cl++) += v*features[feature.first];
          // std::cout << results.at(cl-1) << std::endl;
        }
      }
    }

    // logLikToProb
    auto max_val = *std::max_element(results.begin(), results.end());
    double sum = 0.0;
    for (size_t i = 0; i < results.size(); i++) {
      results.at(i) = std::exp(results.at(i) - max_val);
      sum += results.at(i);
    }

    for (size_t i = 0; i < results.size(); i++) {
      results.at(i) /= sum;
    }

    return results;
  }

  std::string NaiveBayes::WinningCategory(std::vector<double> scores) {
    int i = 0;
    int argmax = 0;
    double max = 0.0;
    for (auto c : scores) {
      if (c > max) {
        argmax = i;
        max = c;
      }
      i++;
    }
    return classes_.at(argmax);
  }

  bool NaiveBayes::LoadModel(const std::string& json) {
    rapidjson::Document d;
    d.Parse(json.c_str());

    const rapidjson::Value& classes = d["classes"];  // Using a reference for consecutive access is handy and faster.
    assert(classes.IsArray());
    for (rapidjson::SizeType i = 0; i < classes.Size(); i++) {
      classes_.push_back(classes[i].GetString());
    }

    const rapidjson::Value& priors = d["priors"];  // Using a reference for consecutive access is handy and faster.
    assert(priors.IsArray());
    for (rapidjson::SizeType i = 0; i < priors.Size(); i++) {
      priors_.push_back(priors[i].GetDouble());
    }

    const rapidjson::Value& features = d["logProbs"];  // Using a reference for consecutive access is handy and faster.
    for (rapidjson::Value::ConstMemberIterator itr = features.MemberBegin(); itr != features.MemberEnd(); ++itr) {
      std::vector<double> v;
      const rapidjson::Value& probs = features[itr->name.GetString()];
      for (rapidjson::SizeType i = 0; i < classes.Size(); i++) {
        v.push_back(probs[i].GetDouble());
      }
      features_[itr->name.GetString()] = v;
    }

    return true;
  }
}  // namespace usermodel
