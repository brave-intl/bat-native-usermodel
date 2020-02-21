#include "transformation.h"
#include <codecvt>
#include <math.h>
namespace usermodel {

Transformation::Transformation(){}

Transformation::Transformation(TransformationType t){
    type=t;
}
Transformation::~Transformation() = default;
TransformationType Transformation::getType(){
    return type;
}

DataPoint Transformation::get(DataPoint inp){
    switch(type){
        case TransformationType::TO_LOWER:
            return get_lower(inp);
        case TransformationType::HASHED_NGRAMS:
            return get_ngrams(inp);
        case TransformationType::NORMALIZE:
            return get_normalized(inp);
        default:
            return DataPoint("");
    }
}

DataPoint Transformation::get_lower(DataPoint datapoint){
    std::string rtn_str;
    rtn_str.assign(datapoint.data_text);
    std::transform(rtn_str.begin(), rtn_str.end(), rtn_str.begin(), ::tolower);
    return DataPoint(rtn_str);

}

DataPoint Transformation::get_ngrams(DataPoint datapoint){
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring widened_text = converter.from_bytes(datapoint.data_text); 
    auto hashed_vector = hash_vectorizer.GetFrequencies(widened_text);
    return DataPoint(hashed_vector,hash_vectorizer.get_buckets());
}

DataPoint Transformation::get_normalized(DataPoint datapoint){
    // return an l2-normalized sparse datapoint
    auto s = 0.0;
    std::map<unsigned, float> normalized_vector;
    //sum over the squared values of the datapoint and calculate norm
    for (auto const& x : datapoint.data_sparse){
        s += x.second*x.second ;
    }
    auto norm = sqrt(s);
    //divide original datapoint entries by the norm
    for (auto const& x : datapoint.data_sparse){
        normalized_vector[x.first]= x.second / norm;
    }
    return DataPoint(normalized_vector,hash_vectorizer.get_buckets());
}
///////////////////////////////////////////////////////////

To_lower::To_lower(){type = TransformationType::TO_LOWER;}

Normalize::Normalize(){type = TransformationType::NORMALIZE;}

///////////////////////////////////////////////////////////

Hashed_ngrams::Hashed_ngrams(){ 
    type = TransformationType::HASHED_NGRAMS;
    hash_vectorizer = HashVectorizer();
}        

Hashed_ngrams::Hashed_ngrams(int n_b, std::vector<int> subgrams){
    type = TransformationType::HASHED_NGRAMS;
    hash_vectorizer = HashVectorizer(n_b, subgrams);
}
Hashed_ngrams::Hashed_ngrams(std::string config){}


}