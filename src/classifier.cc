/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "classifier.h"
#include <cmath>
#include <limits>

namespace usermodel{

std::map<std::string, float> Softmax( const std::map<std::string, float> &y){
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

Linear_classifier::Linear_classifier(const std::map<std::string, DataPoint> &weights, const std::map<std::string,float> &biases){
    weights_ = weights;
    biases_ = biases;
}
Linear_classifier::Linear_classifier(Linear_classifier &other){
    weights_ = other.weights_;
    biases_ = other.biases_;
}
Linear_classifier::~Linear_classifier()=default;
std::map<std::string, float> Linear_classifier::Predict(DataPoint x){
    std::map<std::string, float> rtn;
    for (auto kv: weights_){
        rtn[kv.first] = kv.second*x;
        rtn[kv.first] += biases_[kv.first];
    }
    return rtn;
}

}