#include <iostream> 
#include <vector>
#include <map>
#include "data_point.h"
#include "transformation.h"
#include "classifier.h"

namespace usermodel{

class Pipeline{
    public:
        Pipeline();
        Pipeline(Pipeline &other);
        Pipeline(std::vector<Transformation> representation,  Linear_classifier c);
        ~Pipeline();
        bool from_json(std::string pipeline_json);
        std::map<std::string, float> apply(Data_point &inp);
    private:
        std::vector<Transformation> transformations;
        Linear_classifier classifier;
};
Pipeline pipeline_from_json
}