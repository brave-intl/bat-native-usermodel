/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "pipeline.h"

#include "base/json/json_reader.h"

namespace usermodel {

Pipeline::Pipeline() :
    version_(0),
    timestamp(""),
    locale("en"),
    transformations_({}),
    classifier_(nullptr) {
}

Pipeline::Pipeline(const Pipeline &pipeline) {
  transformations_ = pipeline.transformations;
  classifier_ = pipeline.classifier;
}

Pipeline::Pipeline(
    std::vector<Transformation> transformations,
    Linear_classifier classifier) {
  transformations_ = transformations;
  classifier_ = classifier;
}

Pipeline::~Pipeline() = default;

bool Pipeline::FromJson(const std::string& json) {
  base::Optional<base::Value> value = base::JSONReader::Read(json);
  if (!value || !value->is_dict()) {
    return false;
  }

  base::DictionaryValue* dictionary = nullptr;
  if (!value->GetAsDictionary(&dictionary)) {
    return false;
  }

  if (!GetVersionFromJSON(dictionary)) {
    return false;
  }

  if (!GetTimestampFromJSON(dictionary)) {
    return false;
  }

  if (!GetLocaleFromJSON(dictionary)) {
    return false;
  }

  if (!GetTransformationsFromJSON(dictionary)) {
    return false;
  }

  if (!GetClassifierFromJSON(dictionary)) {
    return false;
  }

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

bool Pipeline::GetTransformationsFromJSON(base::DictionaryValue* dictionary) {
  auto* transformations_value = dictionary->FindKey("transformations");
  if (!transformations_value) {
    return false;
  }

  base::ListValue transformations_values(transformations_value->GetList());

  transformations_->SetTransformationsFromList(transformations_values);

  return true;
}

void Pipeline::SetTransformationsFromList(const base::Value& list) {
  base::ListValue list_values(list.GetList());

  std::vector<Transformation> transformations;
  for (const auto& value : list_values) {
    base::DictionaryValue* dictionary;
    if (!value.GetAsDictionary(&dictionary)) {
      continue;
    }

    // Transformation type
    auto* transformation_type_value =
        dictionary->FindKey("transformation_type");
    if (!transformation_type_value) {
      continue;
    }

    auto transformation_type = transformation_type_value->GetString();
    Transformation transformation(transformation_type);

    // Parameters
    auto* parameters_value = dictionary->FindKey("params");
    if (parameters_value) {
      base::DictionaryValue* parameters_dictionary;
      if (!parameters_value->GetAsDictionary(&parameters_dictionary)) {
        return false;
      }

      // Range of ngrams
      auto* ngrams_range_value = parameters_dictionary->FindKey("ngrams_range");
      if (ngrams_range_value) {
        std::vector<int> ngrams_range;
        base::ListValue ngrams_range_values(ngrams_range_value->GetList());
        for (const auto& value : ngrams_range_values) {
          auto ngram = value.GetInt();
          ngrams_range.push_back(ngram);
        }

        transformation.hash_vectorizer.set_ngrams_range(ngrams_range);
      }

      // Number of buckets
      auto* num_buckets_value = parameters_dictionary->FindKey("num_buckets");
      if (num_buckets_value) {
        auto num_buckets = num_buckets_value->GetInt();
        transformation.hash_vectorizer.set_buckets(num_buckets);
      }
    }

    transformations.push_back(transformation);
  }

  transformations_ = transformations;
}

bool Pipeline::GetClassifierFromJSON(base::DictionaryValue* dictionary) {
  auto* classifier_value = dictionary->FindKey("classifier");
  if (!classifier_value) {
    return false;
  }

  // Classifier type
  auto* classifier_type_value = dictionary->FindKey("classifier_type");
  if (!classifier_type_value) {
    return false;
  }

  auto classifier_type = classifier_type_value->GetString();

  // Classes
  auto* classes_value = dictionary->FindKey("classes");
  if (!classes_value) {
    return false;
  }

  std::vector<std::string> classes;
  base::ListValue classes_values(classes_value->GetList());
  for (const auto& value : classes_values) {
    auto class = value.GetString();
    classes.push_back(class);
  }

  // Class weights
  auto* class_weights_value = dictionary->FindKey("class_weights");
  if (!class_weights_value) {
    return false;
  }

  base::DictionaryValue* class_weights_dictionary;
  if (!class_weights_value->GetAsDictionary(&class_weights_dictionary)) {
    return false;
  }

  int class_count = 0;
  for (const auto& class : classes) {
    auto* classes_value = class_weights_dictionary->FindKey(class);
    if (!classes_value) {
      return false;
    }

    // ...

    class_count++;
  }

  if (classes.size() != class_count) {
    return false;
  }

  return true;
}

std::map<std::string, float> Pipeline::Apply(const Data_point &inp) {
  Data_point last_point = Data_point(inp);
  for (const auto& transformation : transformations_) {
    last_point = transformation.get(last_point);
  }

  return classifier.Predict(last_point);
}

}  // namespace usermodel
