#include "data_point.h"
#include <iostream>
#include <cmath>
int main(){
    //let's create some test data: 
    auto EPS=0.000001;
    //a text datapoint
    std::string a = "trololol";
    auto string_data_point = usermodel::Data_point(a);
    assert(string_data_point.type == usermodel::text_data);
    //some dense vectors:
    std::vector<float> v_5{1.0,2.0,3.0,4.0,5.0};
    auto dense_datapoint_5 = usermodel::Data_point(v_5);
    std::vector<float> v_3{1.0,2.0,3.0};
    auto dense_datapoint_3 = usermodel::Data_point(v_3);
    std::vector<float> v_3_1{1.0, 1.0, 1.0};
    auto dense_datapoint_3_1 = usermodel::Data_point(v_3_1);
    // some sparse vectors:
    std::map<unsigned,float> s_3 = {{0,1.0}, {2,2.0}};// dense equivalent:[1,0,2]
    std::map<unsigned,float> s_5 = {{0,1.0}, {2,3.0}, {3, -2.0}};// dense equivalent:[1,0,3,2,0]
    auto sparse_datapoint_3=usermodel::Data_point(s_3,3);
    auto sparse_datapoint_5=usermodel::Data_point(s_5,5);
    
    // Let's test properties of dense vectors
    assert(dense_datapoint_5.type==usermodel::vector_data);
    assert(dense_datapoint_3.type==usermodel::vector_data);
    assert(dense_datapoint_3_1.type==usermodel::vector_data);
    auto rez_3x3 = dense_datapoint_3*dense_datapoint_3; // = 14
    auto rez_3x1 = dense_datapoint_3*dense_datapoint_3_1;// = 6
    auto rez_5x5 = dense_datapoint_5*dense_datapoint_5;// = 55
    assert( std::abs(6-rez_3x1)<EPS );
    assert(std::abs(14-rez_3x3)<EPS);
    assert(std::abs(55-rez_5x5)<EPS);
    
    // let's test properties of sparse vectors
    assert(sparse_datapoint_3.type == usermodel::sparse_vector);
    assert(sparse_datapoint_5.type == usermodel::sparse_vector);
    auto s_rez_3x3 = sparse_datapoint_3*sparse_datapoint_3; // = 5
    auto s_rez_5x5 = sparse_datapoint_5*sparse_datapoint_5; // = 14
    assert(std::abs(5-s_rez_3x3)<EPS);
    assert(std::abs(14-s_rez_5x5)<EPS);

    // now let's test dense-sparse , and multiplication order:
    auto mixed_rez_3x3_1 = dense_datapoint_3*sparse_datapoint_3; // = 7
    auto mixed_rez_5x5_1 = dense_datapoint_5*sparse_datapoint_5; // = 2
    auto mixed_rez_3x3_2 = sparse_datapoint_3*dense_datapoint_3; // = 7
    auto mixed_rez_5x5_2 = sparse_datapoint_5*dense_datapoint_5; // = 2
    assert( std::abs(mixed_rez_3x3_1-mixed_rez_3x3_2) < EPS);
    assert( std::abs(mixed_rez_5x5_1-mixed_rez_5x5_2) < EPS);
    assert( std::abs(7 - mixed_rez_3x3_1) < EPS);
    assert( std::abs(2 - mixed_rez_5x5_2) < EPS);
    
    // finally let's test the exceptions when doing stupid things
    try{
        auto wrong_1 = dense_datapoint_5*dense_datapoint_3;
    }catch (std::exception &e){
        std::cout << e.what() <<  dense_datapoint_5.n_dims << " / "<<dense_datapoint_3.n_dims<<'\n';
    }
    try{
        auto wrong_2 = dense_datapoint_5*sparse_datapoint_3;
    }catch (std::exception &e){
        std::cout << e.what() <<  dense_datapoint_5.n_dims << " / "<<sparse_datapoint_3.n_dims<<'\n';
    }
    try{
        auto wrong_2 = string_datapoint_5*sparse_datapoint_3;
    }catch (std::exception &e){
        std::cout << e.what() <<  dense_datapoint_5.n_dims << " / "<<sparse_datapoint_3.n_dims<<'\n';
    }

    
    return 0;
}