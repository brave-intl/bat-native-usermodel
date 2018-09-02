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

  // create model  
  usermodel::NaiveBayes mnb;
  mnb.loadModel(loadFile("model.json"));


  auto doc = loadJson("data/predictions.json");
  const Value& data = doc["data"]; // Using a reference for consecutive access is handy and faster.
  for (SizeType it = 0; it < data.Size(); it++) {
    std::string label = data[it]["label"].GetString();
    std::string filename = data[it]["doc"].GetString();

    auto doc = loadFile(std::string("data/") + std::string(filename));

    // load features
    usermodel::BagOfWords bow;
    bow.process(doc);

    auto res = mnb.predict(bow.getFrequencies());
    int i = 0;
    int argmax = 0;
    double max = 0.0;
    for (auto c: res) {
      if (c > max) {
        argmax = i;
        max = c;
      }
      i++;
      //std::cout << mnb.classes().at(i++) << " " << c << std::endl;
    }

    std::cout << filename << " - " << ((mnb.classes().at(argmax) == label) ? "PASS" : "FAIL") << std::endl;
    if (mnb.classes().at(argmax) != label) {
      std::cout << "\t" << mnb.classes().at(argmax) << " - " << label << std::endl;
      std::cout << "FEATURES" << std::endl;
      for (auto f: bow.getFrequencies()) {
        std::cout << "\t" << f.first << std::endl;
      }
    }
  }

  return 0;
}
