#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <functional>

#include "cmdparser.h"

// public
Cmdopt::Cmdopt(std::string shortopt, 
               std::string longopt, 
               std::string desc, 
               bool value_required) :
found{false},               
value_required{value_required},
shortopt{std::move(shortopt)},  
longopt{std::move(longopt)},  
desc{std::move(desc)} {

}

void Cmdparser::show_help() const {
    for (const Cmdopt& o : opts) {
        std::cout 
            << std::setw(15) << std::left 
            << o.shortopt + ", " + o.longopt 
            << o.desc 
            << std::endl;
    }
}

std::vector<std::string>& Cmdparser::get_nonopts() {
    return nonopts;
}

void Cmdparser::add(const std::string& shortopt, 
                    const std::string& longopt, 
                    const std::string& desc, 
                    bool value_required) {
    opts.emplace_back(shortopt, longopt, desc, value_required);
}

bool Cmdparser::parse(int argc, char* argv[]) {
    if (argc < 2) 
        return false;
    for (int i = 1; i < argc; ++i)
        parse_arg(argv[i]); 
    return true;
}

bool Cmdparser::found(const std::string& opt) const {
    auto it = std::find(opts.begin(), opts.end(), opt);
    return it != opts.end() && it->found;
}

bool Cmdparser::found(const std::string& opt, std::string& val) const {
    auto it = std::find(opts.begin(), opts.end(), opt); 
    if (it == opts.end() || !it->found)
        return false;
    val = it->val;
    return true;
}

// private
void Cmdparser::parse_opt(const std::string& opt) {
    assert(
        (opt[0] == '-' && opt[1] != '-') || 
        (opt[0] == '-' && opt[1] == '-' && opt[2] != '-')
    );
    auto i = opt.find('=');
    const auto& optname = opt.substr(0, i);
    auto it = std::find(opts.begin(), opts.end(), optname);
    if (it == opts.end()) {
        std::cerr << optname << ": unknown option." << std::endl;
        return;
    }
    if (i != std::string::npos) {

        if (!it->value_required) {
            std::cerr << optname << ": unexpected symbol '='." << std::endl;
            return;
        }
        if (i == opt.length()-1) {
            std::cerr << opt << ": value expected." << std::endl;
            return;
        }
        const auto& optval = opt.substr(i+1);
        it->val = optval;
    }
    else {
        if (it->value_required) {
            std::cerr << optname << ": value expected." << std::endl;
            return;
        }
    }
    it->found = true;
}

void Cmdparser::parse_arg(const std::string& arg) {
    assert(!arg.empty());
    switch (arg.find_first_not_of('-')) {
    // string like "-", "--", "-------"...
    case std::string::npos:
        std::cerr << arg << ": invalid argument." << std::endl;
        break;
    // not an option (string like "abc1")
    case 0:
        assert(arg[0] != '-');
        nonopts.emplace_back(arg);
        break;
    // short|long option
    case 1: case 2: 
        assert(
            (arg[0] == '-' && arg[1] != '-') || 
            (arg[0] == '-' && arg[1] == '-' && arg[2] != '-')
        );
        parse_opt(arg);
        break;
    // string like "---a"
    default:
        std::cerr << arg << ": invalid argument." << std::endl;
    }
}

bool operator==(const Cmdopt& a, const std::string& opt) {
    return a.shortopt == opt || a.longopt == opt;
}

