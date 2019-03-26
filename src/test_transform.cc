#include "transformation.h"
int main(){
    std::cout << "Testing transformations"<<'\n';
    std::string upper_case = "LOWER CASE";
    std::string lower_case = "lower case";
    auto upper_datapoint = usermodel::Data_point(upper_case);
    usermodel::To_lower to_lower;
    if (to_lower.apply(upper_datapoint) ){
        auto lower_datapoint=to_lower.get();
        assert(lower_datapoint.type==0);
        assert( lower_case.compare(lower_datapoint.data_text)==0 );
    }
    return 0;
}