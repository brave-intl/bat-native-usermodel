/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/vendor/bat-native-usermodel/src/transformation.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"
#include <list>
#include <vector> 

namespace usermodel {

// Test data directory, relative to source root
// const base::FilePath::CharType kTestDataRelativePath[] =
//   FILE_PATH_LITERAL("brave/vendor/bat-native-usermodel/test/data");

class TransformationTest : public ::testing::Test {
 protected:
  TransformationTest() {
    // You can do set-up work for each test here
  }

  ~TransformationTest() override {
    // You can do clean-up work that doesn't throw exceptions here
  }

  // If the constructor and destructor are not enough for setting up and
  // cleaning up each test, you can use the following methods

  void SetUp() override {
    // Code here will be called immediately after the constructor (right before
    // each test)
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right before the
    // destructor)
  }

  // Objects declared here can be used by all tests in the test case
};

TEST_F(TransformationTest, ToLowerTest) {
  std::string upper_case = "LOWER CASE";
  std::string lower_case = "lower case";
  auto upper_datapoint = usermodel::DataPoint(upper_case);
  usermodel::To_lower to_lower;
  auto lower_datapoint=to_lower.get(upper_datapoint);
  EXPECT_TRUE(DataType::text_data == lower_datapoint.type);
  EXPECT_EQ(0, lower_case.compare(lower_datapoint.data_text));
}

TEST_F(TransformationTest, HashingTest) {
  std::string test_string = "tiny";
  auto text_datapoint = usermodel::DataPoint(test_string);
  usermodel::Hashed_ngrams hashed_ngrams;
  hashed_ngrams= usermodel::Hashed_ngrams();
  // hashed_ngrams.apply();
  auto vector_data = hashed_ngrams.get(text_datapoint);
  EXPECT_EQ(DataType::sparse_vector, vector_data.type);
  EXPECT_EQ(10000, vector_data.n_dims);//10k is the default size
  size_t expected_elements = 10;// hashes for [t,i,n,y, ti,in, ny, tin, iny, tiny]==> 10 total
  EXPECT_EQ(expected_elements, vector_data.data_sparse.size());
}

TEST_F(TransformationTest, CustomHashingTest) {
  std::string test_string = "tiny";
  auto text_datapoint = usermodel::DataPoint(test_string);
  usermodel::Hashed_ngrams hashed_ngrams;
  hashed_ngrams= usermodel::Hashed_ngrams(3, std::vector<int>{1, 2, 3} );
  auto vector_data = hashed_ngrams.get(text_datapoint);
  EXPECT_EQ(DataType::sparse_vector, vector_data.type);
  EXPECT_EQ(3, vector_data.n_dims);//3 is the custom size
  size_t expected_elements = 3;// all 3 buckets should be nonempty
  EXPECT_EQ(expected_elements, vector_data.data_sparse.size());
}

TEST_F(TransformationTest, NormalizationTest) {
  std::string test_string = "quite a small test string";
  auto text_datapoint = usermodel::DataPoint(test_string);
  usermodel::Hashed_ngrams hashed_ngrams;
  hashed_ngrams= usermodel::Hashed_ngrams(10, std::vector<int>{3, 4} );
  auto vector_data = hashed_ngrams.get(text_datapoint);
  usermodel::Normalize normalize;
  auto normalized_datapoint = normalize.get(vector_data);
  auto s = 0.0; 
  for (auto const& x : normalized_datapoint.data_sparse){
        EXPECT_TRUE(x.second>=0.0);
        EXPECT_TRUE(x.second<=1.0);
        s += x.second*x.second ;
    }
  EXPECT_TRUE((s-1)<0.0000001);
}

TEST_F(TransformationTest, ChainingTest) {
  //chain a mixture of the above tests together / things should be talking to each other ok
  std::vector<Transformation> chain;
  usermodel::To_lower to_lower;
  chain.push_back(to_lower);
  usermodel::Hashed_ngrams hashed_ngrams;
  hashed_ngrams= usermodel::Hashed_ngrams();
  chain.push_back(hashed_ngrams);
  
  std::string test_string = "TINY";
  auto last_point = DataPoint(test_string);
  
  for (unsigned i = 0; i<chain.size();i++){
    auto transform = chain[i];
    last_point = transform.get(last_point);
  }

  EXPECT_EQ(DataType::sparse_vector, last_point.type);
  EXPECT_EQ(10000, last_point.n_dims);
  size_t expected_elements = 10;// hashes for [t,i,n,y, ti,in, ny, tin, iny, tiny]==> 10 total
  EXPECT_EQ(expected_elements, last_point.data_sparse.size());
}

}  // namespace usermodel
