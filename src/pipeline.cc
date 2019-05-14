#include <iostream> 
#include "pipeline.h"
#include <list>

namespace usermodel{
    Pipeline::Pipeline(){}
    Pipeline::~Pipeline() = default;
    Pipeline::Pipeline(Pipeline &other){
        transformations = other.transformations;
        classifier = other.classifier;
    }
    Pipeline::Pipeline(std::list<Transformation> representation,  Classifier c){
        transformations = representation;
        classifier = c;
    }
    Pipeline::Pipeline(std::list<Transformation> representation){
        transformations = representation;
    }
    std::map<std::string, float> Pipeline::apply(Data_point &inp){
        Data_point last_point = Data_point(inp);
          for (std::list<Transformation>::iterator it = transformations.begin(); it != transformations.end(); ++it){
            //   auto tmp = it->apply(last_point);
            //   if (tmp){
            //     last_point = Data_point(it->get());
            //   }          
            it->apply(last_point);
            last_point = Data_point(it->get());
          }
        return classifier.Predict(last_point);

    }
    // Pipeline(std::string json_spec);
    
    // std::map<std::string, float> apply(Data_point &inp);

}