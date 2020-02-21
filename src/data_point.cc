#include "data_point.h"
#include <stdexcept>
#include <limits>

namespace usermodel{
Data_point::Data_point(const Data_point &other_point){
    type = other_point.type;
    data_text = other_point.data_text;
    data_sparse = other_point.data_sparse;
    data_vector = other_point.data_vector;
    n_dims = other_point.n_dims;
    type = other_point.type;
}
Data_point::~Data_point() = default;    
Data_point::Data_point(const std::string &data){
    type = data_type::text_data;
    data_text = data;
    n_dims=0;
}

Data_point::Data_point(const std::vector<float> &data){
    type = data_type::vector_data;
    data_vector = data;
    n_dims = data.size();
}

Data_point::Data_point(const std::map<unsigned,float> &data, int dims){
    type = data_type::sparse_vector;
    n_dims=dims;
    data_sparse = data;
}

float operator * (const Data_point a, const Data_point b){
    if ( (a.n_dims==0)||(b.n_dims==0)){
        // throw std::invalid_argument("One or more inputs has zero length");
        return std::numeric_limits<double>::quiet_NaN();
    }
        
    if (a.n_dims!=b.n_dims){
        // throw std::invalid_argument("Inputs have incompatible lengths");
        return std::numeric_limits<double>::quiet_NaN();
    }

    float rtn = 0.0;
    if( (a.type == data_type::vector_data) && (b.type==data_type::vector_data)){
        for (unsigned i = 0; i < a.data_vector.size(); i++)
            rtn+= a.data_vector[i]*b.data_vector[i];
    }else if ((a.type==data_type::vector_data)&&(b.type==data_type::sparse_vector)){
        for (auto kv : b.data_sparse)
            rtn += a.data_vector[kv.first] * kv.second;
    }else if ((a.type==data_type::sparse_vector)&&(b.type==data_type::vector_data)){
        for (auto kv : a.data_sparse)
            rtn += b.data_vector[kv.first] * kv.second;
    }else{
        for (auto kv : a.data_sparse)
            if ( b.data_sparse.count(kv.first)>0 ){
                // rtn += kv.second*b.data_sparse[kv.first];
                auto tmp = b.data_sparse.at(kv.first);
                rtn+=kv.second*tmp; 
            }
    }
    return rtn;
}

}