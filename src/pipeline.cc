/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "pipeline.h"
#include "base/json/json_reader.h"
#include "base/values.h"
#include "advertising_categories.h"

namespace usermodel {

Pipeline::Pipeline(){
    version_ = 0;
    timestamp_ ="";
    locale_ = "en";
    transformations_ = {};
    advertising_categories_ = get_advertising_categories();
    get_reverse_categories();
    // classifier_ = nullptr;
}

Pipeline::Pipeline(const Pipeline &pipeline) {
  transformations_ = pipeline.transformations_;
  classifier_ = pipeline.classifier_;
  advertising_categories_ = get_advertising_categories();
  get_reverse_categories();
}

Pipeline::Pipeline(std::vector<Transformation> transformations,
  Linear_classifier classifier) {
  transformations_ = transformations;
  classifier_ = classifier;
  advertising_categories_ = get_advertising_categories();
  get_reverse_categories();
}

void Pipeline::get_reverse_categories(){
  for (auto const& category : advertising_categories_){
    reverse_categories_[category.second] = category.first;
  }
}

Pipeline::~Pipeline() = default;

bool Pipeline::FromJson(const std::string& json) {
  base::Optional<base::Value> root = base::JSONReader::Read(json);
  if (!root){
    return false;
  }
  base::Value* version = root->FindKeyOfType("version", base::Value::Type::INTEGER);
  int version_number;
  bool parsed_version_number = version->GetAsInteger(&version_number);
  if (!parsed_version_number){

    return false;
  }
  version_ = version_number;
  
  base::Value* timestamp = root->FindKey("timestamp");
  if (!timestamp){
    return false;
  }
  std::string parsed_timestamp;
  bool parsed_timestamp_success = timestamp->GetAsString(&parsed_timestamp);
  if (!parsed_timestamp_success){
    return false;
  }
  timestamp_ = parsed_timestamp;

  base::Value* locale = root->FindKey("locale");
  if (!locale){
    return false;
  }
  std::string parsed_locale;
  bool parsed_locale_success = locale->GetAsString(&parsed_locale);
  if (!parsed_locale_success){
    return false;
  }
  locale_ = parsed_locale;

  base::Value* transformations = root->FindKey("transformations");
  
  bool loaded_transformations = parse_transformations(transformations);
  if (!loaded_transformations){
    return false;
  }
  base::Value* classifier = root->FindKey("classifier");
  if (!parse_classifier(classifier)){
    return false;
  }
  
  return true;
}

bool Pipeline::parse_transformations(base::Value* transformations) {
  if (!transformations->is_list()) {
    return false;
  }
  std::vector<Transformation> transformation_sequence;
  for (size_t i = 0; i < transformations->GetList().size(); i++) {
    const base::Value& transformation = transformations->GetList()[i];
    const base::Value* transformation_type =
        transformation.FindKey("transformation_type");
    if (!transformation_type) {
      return false;
    }
    std::string parsed_transformation_type;
    bool parsed_transformation_success =
        transformation_type->GetAsString(&parsed_transformation_type);
    if (!parsed_transformation_success) {
      return false;
    }
    if (parsed_transformation_type.compare("TO_LOWER") == 0) {
      transformation_sequence.push_back(To_lower());
    }
    if (parsed_transformation_type.compare("HASHED_NGRAMS") == 0) {
      const base::Value* transformation_params =
          transformation.FindKey("params");
      if (!transformation_params) {
        return false;
      }

      const base::Value* nb = transformation_params->FindKey("num_buckets");
      int num_buckets;
      bool parsed_num_buckets = nb->GetAsInteger(&num_buckets);
      if (!parsed_num_buckets) {
        return false;
      }
      const base::Value* n_gram_sizes = transformation_params->FindKey("ngrams_range");
      if (!n_gram_sizes->is_list()) {
        return false;
      }
      std::vector<int> ngram_range = {};
      for (size_t i = 0; i < n_gram_sizes->GetList().size(); i++) {
        const base::Value& n = n_gram_sizes->GetList()[i];
        ngram_range.push_back(n.GetInt());
      }
      usermodel::Hashed_ngrams hashed_ngrams;
      hashed_ngrams = usermodel::Hashed_ngrams(num_buckets, ngram_range);
      transformation_sequence.push_back(hashed_ngrams);
    }
  }
  transformations_ = transformation_sequence;
  return true;
}

bool Pipeline::parse_classifier(base::Value* classifier){
  std::vector<std::string> classes;
  base::Value* classifier_type = classifier->FindKey("classifier_type");
  if (!classifier_type){
    return false;
  }
  std::string parsed_classifier_type;
  bool parsed_classifier_type_success = classifier_type->GetAsString(&parsed_classifier_type);
  if (!parsed_classifier_type_success){
    return false;
  }
  if (parsed_classifier_type.compare("LINEAR")){
    return false;
  }
  
  base::Value* specified_classes = classifier->FindKey("classes");
  if (!specified_classes->is_list()){
    return false;
  }

  for (size_t i = 0 ; i < specified_classes->GetList().size();i++){
    const base::Value& class_name = specified_classes->GetList()[i];
    classes.push_back(class_name.GetString());
  }
  base::Value* class_weights = classifier->FindKey("class_weights");
  if ( !class_weights->is_dict()) {
    return false;
  }
  std::map<std::string,DataPoint> weights;
  for (size_t i = 0 ; i < classes.size();i++){
    base::Value* this_class = class_weights->FindKey(classes[i]);
    if ( !this_class->is_list()) {
      return false;
    } 
    std::vector<float> tmp_weights = {};
    for (size_t j = 0 ; j < this_class->GetList().size(); j++ ){
      const base::Value& weight = this_class->GetList()[j];
      tmp_weights.push_back( static_cast<float> (weight.GetDouble()) );
    }
    auto dubs = usermodel::DataPoint(tmp_weights);
    weights.insert({classes[i], dubs});
  }
  //finally do the biases: 
  std::map<std::string, float> specified_biases = {};
  base::Value* biases = classifier->FindKey("biases");
  if (!biases->is_list()){
    return false;
  }
  if (biases->GetList().size() != classes.size()){
    return false;
  }
  for (size_t i = 0 ; i < biases->GetList().size();i++){
    const base::Value& this_bias = biases->GetList()[i];
    specified_biases.insert({classes.at(i), static_cast<float> (this_bias.GetDouble())}) ;
  }
  classifier_ = Linear_classifier(weights, specified_biases);
  return true;
}


bool Pipeline::GetVersionFromJSON(base::DictionaryValue* dictionary) {
  auto* version_value = dictionary->FindKey("version");
  if (!version_value) {
    return false;
  }

  version_ = std::stoull(version_value->GetString());
  return true;
}

bool Pipeline::GetTimestampFromJSON(base::DictionaryValue* dictionary) {
  auto* timestamp_value = dictionary->FindKey("timestamp");
  if (!timestamp_value) {
    return false;
  }

  timestamp_ = timestamp_value->GetString();
  return true;
}

bool Pipeline::GetLocaleFromJSON(base::DictionaryValue* dictionary) {
  auto* locale_value = dictionary->FindKey("locale");
  if (!locale_value) {
    return false;
  }

  locale_ = locale_value->GetString();
  return true;
}

std::map<std::string, float> Pipeline::Apply( const DataPoint &inp) {
  DataPoint last_point = DataPoint(inp);
  for ( auto& transformation : transformations_) {
    last_point = transformation.get(last_point);
  }

  return classifier_.Predict(last_point);
}

std::map<std::string, double> Pipeline::Get_Top_Predictions(const std::string &html){
  std::map<std::string, double> rtn;
  DataPoint data = DataPoint(html);
  auto predictions = Softmax(Apply(data));
  float expected_prob = 1.0 / predictions.size();
  for (auto const &prediction: predictions){
    if (prediction.second>expected_prob) {
      rtn[prediction.first] = prediction.second;
    }
  }
  return rtn;
}

std::vector<double> Pipeline::Get_Advertising_Predictions(const std::string &html){
  DataPoint data = DataPoint(html);
  auto predictions = Softmax(Apply(data));
  std::vector<double> rtn = {};
  for (size_t i = 0 ; i < advertising_categories_.size();i++){
    rtn.push_back(0.0);
  }
  for (auto const& prediction: predictions){
    auto class_name = prediction.first;
    auto value = prediction.second;
    if (advertising_categories_.count(class_name) > 0 ){
      rtn[advertising_categories_[class_name]] = value;
    }
  }
  return rtn;
}

std::string Pipeline::get_category(int c){
  return reverse_categories_[c];
}
}  // namespace usermodel
