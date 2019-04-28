#ifndef PIPELINE_NODELET_2__HELPER_
#define PIPELINE_NODELET_2__HELPER_

#include <chrono>

namespace helper{
    
    inline void print_time_stamp(std::string comment="timestamp"){
	std::cout<< comment<<" "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()<<"\t";
    };


}

#endif
