#include <iostream>
#include <fstream>
#include <string>
#include "pipeline.pb.h"
int main(){
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        pipelines::Pipeline pipeline;
        std::fstream input("test_model.pb", std::ios::in | std::ios::binary);
        if (!pipeline.ParseFromIstream(&input)) {
            std::cerr << "Failed to parse address book." << '\n';
        return -1;
        }
        std::cout << "Successfully read pipeline for: " 
                << pipeline.language() 
                << " created at: "
                << pipeline.timestamp()
                << " version " 
                << pipeline.version()
                << '\n';
        auto representations = pipeline.representation();
        std::cout << "representations: " << representations.size();
        return 0;

}
