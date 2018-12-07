/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "naive_bayes.h"

#include <algorithm>

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace usermodel {

NaiveBayes::NaiveBayes() = default;
NaiveBayes::~NaiveBayes() = default;

bool NaiveBayes::LoadModel(const std::string& json) {
  if (json.empty()) {
    return false;
  }

  rapidjson::Document document;
  document.Parse(json.c_str());

  if (document.HasParseError()) {
    return false;
  }

  const rapidjson::Value& classes = document["classes"];
  if (!classes.IsArray()) {
    return false;
  }

  for (rapidjson::SizeType i = 0; i < classes.Size(); i++) {
    classes_.push_back(classes[i].GetString());
  }

  const rapidjson::Value& priors = document["priors"];
  if (!priors.IsArray()) {
    return false;
  }

  for (rapidjson::SizeType i = 0; i < priors.Size(); i++) {
    priors_.push_back(priors[i].GetDouble());
  }

  const rapidjson::Value& features = document["logProbs"];
  if (!features.IsObject()) {
    return false;
  }

  for (rapidjson::Value::ConstMemberIterator it = features.MemberBegin();
      it != features.MemberEnd(); ++it) {
    std::vector<double> v;
    const rapidjson::Value& probs = features[it->name.GetString()];
    for (rapidjson::SizeType i = 0; i < classes.Size(); i++) {
      v.push_back(probs[i].GetDouble());
    }
    features_[it->name.GetString()] = v;
  }

  return true;
}

std::vector<std::string> NaiveBayes::Classes() {
  return classes_;
}

std::vector<double> NaiveBayes::Predict(
    std::map<std::string, double> features) {
  std::vector<double> results;

  // Initialize with the priors
  for (size_t i = 0; i < classes_.size(); i++) {
    results.push_back(priors_[i]);
  }

  for (auto feature : features) {
    if (features_.find(feature.first) != features_.end()) {
      int cl = 0;
      for (auto v : features_[feature.first]) {
        results.at(cl++) += v*features[feature.first];
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

}  // namespace usermodel
