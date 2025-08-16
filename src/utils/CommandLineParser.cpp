#include "CommandLineParser.hpp"

CommandLineParser::CommandLineParser(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg.substr(0, 2) == "--") {
            // 长选项
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                options_[arg] = argv[++i];
            } else {
                flags_.push_back(arg);
            }
        } else if (arg.substr(0, 1) == "-") {
            // 短选项
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                options_[arg] = argv[++i];
            } else {
                flags_.push_back(arg);
            }
        } else {
            // 位置参数
            positionalArgs_.push_back(arg);
        }
    }
}

bool CommandLineParser::hasFlag(const std::string& flag) const {
    return std::find(flags_.begin(), flags_.end(), flag) != flags_.end();
}

bool CommandLineParser::hasOption(const std::string& option) const {
    return options_.find(option) != options_.end();
}

std::string CommandLineParser::getOption(const std::string& option, const std::string& defaultValue) const {
    auto it = options_.find(option);
    return (it != options_.end()) ? it->second : defaultValue;
}

std::vector<std::string> CommandLineParser::getPositionalArgs() const {
    return positionalArgs_;
}