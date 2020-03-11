/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#include <iostream>
#include "data_point.h"
#include <vector>
namespace usermodel{

std::map<std::string, float> Softmax( std::map<std::string, float> y);

class Linear_classifier{
    public:
        Linear_classifier();
        Linear_classifier(Linear_classifier &other);
        Linear_classifier(std::string model);
        Linear_classifier(std::map<std::string, DataPoint> weights_, std::map<std::string,float> biases_);
        ~Linear_classifier();
        std::map<std::string, float> Predict(DataPoint x);
    private: 
        std::map<std::string, DataPoint> weights;
        std::map<std::string,float> biases;
};
}