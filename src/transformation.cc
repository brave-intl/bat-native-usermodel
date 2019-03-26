#include "transformation.h"

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

}