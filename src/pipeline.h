#include <iostream> 
#include <list>
#include <map>
#include "data_point.h"
#include "transformation.h"
#include "classifier.h"

namespace usermodel{

class Pipeline{
    public:
        Pipeline();
        Pipeline(Pipeline &other);
        Pipeline(std::list<Transformation> representation,  Classifier c);
        ~Pipeline();
        Pipeline(std::list<Transformation> representation);
        // Pipeline(std::string json_spec);
        // Pipeline() --> protobuf equiv
        std::map<std::string, float> apply(Data_point &inp);
    private:
        std::list<Transformation> transformations;
        Classifier classifier;
};

}