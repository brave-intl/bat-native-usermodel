/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef DATA_POINT_H
#define DATA_POINT_H
#include <map>
#include <vector>
#include <iostream>
namespace usermodel{

enum data_type{
    text_data = 0,
    vector_data = 1,
    sparse_vector = 2 
};

class Data_point {
    public:
        Data_point(const Data_point &other_point);
        Data_point(std::string data);
        Data_point(std::vector<float> data);
        Data_point(std::map<unsigned,float> data, int ndims);
        ~Data_point();
        friend float operator * (Data_point a,Data_point b);
        // friend 
        data_type get_type();
        int get_dims();
        data_type type;
        int n_dims;
        std::string data_text;
        std::vector<float> data_vector;
        std::map<unsigned,float> data_sparse;
};

// float inner_product(Data_point a, Data_point b);
// float sum(Data_point a, Data_point b);

}

#endif