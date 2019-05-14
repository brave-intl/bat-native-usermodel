#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H
#include <iostream> 
#include "data_point.h"
#include "hashing_extractor.h"
namespace usermodel{

enum transformation_type{
    TO_LOWER = 1,
    HASHED_NGRAMS=2
};

class Transformation{
    public:
        Transformation();
        Transformation(data_type type);
        Transformation(std::string parameters);
        ~Transformation();
        bool apply(Data_point &inp);
        Data_point get();
    protected:
        void transform(Data_point inp);
        data_type transformation_type;
        Data_point *input, *output;
};

class To_lower: public Transformation{
    public:
        To_lower();
        bool apply(Data_point &inp);
    private:
        void transform(Data_point inp);
};

class Hashed_ngrams: public Transformation{
    public:
        Hashed_ngrams();
        Hashed_ngrams(std::string parameters);
        bool apply(Data_point &inp);
    private:
        HashVectorizer vectorizer;
        void transform(Data_point inp);
};

}
#endif