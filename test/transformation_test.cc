/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <fstream>
#include <streambuf>
#include <iostream>
#include "transformation.h"
#include "base/files/file_path.h"

#include "brave/vendor/bat-native-usermodel/src/user_model_impl.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace usermodel {

// Test data directory, relative to source root
const base::FilePath::CharType kTestDataRelativePath[] =
  FILE_PATH_LITERAL("brave/vendor/bat-native-usermodel/test/data");

class TraansformationTest : public ::testing::Test {
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
  auto upper_datapoint = usermodel::Data_point(upper_case);
  usermodel::To_lower to_lower;
  EXPECT_TRUE(to_lower.apply(upper_datapoint));
  auto lower_datapoint=to_lower.get();
  EXPECT_EQ(0, lower_datapoint.type);
  EXPECCT_EQ(0, lower_case.compare(lower_datapoint.data_text))
}

}  // namespace usermodel
