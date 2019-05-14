/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/vendor/bat-native-usermodel/src/data_point.h"
#include <iostream>
#include <cmath>
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace usermodel {

// SO! You will notice that there is a fair bit of repetition for things here.
// If you want to factor things out in a setup/teardown, please do so
// I tried, I failed, I couldn't waste any more time
class Data_point_test : public ::testing::Test {
 protected:
  void SetUp() override {
  }
  void TearDown() override {}

};


TEST_F(Data_point_test, Datapoint_type_assertions) {
  //string:
  std::string a = "trololol";
  auto string_data_point = usermodel::Data_point(a);
  EXPECT_EQ(usermodel::text_data, string_data_point.type);
  EXPECT_EQ(0,string_data_point.n_dims);
  //dense vectors:
  std::vector<float> v_5{1.0,2.0,3.0,4.0,5.0};
  auto dense_datapoint_5 = usermodel::Data_point(v_5);
  EXPECT_EQ(usermodel::vector_data, dense_datapoint_5.type);
  EXPECT_EQ(5, dense_datapoint_5.n_dims);
  std::vector<float> v_3{1.0,2.0,3.0};
  auto dense_datapoint_3 = usermodel::Data_point(v_3);
  EXPECT_EQ(usermodel::vector_data, dense_datapoint_3.type);
  EXPECT_EQ(3, dense_datapoint_3.n_dims);
  //sparse vectors:
  std::map<unsigned,float> s_3 = {{0,1.0}, {2,2.0}};// dense equivalent:[1,0,2]
  auto sparse_datapoint_3=usermodel::Data_point(s_3,3);
  EXPECT_EQ(usermodel::sparse_vector, sparse_datapoint_3.type);
  EXPECT_EQ(3, sparse_datapoint_3.n_dims);
  std::map<unsigned,float> s_5 = {{0,1.0}, {2,3.0}, {3, -2.0}};// dense equivalent:[1,0,3,2,0]
  auto sparse_datapoint_5=usermodel::Data_point(s_5,5);
  EXPECT_EQ(usermodel::sparse_vector, sparse_datapoint_5.type);
  EXPECT_EQ(5, sparse_datapoint_5.n_dims);
}

TEST_F(Data_point_test, Dense_dense_product) {
  auto EPS=0.000001;
  std::vector<float> v_5{1.0,2.0,3.0,4.0,5.0};
  auto dense_datapoint_5 = usermodel::Data_point(v_5);
  std::vector<float> v_3{1.0,2.0,3.0};
  auto dense_datapoint_3 = usermodel::Data_point(v_3);
  std::vector<float> v_3_1{1.0, 1.0, 1.0};
  auto dense_datapoint_3_1 = usermodel::Data_point(v_3_1);
  auto rez_3x3 = dense_datapoint_3*dense_datapoint_3; // = 14
  auto rez_5x5 = dense_datapoint_5*dense_datapoint_5; // = 55
  auto rez_3x1 = dense_datapoint_3*dense_datapoint_3_1;// = 6
  EXPECT_TRUE(std::abs(14-rez_3x3)<EPS);
  EXPECT_TRUE(std::abs(55-rez_5x5)<EPS);
  EXPECT_TRUE(std::abs(6-rez_3x1)<EPS);
}

TEST_F(Data_point_test, Sparse_sparse_product) {
  auto EPS=0.000001;
  std::map<unsigned,float> s_3 = {{0,1.0}, {2,2.0}};// dense equivalent:[1,0,2]
  std::map<unsigned,float> s_5 = {{0,1.0}, {2,3.0}, {3, -2.0}};// dense equivalent:[1,0,3,2,0]
  auto sparse_datapoint_3=usermodel::Data_point(s_3,3);
  auto sparse_datapoint_5=usermodel::Data_point(s_5,5); 
  auto rez_3x3 = sparse_datapoint_3*sparse_datapoint_3; // = 5
  auto rez_5x5 = sparse_datapoint_5*sparse_datapoint_5; // = 14
  EXPECT_TRUE(std::abs(14-rez_5x5)<EPS);
  EXPECT_TRUE(std::abs(5-rez_3x3)<EPS);
}

TEST_F(Data_point_test, Sparse_dense_product) {
  auto EPS=0.000001;

  std::vector<float> v_5{1.0,2.0,3.0,4.0,5.0};
  auto dense_datapoint_5 = usermodel::Data_point(v_5);
  std::vector<float> v_3{1.0,2.0,3.0};
  auto dense_datapoint_3 = usermodel::Data_point(v_3);
  std::map<unsigned,float> s_3 = {{0,1.0}, {2,2.0}};// dense equivalent:[1,0,2]
  std::map<unsigned,float> s_5 = {{0,1.0}, {2,3.0}, {3, -2.0}};// dense equivalent:[1,0,3,2,0]
  auto sparse_datapoint_3=usermodel::Data_point(s_3,3);
  auto sparse_datapoint_5=usermodel::Data_point(s_5,5);


  auto mixed_rez_3x3_1 = dense_datapoint_3*sparse_datapoint_3; // = 7
  auto mixed_rez_5x5_1 = dense_datapoint_5*sparse_datapoint_5; // = 2
  auto mixed_rez_3x3_2 = sparse_datapoint_3*dense_datapoint_3; // = 7
  auto mixed_rez_5x5_2 = sparse_datapoint_5*dense_datapoint_5; // = 2
  
  EXPECT_TRUE( std::abs(mixed_rez_3x3_1-mixed_rez_3x3_2) < EPS);
  EXPECT_TRUE( std::abs(mixed_rez_5x5_1-mixed_rez_5x5_2) < EPS);
  EXPECT_TRUE( std::abs(7 - mixed_rez_3x3_1) < EPS);
  EXPECT_TRUE( std::abs(2 - mixed_rez_5x5_2) < EPS);
}

TEST_F(Data_point_test, Nonsense_product) {
  std::string a = "trololol";
  auto string_data_point = usermodel::Data_point(a);
  std::vector<float> v_5{1.0,2.0,3.0,4.0,5.0};
  auto dense_datapoint_5 = usermodel::Data_point(v_5);
  std::vector<float> v_3{1.0,2.0,3.0};
  auto dense_datapoint_3 = usermodel::Data_point(v_3);
  std::map<unsigned,float> s_3 = {{0,1.0}, {2,2.0}};// dense equivalent:[1,0,2]
  std::map<unsigned,float> s_5 = {{0,1.0}, {2,3.0}, {3, -2.0}};// dense equivalent:[1,0,3,2,0]
  auto sparse_datapoint_3=usermodel::Data_point(s_3,3);
  auto sparse_datapoint_5=usermodel::Data_point(s_5,5);


  auto wrong_dd = dense_datapoint_5*dense_datapoint_3;
  EXPECT_TRUE( std::isnan(wrong_dd));
  auto wrong_ss = sparse_datapoint_3*sparse_datapoint_5;
  EXPECT_TRUE(std::isnan(wrong_ss));
  auto wrong_sd = sparse_datapoint_3 * dense_datapoint_5;
  EXPECT_TRUE(std::isnan(wrong_sd)); 
  auto wrong_ds = dense_datapoint_5 * sparse_datapoint_3;
  EXPECT_TRUE(std::isnan(wrong_ds));
  auto wrong_strd = string_data_point*dense_datapoint_5;
  EXPECT_TRUE(std::isnan(wrong_strd));
  auto wrong_strs = string_data_point*sparse_datapoint_3;
  EXPECT_TRUE(std::isnan(wrong_strs));
}

}  // namespace usermodel