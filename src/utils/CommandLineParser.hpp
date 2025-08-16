#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

class CommandLineParser {
public:
    CommandLineParser(int argc, char* argv[]);
    
    bool hasFlag(const std::string& flag) const;
    bool hasOption(const std::string& option) const;
    std::string getOption(const std::string& option, const std::string& defaultValue = "") const;
    std::vector<std::string> getPositionalArgs() const;
    
private:
    std::unordered_map<std::string, std::string> options_;
    std::vector<std::string> positionalArgs_;
    std::vector<std::string> flags_;
};