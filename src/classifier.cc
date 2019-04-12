#include <iostream>
#include "classifier.h"

namespace usermodel{

Classifier::Classifier(){}
Classifier::~Classifier()= default;
std::map<std::string, float> Classifier::Predict(Data_point x){
    std::map<std::string,float> rtn = {{"N/A",0.0}};
    return rtn;
}

Linear_classifier::Linear_classifier(){}
Linear_classifier::Linear_classifier(std::map<std::string, Data_point> weights_, std::map<std::string,float> biases_){
    weights = weights_;
    biases = biases_;
}
Linear_classifier::~Linear_classifier()=default;
std::map<std::string, float> Linear_classifier::Predict(Data_point x){
    std::map<std::string, float> rtn;
    for (auto kv: weights){
        rtn[kv.first] = kv.second*x;
        rtn[kv.first] += biases[kv.first];
    }
    return rtn;
}

}