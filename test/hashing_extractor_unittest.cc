#include "base/json/json_reader.h"
#include <stddef.h>
#include <utility>
#include "base/base_paths.h"
#include "base/files/file_util.h"
#include "base/logging.h"
#include "base/path_service.h"
#include "base/stl_util.h"
#include "base/strings/string_piece.h"
#include "base/strings/utf_string_conversions.h"
#include "base/values.h"
#include "build/build_config.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "brave/vendor/bat-native-usermodel/src/hashing_extractor.h"
#include <fstream>
#include <codecvt>
#include <cmath>


const base::FilePath::CharType kTestDataRelativePath[] =
  FILE_PATH_LITERAL("brave/vendor/bat-native-usermodel/test/data");

namespace usermodel {

class Hashing_extractor_test : public ::testing::Test {
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
//If you ever have to convince people for protobuf vs json take a look at the following test: 
void run_test_case(std::string test_case){
    float EPS = 0.0000001;
    auto test_json = usermodel::LoadFile("hash_check.json");
    ASSERT_TRUE( (test_json.size() > 0) );
    
    base::Optional<base::Value> root = base::JSONReader::Read(test_json);
    ASSERT_TRUE(root);

    base::Value* case_params = root->FindKeyOfType(test_case, base::Value::Type::DICTIONARY);
    ASSERT_TRUE(case_params);
    base::Value* input = case_params->FindKey("input");
    ASSERT_TRUE(input);
    std::string input_value;
    EXPECT_TRUE(input->GetAsString(&input_value));

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring w_input_value = converter.from_bytes(input_value);

    // EXPECT_TRUE( (0==input_value.size()) ); 
    base::Value* idx = case_params->FindKey("idx");
    ASSERT_TRUE(idx->is_list());
    base::Value* count = case_params->FindKey("count");
    ASSERT_TRUE(count->is_list());
    usermodel::HashVectorizer vectorizer;
    EXPECT_TRUE(vectorizer.Process(w_input_value));
    auto frequencies = vectorizer.GetFrequencies();
    EXPECT_EQ(idx->GetList().size(), frequencies.size());
    //if (frequencies.size() > 0){
    for (unsigned long i = 0; i < frequencies.size(); i++){
        const base::Value& idx_val = idx->GetList()[i];
        const base::Value& count_val = count->GetList()[i];
        EXPECT_TRUE( (count_val.GetInt()- frequencies[idx_val.GetInt()]) <EPS );
        // ASSERT_TRUE(value1.isint());
    }
        
     
    //}


}


TEST(Hashing_extractor_test, futile) {
    base::Optional<base::Value> root = base::JSONReader::Read(
        "{"
        "  \"test\": {"
        "    \"foo\": true,"
        "    \"bar\": 3.14,"
        "    \"baz\": \"bat\","
        "    \"moo\": \"cow\""
        "  },"
        "  \"list\": ["
        "    \"a\","
        "    \"b\""
        "  ]"
        "}");
    ASSERT_TRUE(root);
    ASSERT_TRUE(root->is_dict());

    base::Value* dict = root->FindKeyOfType("test", base::Value::Type::DICTIONARY);
    ASSERT_TRUE(dict);
    base::Value* list = root->FindKeyOfType("list", base::Value::Type::LIST);
    ASSERT_TRUE(list);
}

TEST(Hashing_extractor_test, test_empty) {
    usermodel::run_test_case("empty");
}

TEST(Hashing_extractor_test, test_tiny) {
    usermodel::run_test_case("tiny");
}

TEST(Hashing_extractor_test, test_english) {
    usermodel::run_test_case("english");
}

TEST(Hashing_extractor_test, test_greek) {
    usermodel::run_test_case("greek");
}

TEST(Hashing_extractor_test, test_japanese) {
    usermodel::run_test_case("japanese");
}

TEST(Hashing_extractor_test, test_multilingual) {
    usermodel::run_test_case("japanese");
}



}