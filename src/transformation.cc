#include "transformation.h"
#include <codecvt>
namespace usermodel {

Transformation::Transformation(){}
Transformation::Transformation(data_type type){
    transformation_type=type;
}
Transformation::~Transformation() = default;

To_lower::To_lower(){Transformation{text_data};}

Data_point Transformation::get(){
    return *output;
}
void Transformation::transform(Data_point inp){}

bool Transformation::apply(Data_point &data_point){
    if (transformation_type!=data_point.type)
        return false;
    else
        transform(data_point);
    return true;
}
bool To_lower::apply(Data_point &data_point){
    auto tmp = Transformation::apply(data_point);
    transform(data_point);
    return tmp;
}
void To_lower::transform(Data_point inp){
    std::string rtn_str;
    rtn_str.assign(inp.data_text);
    std::transform(rtn_str.begin(), rtn_str.end(), rtn_str.begin(), ::tolower);
    output = new usermodel::Data_point(rtn_str);
}

Hashed_ngrams::Hashed_ngrams(){}
Hashed_ngrams::Hashed_ngrams(std::string config){
    //TODO: Config json parsing
}

bool Hashed_ngrams::apply(Data_point &data_point){
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring widened_text = converter.from_bytes(data_point.data_text); 
    bool rtn =  vectorizer.Process(widened_text);
    auto hashed_vector = vectorizer.GetFrequencies();
    output = new Data_point(hashed_vector,1234);
    return rtn;
}
// Data_point Hashed_ngrams::get(){
//     auto vector_data = vectorizer.GetFrequencies();
//     auto tmp = Data_point(vector_data,1234); 
//     return 
// }

}