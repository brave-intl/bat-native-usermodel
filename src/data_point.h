/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#ifndef DATAPOINT_H
#define DATAPOINT_H
#include <map>
#include <vector>
#include <iostream>
namespace usermodel{

enum class DataType{
    TextData = 0,
    VectorData = 1,
    SparseVector = 2 
};

class DataPoint {
    public:
        DataPoint(const DataPoint &other_point);
        DataPoint(const std::string &data);
        DataPoint(const std::vector<float> &data);
        DataPoint(const std::map<unsigned,float> &data, int ndims);
        ~DataPoint();
        friend float operator * (const DataPoint a, const DataPoint b);
        // friend 
        DataType get_type();
        int get_dims();
        DataType type;
        int n_dims;
        std::string data_text;
        std::vector<float> data_vector;
        std::map<unsigned,float> data_sparse;
};



}

#endif