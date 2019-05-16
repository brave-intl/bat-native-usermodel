#include <iostream> 
#include "pipeline.h"
#include <list>
#include "base/json/json_reader.h"

namespace usermodel{
    Pipeline::Pipeline(){}
    Pipeline::~Pipeline() = default;
    Pipeline::Pipeline(Pipeline &other){
        transformations = other.transformations;
        classifier = other.classifier;
    }
    Pipeline::Pipeline(std::vector<Transformation> representation,  Linear_classifier c){
        transformations = representation;
        classifier = c;
    }
    bool Pipeline::from_json(std::string pipeline_json){
        base::Optional<base::Value> value = base::JSONReader::Read(pipeline_json);
        if (!value || !value->is_dict()) {
            //TODO: Integrate logging api BLOG(ERROR) << "Failed to parse JSON: " << json;
            return false;
        }
        base::DictionaryValue* dictionary = nullptr;
        if (!value->GetAsDictionary(&dictionary)) {
            //BLOG(ERROR) << "Failed to get dictionary: " << json;
            return false;
        }
        return true;
    }
    std::map<std::string, float> Pipeline::apply(Data_point &inp){
        Data_point last_point = Data_point(inp);
        for (unsigned i = 0; i < transformations.size(); i++){
            last_point = transformations[i].get(last_point);
        }
        return classifier.Predict(last_point);

    }

}