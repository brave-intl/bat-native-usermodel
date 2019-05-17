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
    auto rez = pipeline.apply(string_data_point);
    
    EXPECT_EQ(expected_len, rez.size());
}

// TEST_F(Pipeline_test, Load_representation_json) {
//     std::string test_string = "{\"transforms\": [{\"transformation_type\": \"TO_LOWER\", \"params\": null}, {\"transformation_type\": \"HASHED_NGRAMS\", \"params\": {\"n_range\": [1, 2, 3, 4, 5], \"num_buckets\": 500}}]}"  //  NOLINT    
//     Rep = load_rep(test_string);

//     EXPECT_EQ(expected_len, rez.size());
// }


//Test compatibility with a simple python generated model 

TEST_F(Pipeline_test, Test_Python_Compat){

std::vector<std::string> test_messages{ "This is a spam email.", 
                                        "Another spam trying to sell you viagra",
                                        "Message from mom with no real subject", 
                                        "Another messase from mom with no real subject",
                                        "Yadayada"};

std::vector<std::string> test_labels{"spam", "spam", "ham", "ham", "junk"};

auto test_json = LoadFile("pipeline.json");
usermodel::Pipeline pipeline; 
auto load_success = pipeline.from_json(test_json);
EXPECT_TRUE(load_success);
for (long unsigned i = 0; i < test_messages.size(); i++){
    auto tmp = Data_point(test_messages[i]);
    auto predictions = pipeline.apply(tmp);
    EXPECT_EQ(3, predictions.size());
    for (auto const& prediction : predictions){
        EXPECT_TRUE(prediction.second <= predictions[test_labels[i]]);
    } 

}

}

}