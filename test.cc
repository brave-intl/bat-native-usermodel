// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <regex>
#include <iterator>

#include "bag_of_words_extractor.h"
#include "naive_bayes.h"
#include "logistic_regression.h"
#include "user_model.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <fstream>
#include <streambuf>

#include "../include/user_profile.h"

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

using namespace rapidjson;
using namespace usermodel;

std::string loadFile(std::string filename) {
  std::ifstream t(filename);
  std::string data((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

  return data;  
}

Document loadJson(std::string filename) {
  auto json = loadFile(filename);

  Document d;
  d.Parse(json.c_str());

  return d;
}

TEST_CASE( "Test user profiles", "[user_profile]" ) {
  NaiveBayes model;
  model.LoadModel(loadFile("model.json"));

  auto user = UserProfile();
  for ( auto c : model.Classes() ) {
    user.long_term_interests_[c] = 0.0;
    user.short_term_interests_[c] = 0.0;
    user.search_intent_[c] = 0.0;
    user.shopping_intent_[c] = 0.0;
  }
  REQUIRE( user.long_term_interests_.size() != 0 );
  
  auto json = user.ToJSON();
  REQUIRE( json != "" );
  //std::cout << json << std::endl;

  auto profile = UserProfile::FromJSON(json);

  REQUIRE( profile->long_term_interests_.size() != 0 );
  REQUIRE( profile->short_term_interests_.size() != 0 );
  REQUIRE( profile->search_intent_.size() != 0 );
  REQUIRE( profile->shopping_intent_.size() != 0 );
}

TEST_CASE( "Test logistic regression", "[classifier]" ) {
  usermodel::LogisticRegression cl;
  cl.LoadModel("{\"features\":[\"test1\", \"test2\", \"test3\"], \"weights\":[0.2, 0.4, 0.9], \"bias\": 0.5}");
  auto features = std::map<std::string, double>();
  features["test1"] = 0.1;
  features["test2"] = 0.3;
  REQUIRE(cl.Predict(features).at(0) ==  Approx( 0.65475 ));
}

TEST_CASE( "Test naive bayes", "[classifier]" ) {
  usermodel::UserModel um;
  um.initializePageClassifier(loadFile("model.json"));

  auto doc = loadJson("data/predictions.json");
  const Value& data = doc["data"]; // Using a reference for consecutive access is handy and faster.
  for (SizeType it = 0; it < data.Size(); it++) {
    std::string label = data[it]["label"].GetString();
    std::string filename = data[it]["doc"].GetString();

    auto doc = loadFile(std::string("data/") + std::string(filename));
    auto scores = um.classifyPage(doc);
    auto predicted = um.winningCategory(scores);
    REQUIRE( predicted == label );
  }
}