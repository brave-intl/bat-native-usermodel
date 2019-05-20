/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/vendor/bat-native-usermodel/src/pipeline.h"
#include "brave/vendor/bat-native-usermodel/src/transformation.h"
#include "brave/vendor/bat-native-usermodel/src/data_point.h"
#include <iostream>
#include <cmath>
#include <vector>
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "base/base_paths.h"
#include "base/files/file_util.h"
#include "base/logging.h"
#include "base/path_service.h"
#include "base/stl_util.h"
#include <fstream>

const base::FilePath::CharType kTestDataRelativePath[] =
  FILE_PATH_LITERAL("brave/vendor/bat-native-usermodel/test/data");


namespace usermodel {

class Pipeline_test : public ::testing::Test {
 protected:
  void SetUp() override {
  }
  void TearDown() override {}

};

std::string LoadFile(const std::string& filename) {
    base::FilePath path(kTestDataRelativePath);
    path = path.AppendASCII(filename);

    std::ifstream ifs(path.value());
    if (ifs.fail()) {
      return "";
    }

    std::stringstream stream;
    stream << ifs.rdbuf();
    std::string value = stream.str();

    return value;
}

TEST_F(Pipeline_test, Build_simple_pipeline) {
    std::string test_string = "Test String";
    auto string_data_point = Data_point(test_string);
    std::vector<Transformation> transformations;
    usermodel::To_lower to_lower;
    transformations.push_back(to_lower);
    // usermodel::Hashed_ngrams hashed_ngrams;
    auto hashed_ngrams = usermodel::Hashed_ngrams(3, std::vector<int>{1, 2, 3} );
    transformations.push_back(hashed_ngrams);
    std::map<std::string,Data_point> weights = {
        {"class_1", Data_point(std::vector<float>{1.0, 2.0, 3.0}) },
        {"class_2", Data_point(std::vector<float>{3.0, 2.0, 1.0}) }, 
        {"class_3", Data_point(std::vector<float>{2.0, 2.0, 2.0}) }
    };
    std::map<std::string,float> biases = {
        {"class_1",0.0},
        {"class_2", 0.0},
        {"class_3", 0.0}
    };
    auto class0 = Data_point(std::vector<float>{1.0, 0.0, 0.0});
    unsigned int expected_len = 3;
    Linear_classifier linear_classifier(weights,biases);
    //auto linear_classifier = Linear_classifier(weights, biases);
    auto rez0 = linear_classifier.Predict(class0);
    EXPECT_EQ(expected_len, rez0.size() );
    // auto pipeline = Pipeline(transformations, linear_classifier);
    Pipeline pipeline;
    pipeline = Pipeline(transformations, linear_classifier);
    auto rez = pipeline.Apply(string_data_point);
    
    EXPECT_EQ(expected_len, rez.size());
}

TEST_F(Pipeline_test, Test_Load_From_Json){
  auto test_json = LoadFile("pipeline.json");
  usermodel::Pipeline pipeline; 
  auto load_success = pipeline.FromJson(test_json);
  EXPECT_TRUE(load_success);
  std::vector<std::string> train_texts = {
    "This is a spam email.", 
    "Another spam trying to sell you viagra",
    "Message from mom with no real subject", 
    "Another messase from mom with no real subject",
    "Yadayada"
  };
  std::vector<std::string> train_labels = {"spam", "spam", "ham", "ham", "junk"};
  //let's see if results match as well: 
  for (unsigned long i = 0 ; i < train_texts.size();i++){
    auto preds = pipeline.Apply(Data_point(train_texts[i]));
    for (auto const& pred : preds){
      auto other_predictions = pred.second;
      EXPECT_TRUE(preds[train_labels[i]] >=other_predictions );
    }
  }
}

TEST_F(Pipeline_test, Test_GetAdvertisingPredictions) {
    std::string test_string = "Test String";
    std::vector<Transformation> transformations;
    usermodel::To_lower to_lower;
    transformations.push_back(to_lower);
    auto hashed_ngrams = usermodel::Hashed_ngrams(3, std::vector<int>{1, 2, 3} );
    transformations.push_back(hashed_ngrams);
    std::map<std::string,Data_point> weights = {
        {"arts & entertainment-opera", Data_point(std::vector<float>{1.0, 2.0, 3.0}) },
        {"home-garden", Data_point(std::vector<float>{3.0, 2.0, 1.0}) }, 
        {"travel-hotels", Data_point(std::vector<float>{2.0, 2.0, 2.0}) }
    };
    std::map<std::string,float> biases = {
        {"arts & entertainment-opera",0.0},
        {"home-garden", 0.0},
        {"travel-hotels", 0.0}
    };
    Linear_classifier linear_classifier(weights,biases);
    // auto pipeline = Pipeline(transformations, linear_classifier);
    Pipeline pipeline;
    pipeline = Pipeline(transformations, linear_classifier);    
    unsigned int expected_len = 203;//203 categories
    auto rez = pipeline.Get_Advertising_Predictions(test_string);
    EXPECT_EQ(expected_len, rez.size());
    double sum = 0.0;
    for (unsigned long i = 0 ; i < rez.size(); i++ ){
      EXPECT_TRUE(rez[i]>=0);
      EXPECT_TRUE(rez[i]<=1);
      sum+=rez[i];
    }
    EXPECT_TRUE(std::abs(sum-1.0)<0.000001);
}


}