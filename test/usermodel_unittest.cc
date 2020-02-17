/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <fstream>
#include <streambuf>

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

class UserModelTest : public ::testing::Test {
 protected:
  UserModelTest() {
    // You can do set-up work for each test here
  }

  ~UserModelTest() override {
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
  std::string LoadFile(const std::string& filename) {
    base::FilePath path(kTestDataRelativePath);
    path = path.AppendASCII(filename);

    std::ifstream ifs(path.value().c_str());
    if (ifs.fail()) {
      return "";
    }

    std::stringstream stream;
    stream << ifs.rdbuf();
    std::string value = stream.str();

    return value;
  }
};

TEST_F(UserModelTest, ValidModelTest) {
  UserModelImpl user_model;

  auto model = LoadFile("hashing_model.json");
  EXPECT_TRUE(user_model.InitializePageClassifier(model));
}

TEST_F(UserModelTest, InvalidModelTest) {
  UserModelImpl user_model;

  auto model = LoadFile("invalid-model.json");
  EXPECT_FALSE(user_model.InitializePageClassifier(model));
}

TEST_F(UserModelTest, MissingModelTest) {
  UserModelImpl user_model;

  auto model = LoadFile("missing-model.json");
  EXPECT_FALSE(user_model.InitializePageClassifier(model));
}



}  // namespace usermodel
