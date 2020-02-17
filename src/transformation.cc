#include "transformation.h"
#include <codecvt>
#include <math.h>
namespace usermodel {

Transformation::Transformation(){}

Transformation::Transformation(transformation_type t){
    type=t;
}
Transformation::~Transformation() = default;
transformation_type Transformation::getType(){
    return type;
}

Data_point Transformation::get(Data_point inp){
    if (type == TO_LOWER)
        return get_lower(inp);
    if (type == HASHED_NGRAMS)
        return get_ngrams(inp);
    if (type == NORMALIZE)
        return get_normalized(inp);
    return Data_point("");
}

Data_point Transformation::get_lower(Data_point data_point){
    std::string rtn_str;
    rtn_str.assign(data_point.data_text);
    std::transform(rtn_str.begin(), rtn_str.end(), rtn_str.begin(), ::tolower);
    return Data_point(rtn_str);
}

Data_point Transformation::get_ngrams(Data_point data_point){
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring widened_text = converter.from_bytes(data_point.data_text); 
    auto hashed_vector = hash_vectorizer.GetFrequencies(widened_text);
    return Data_point(hashed_vector,hash_vectorizer.get_buckets());
}

Data_point Transformation::get_normalized(Data_point data_point){
    // return an l2-normalized sparse datapoint
    auto s = 0.0;
    std::map<unsigned, float> normalized_vector;
    //sum over the squared values of the datapoint and calculate norm
    for (auto const& x : data_point.data_sparse){
        s += x.second*x.second ;
    }
    auto norm = sqrt(s);
    //divide original datapoint entries by the norm
    for (auto const& x : data_point.data_sparse){
        normalized_vector[x.first]= x.second / norm;
    }
    return Data_point(normalized_vector,hash_vectorizer.get_buckets());
}
///////////////////////////////////////////////////////////

To_lower::To_lower(){type = TO_LOWER;}

Normalize::Normalize(){type = NORMALIZE;}

///////////////////////////////////////////////////////////

Hashed_ngrams::Hashed_ngrams(){ 
    type = HASHED_NGRAMS;
    hash_vectorizer = HashVectorizer();
}        

Hashed_ngrams::Hashed_ngrams(int n_b, std::vector<int> subgrams){
    type = HASHED_NGRAMS;
    hash_vectorizer = HashVectorizer(n_b, subgrams);
}
Hashed_ngrams::Hashed_ngrams(std::string config){}


}