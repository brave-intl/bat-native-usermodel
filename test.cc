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

#include "deps/bat-native-rapidjson/include/rapidjson/document.h"
#include "deps/bat-native-rapidjson/include/rapidjson/writer.h"
#include "deps/bat-native-rapidjson/include/rapidjson/stringbuffer.h"

#include <fstream>
#include <streambuf>

using namespace rapidjson;

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

int main(int argc, char* argv[]) {
  printf("Testing bat-native-usermodel...\n");

  usermodel::LogisticRegression cl;
  cl.loadModel("{\"features\":[\"test1\", \"test2\", \"test3\"], \"weights\":[0.2, 0.4, 0.9], \"bias\": 0.5}");
  auto features = std::map<std::string, double>();
  features["test1"] = 0.1;
  features["test2"] = 0.3;
  std::cout << "results = " << cl.predict(features).at(0) << std::endl;

  // test naive bayes
  std::cout << "TESTING NAIVE BAYES" << std::endl;

  // create model  
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

    std::cout << filename << " - " << ((predicted == label) ? "PASS" : "FAIL") << std::endl;
    if (predicted != label) {
      std::cout << "\t" << predicted << " - " << label << std::endl;
    }
  }

  return 0;
}
