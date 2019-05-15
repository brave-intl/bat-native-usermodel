/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/vendor/bat-native-usermodel/src/pipeline.h"
#include "brave/vendor/bat-native-usermodel/src/transformation.h"
#include "brave/vendor/bat-native-usermodel/src/data_point.h"
#include <iostream>
#include <cmath>
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace usermodel {

class Pipeline_test : public ::testing::Test {
 protected:
  void SetUp() override {
  }
  void TearDown() override {}

};


TEST_F(Pipeline_test, Build_simple_pipeline) {
    std::string test_string = "Test String";
    auto string_data_point = Data_point(test_string);
    std::list<Transformation> transformations;
    usermodel::To_lower to_lower;
    transformations.push_back(to_lower);
    usermodel::Hashed_ngrams hashed_ngrams;
    transformations.push_back(hashed_ngrams);
    std::map<std::string,Data_point> weights = {
        {"class_1", std::vector<float>{1.0, 2.0, 3.0}},
        {"class_2", std::vector<float>{3.0, 2.0, 1.0}}, 
        {"class_3", std::vector<float>{2.0, 2.0, 2.0}}
    };
    std::map<std::string,float> biases = {
        {"class_1",0.0},
        {"class_2", 0.0},
        {"class_3", 0.0}
    };
    Classifier linear_classifier = Linear_classifier(weights,biases);
    // auto pipeline = Pipeline(transformations, linear_classifier);
    Pipeline pipeline;
    pipeline = Pipeline(transformations, linear_classifier);
    auto rez = pipeline.apply(string_data_point);
    unsigned int expected_len = 2;
    EXPECT_EQ(expected_len, rez.size());
}
}