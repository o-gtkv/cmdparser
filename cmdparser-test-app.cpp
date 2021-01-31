#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstdlib>

#include "cmdparser.h"

void sort(std::vector<std::string>& strings, const std::string& val) {
    int(*pred)(int); 
    if (val == "alpha1st") 
        pred = std::isalpha;
    else if (val == "digit1st") 
        pred = std::isdigit;
    else {
        std::cout << "Unknown value: " << val << std::endl;
        return;
    }
    for (auto& s : strings) 
        std::stable_partition(s.begin(), s.end(), pred);
    return;
}

int main(int argc, char* argv[]) {
    std::string val;
    Cmdparser p;
    p.add("-h", "--help", "Show this help and exit.");
    p.add("-u", "--upcase", "Upcase characters.");
    p.add("-s", "--sort", "Sort characters:\n"
                          "alpha1st - letters followed by digits\n" 
                          "digit1st - digits followed by latters\n", true);
    if (!p.parse(argc, argv)) {        
        std::cout 
            << "USAGE:\n" 
            << argv[0] << " " << "[OPTIONS...] <TEXT>" << std::endl;
        return EXIT_FAILURE;
    }
    if (p.found("-h")) {
        p.show_help();
        return EXIT_SUCCESS;
    }    
    auto& strings = p.get_nonopts();
    if (strings.empty()) {
        std::cerr << "Nothing to do." << std::endl;
        return EXIT_FAILURE;
    }    
    if (p.found("-u")) {
        for (auto& s : strings) {
            std::transform(
                s.begin(), s.end(), s.begin(),
                static_cast<int(*)(int)>(std::toupper)
            );
        }
    }
    if (p.found("-s", val))
        sort(strings, val);
    for (const auto& s : strings)
        std::cout << s << std::endl;
}

