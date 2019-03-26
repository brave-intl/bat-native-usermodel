#include <iostream>
#include "data_point.h"

namespace usermodel{

class Classifier{
    public:
    Classifier();
    Classifier(std::string model);
    ~Classifier();
    std::map<std::string, float> Predict(Data_point x);
};

}