#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "parse_descriptor.h"

/*
    Contains the user interface. A descriptor can be provided to the program by 
    providing the path to a text file  or  entering the descriptor directly.
*/

std::stringstream input_to_ss(const std::string raw_user_input)
{
    std::string raw_descriptor;
    std::ifstream ist {raw_user_input};
    if (ist)
    {
        ist>>raw_descriptor;
    } else {
        raw_descriptor = raw_user_input; 
    }
    std::stringstream ss{raw_descriptor};
    return ss;
}

int main()
{
    try {
        std::cout << "Please enter a descriptor directly or enter the path to a file containing a descriptor: "; 
        std::string raw_user_input;
        std::cin>>raw_user_input;
        const Descriptor descriptor = ss_to_descriptor(input_to_ss(raw_user_input));
		print_descriptor_info(descriptor);
    } catch (std::runtime_error& error) {
        std::cerr << error.what() << '\n';
    } catch (...) {
        std::cerr << "An unknown error occured" << '\n';
    }
    return 0;
}

