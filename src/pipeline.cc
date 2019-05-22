/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "pipeline.h"
// #include "vector"
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
    get_reverse_categories()
    // classifier_ = nullptr;
}

Pipeline::Pipeline(const Pipeline &pipeline) {
  transformations_ = pipeline.transformations_;
  classifier_ = pipeline.classifier_;
  advertising_categories_ = get_advertising_categories();
  get_reverse_categories()
}

Pipeline::Pipeline(std::vector<Transformation> transformations,
  Linear_classifier classifier) {
  transformations_ = transformations;
  classifier_ = classifier;
  advertising_categories_ = get_advertising_categories();
  get_reverse_categories()
}

Pipeline::get_reverse_categories(){
  for (auto const& category : advertising_categories_){
    reverse_categories_[category.second] = category.first;
  }
}

Pipeline::~Pipeline() = default;

bool Pipeline::FromJson(const std::string& json) {
  std::cout<<"*** IN: "<<std::endl;
  base::Optional<base::Value> root = base::JSONReader::Read(json);
  if (!root){
    std::cout<<"*** wtf root!: "<<std::endl;
    return false;
  } 
  std::cout<<"*** Loaded root: "<<std::endl;
  base::Value* version = root->FindKeyOfType("version", base::Value::Type::INTEGER);
  int version_number;
  bool parsed_version_number = version->GetAsInteger(&version_number);
  if (!parsed_version_number){
    std::cout<<"*** version number poopoo: "<<std::endl;
    return false;
  }
  version_ = version_number;
  std::cout<<"*** version number: "<< version_<<std::endl;
  
  
  base::Value* timestamp = root->FindKey("timestamp");
  if (!timestamp){
    std::cout<< "*** timestamp poopoo "<<std::endl;
    return false;
  }
  std::string parsed_timestamp;
  bool parsed_timestamp_success = timestamp->GetAsString(&parsed_timestamp);
  if (!parsed_timestamp_success){
    std::cout<<"*** timestamp poopoo 2 "<<std::endl;
    return false;
  }
  timestamp_ = parsed_timestamp;
  std::cout<<"*** timestamp: "<< timestamp_<<std::endl;

  base::Value* locale = root->FindKey("locale");
  if (!locale){
    std::cout<< "*** locale poopoo "<<std::endl;
    return false;
  }
  std::string parsed_locale;
  bool parsed_locale_success = locale->GetAsString(&parsed_locale);
  if (!parsed_locale_success){
    std::cout<<"*** locale poopoo 2 "<<std::endl;
    return false;
  }
  locale_ = parsed_locale;
  std::cout<<"*** locale: "<< locale_<<std::endl;

  std::cout<<"*** about to load transformations "<<std::endl;

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

bool Pipeline::parse_transformations(base::Value* transformations){
  if (!transformations->is_list()){
    std::cout<<"No valid transformations"<<std::endl;
    return false;
  }
  std::vector<Transformation> transformation_sequence;
  std::cout<<"Now have: "<< transformations->GetList().size()<<" transformations to process"<<std::endl;
  for (unsigned long i = 0 ; i < transformations->GetList().size();i++){
    const base::Value& transformation = transformations->GetList()[i];
    const base::Value* transformation_type = transformation.FindKey("transformation_type");
    if (!transformation_type){
      return false;
    }
    std::string parsed_transformation_type;
    bool parsed_transformation_success = transformation_type->GetAsString(&parsed_transformation_type);
    if (!parsed_transformation_success){
      std::cout<<"*** transformation type poopoo 2 "<<std::endl;
      return false;
    }
    if (parsed_transformation_type.compare("TO_LOWER")==0){
      std::cout<<"*** "<<i<<": pushing to lower "<< std::endl;
      transformation_sequence.push_back(To_lower());
    }
    if (parsed_transformation_type.compare("HASHED_NGRAMS")==0){
       std::cout<<"*** "<<i<<": pushing hashed_ngrams "<< std::endl;
       const base::Value* transformation_params = transformation.FindKey("params");
       if (!transformation_params){
         std::cout<< "*** NO PARAMS FOUND"<<std::endl;
         return false;
       }
    const base::Value* nb = transformation_params->FindKey("num_buckets");
    int num_buckets;
    bool parsed_num_buckets = nb->GetAsInteger(&num_buckets);
    if (!parsed_num_buckets){
      std::cout<<"*** num buckets poopoo: "<<std::endl;
      return false;
    }
    std::cout<<"*** num_buckets: "<< num_buckets <<std::endl;
    const base::Value* n_gram_sizes = transformation_params->FindKey("ngrams_range");
    if (!n_gram_sizes->is_list()){
      std::cout << "*** failed to load n-grams list"<<std::endl;
      return false;
    }
    std::vector<int> ngram_range = {};
    for (unsigned long i =0; i < n_gram_sizes->GetList().size();i++){
      const base::Value& n = n_gram_sizes->GetList()[i];
      std::cout<< "*** pushing back n: "<<n <<std::endl;
      ngram_range.push_back(n.GetInt());
    }
    usermodel::Hashed_ngrams hashed_ngrams;
    hashed_ngrams = usermodel::Hashed_ngrams(num_buckets, ngram_range);
    transformation_sequence.push_back(hashed_ngrams);
    std::cout<< "*** pushed back hashed ngrams";
    }
  }
  transformations_ = transformation_sequence;
  return true;
}

bool Pipeline::parse_classifier(base::Value* classifier){
  std::cout << "*** parsing classifier:"<<std::endl;
  std::vector<std::string> classes;
  base::Value* classifier_type = classifier->FindKey("classifier_type");
  if (!classifier_type){
    std::cout<< "*** classifier_type poopoo "<<std::endl;
    return false;
  }
  std::string parsed_classifier_type;
  bool parsed_classifier_type_success = classifier_type->GetAsString(&parsed_classifier_type);
  if (!parsed_classifier_type_success){
    std::cout<<"*** class_type poopoo 2 "<<std::endl;
    return false;
  }
  if (parsed_classifier_type.compare("LINEAR")){
    std::cout<<"*** unknown class type"<<std::endl;
    return false;
  }
  std::cout<<"*** classifier_type: "<< parsed_classifier_type <<std::endl;
  
  base::Value* specified_classes = classifier->FindKey("classes");

  if (!specified_classes->is_list()){
    std::cout<<"No valid specified classes"<<std::endl;
    return false;
  }

  std::cout<<"Now have: "<< specified_classes->GetList().size()<<" classes to process"<<std::endl;
  for (unsigned long i = 0 ; i < specified_classes->GetList().size();i++){
    const base::Value& class_name = specified_classes->GetList()[i];
    std::cout << "got class: " << class_name << std::endl;     
    classes.push_back(class_name.GetString());
  }
  base::Value* class_weights = classifier->FindKey("class_weights");
  if ( !class_weights->is_dict()) {
    return false;
  }
  std::map<std::string,Data_point> weights;
  weights = {};
  std::cout<<" Loaded class weights "<<std::endl;
  for (unsigned long i = 0 ; i < classes.size();i++){
    base::Value* this_class = class_weights->FindKey(classes[i]);
    std::cout << "Loading weights for "<< classes[i] <<std::endl;
    if ( !this_class->is_list()) {
      return false;
    } 
    std::vector<float> tmp_weights = {};
    for (unsigned long j = 0 ; j < this_class->GetList().size(); j++ ){
      const base::Value& weight = this_class->GetList()[j];
      tmp_weights.push_back( (float) weight.GetDouble());
    }
    auto dubs = usermodel::Data_point(tmp_weights);
    weights.insert({classes[i], dubs});
  }
  //finally do the biases: 
  std::map<std::string, float> specified_biases = {};
  base::Value* biases = classifier->FindKey("biases");
  if (!biases->is_list()){
    std::cout<<"biases not list"<<std::endl;
    return false;
  }
  if (biases->GetList().size() != classes.size()){
    std::cout<< "sizes don't match" << std::endl;
    return false;
  }
  for (unsigned long i = 0 ; i < biases->GetList().size();i++){
    const base::Value& this_bias = biases->GetList()[i];
    specified_biases.insert({classes[i], (float) this_bias.GetDouble()});
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
  std::cout<< "***READ VERSION: "<< version_ << std::endl;
  return true;
}

bool Pipeline::GetTimestampFromJSON(base::DictionaryValue* dictionary) {
  auto* timestamp_value = dictionary->FindKey("timestamp");
  if (!timestamp_value) {
    return false;
  }

  timestamp_ = timestamp_value->GetString();
  std::cout<< "***READ timestamp: "<< timestamp_ << std::endl;
  return true;
}

bool Pipeline::GetLocaleFromJSON(base::DictionaryValue* dictionary) {
  auto* locale_value = dictionary->FindKey("locale");
  if (!locale_value) {
    return false;
  }

  locale_ = locale_value->GetString();
  std::cout<< "READ locale: "<< locale_ << std::endl;
  return true;
}

std::map<std::string, float> Pipeline::Apply( Data_point inp) {
  Data_point last_point = Data_point(inp);
  for ( auto& transformation : transformations_) {
    last_point = transformation.get(last_point);
  }

  return classifier_.Predict(last_point);
}
std::vector<double> Pipeline::Get_Advertising_Predictions(std::string html){
  Data_point data = Data_point(html);
  auto predictions = Softmax(Apply(data));
  std::vector<double> rtn = {};
  for (unsigned long i = 0 ; i < advertising_categories_.size();i++){
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
