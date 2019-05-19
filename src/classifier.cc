#include <iostream>
#include "classifier.h"
#include <cmath>
#include <limits>

namespace usermodel{

std::map<std::string, float> Softmax( std::map<std::string, float> y){
    //first pass get minimum
    float maximum = -std::numeric_limits<float>::infinity();
    for (auto const& x : y){
        if (x.second>maximum){
            maximum = x.second;
        }
    }
    std::map<std::string, float> rtn;
    float sum_exp = 0.0;
    //second pass get sum_exp and partial exponentials
    for (auto const& x : y){
        auto tmp = x.second - maximum;
        rtn[x.first] = std::exp(tmp);
        sum_exp += rtn[x.first];           
    }
    //third pass: divide by sum_exp
    for (auto const& x : rtn){
        rtn[x.first] = rtn[x.first]/sum_exp;       
    }

    return rtn;
}
Linear_classifier::Linear_classifier(){}

Linear_classifier::Linear_classifier(std::map<std::string, Data_point> weights_, std::map<std::string,float> biases_){
    weights = weights_;
    biases = biases_;
}
Linear_classifier::Linear_classifier(Linear_classifier &other){
    weights = other.weights;
    biases = other.biases;
}
Linear_classifier::~Linear_classifier()=default;
std::map<std::string, float> Linear_classifier::Predict(Data_point x){
    std::cout<< "*** IN classifier classify"<<"\n";
    std::map<std::string, float> rtn;
    for (auto kv: weights){
        rtn[kv.first] = kv.second*x;
        rtn[kv.first] += biases[kv.first];
        std::cout<< "***category: "<<kv.first << " : "<< rtn[kv.first]<<"\n"; 
    }
    std::cout<< "*** OUT classifier classify"<<"\n";
    return rtn;
}

}