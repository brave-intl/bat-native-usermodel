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
    Pipeline::Pipeline(std::vector<Transformation> representation,  Linear_classifier c){
        transformations = representation;
        classifier = c;
    }
    std::map<std::string, float> Pipeline::apply(Data_point &inp){
        Data_point last_point = Data_point(inp);
        for (unsigned i = 0; i < transformations.size(); i++){
            last_point = transformations[i].get(last_point);
            std::cout << "after loop:  " << i << " type is: "<< last_point.type<< '\n';
        }
        std::cout << "after final loop type is: "<< last_point.type<< '\n';
        if (last_point.type == sparse_vector){
            std::cout << "and dimensionality is: " << last_point.n_dims<<'\n';
            std::cout <<"while c++ dims are:"<< last_point.data_sparse.size()<<'\n';
        } 
        return classifier.Predict(last_point);

    }

}