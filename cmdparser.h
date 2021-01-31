#ifndef CMDPARSER_H
#define CMDPARSER_H

#include <vector>
#include <string>

struct Cmdopt {
    bool found, value_required;
    std::string shortopt, longopt, desc, val;
    Cmdopt(std::string shortopt, 
           std::string longopt, 
           std::string desc, 
           bool value_required);
};

class Cmdparser {
public:
    void show_help() const;
    std::vector<std::string>& get_nonopts();
    bool parse(int argc, char* argv[]);
    bool found(const std::string& opt) const;
    bool found(const std::string& opt, std::string& val) const;
    void add(const std::string& shortopt, 
             const std::string& longopt, 
             const std::string& desc, 
             bool value_required = false); 
private:
    std::vector<std::string> nonopts;
    std::vector<Cmdopt>      opts;
    void parse_opt(const std::string& opt); 
    void parse_arg(const std::string& arg);
};

bool operator==(const Cmdopt& a, const std::string& opt);

#endif
