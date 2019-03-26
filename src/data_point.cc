#include "data_point.h"
#include <stdexcept>

namespace usermodel{
Data_point::~Data_point() = default;    
Data_point::Data_point(std::string data){
    type = text_data;
    data_text = data;
    n_dims=0;
}

Data_point::Data_point(std::vector<float> data){
    type = vector_data;
    data_vector = data;
    n_dims = data.size();
}

Data_point::Data_point(std::map<unsigned,float> data, int dims){
    type = sparse_vector;
    n_dims=dims;
    data_sparse = data;
}

float operator * (const Data_point a, const Data_point b){
    if ( (a.n_dims==0)||(b.n_dims==0))
        throw std::invalid_argument("One or more inputs has zero length");
    if (a.n_dims!=b.n_dims)
        throw std::invalid_argument("Inputs have incompatible lengths");
    float rtn = 0.0;
    if( (a.type == vector_data) && (b.type==vector_data)){
        for (auto i = 0; i < a.data_vector.size(); i++)
            rtn+= a.data_vector[i]*b.data_vector[i];
    }else if ((a.type==vector_data)&&(b.type==sparse_vector)){
        for (auto kv : b.data_sparse)
            rtn += a.data_vector[kv.first] * kv.second;
    }else if ((a.type==sparse_vector)&&(b.type==vector_data)){
        for (auto kv : a.data_sparse)
            rtn += b.data_vector[kv.first] * kv.second;
    }else
        for (auto kv : a.data_sparse)
            if ( b.data_sparse.count(kv.first)>0 ){
                // rtn += kv.second*b.data_sparse[kv.first];
                auto tmp = b.data_sparse.at(kv.first);
                rtn+=kv.second*tmp; 
            }
    return rtn;
}

}