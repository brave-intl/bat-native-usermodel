/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef DEPS_BAT_USERMODEL_SRC_PIPELINE_H_
#define DEPS_BAT_USERMODEL_SRC_PIPELINE_H_

#include <stdint.h>
#include <string>
#include <vector>
#include <map>

#include "data_point.h"
#include "transformation.h"
#include "classifier.h"

#include "base/values.h"

namespace usermodel {

class Pipeline {
 public:
  Pipeline();
  Pipeline(const Pipeline &pipeline);
  Pipeline(
      std::vector<Transformation> transformations,
      Linear_classifier classifier);
  ~Pipeline();

  bool FromJson(const std::string& json);
  std::map<std::string, float> Apply(const DataPoint &inp);
  std::map<std::string, float> Get_Top_Predictions(const std::string &html);
  std::vector<double> Get_Advertising_Predictions(const std::string &html);
  std::string get_category(int c);
 private:
  uint16_t version_;
  std::string timestamp_;
  std::string locale_;
  std::vector<Transformation> transformations_;
  std::map<std::string,int> advertising_categories_;
  std::map<int, std::string> reverse_categories_;
  Linear_classifier classifier_;
  void get_reverse_categories();
  bool parse_classifier(base::Value* classifier);
  bool parse_transformations(base::Value* transformations);
  bool GetVersionFromJSON(base::DictionaryValue* dictionary);
  bool GetTimestampFromJSON(base::DictionaryValue* dictionary);
  bool GetLocaleFromJSON(base::DictionaryValue* dictionary);
  bool GetTransformationsFromJSON(base::DictionaryValue* dictionary);
  bool GetClassifierFromJSON(base::DictionaryValue* dictionary);
  bool GetTransformationsFromList(base::ListValue List);
};

}  // namespace usermodel

#endif  // DEPS_BAT_USERMODEL_SRC_PIPELINE_H_
