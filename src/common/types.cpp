#include "common/types.hpp"

namespace chtl {

std::string toString(CodeSegmentType type) {
    switch (type) {
        case CodeSegmentType::CHTL:
            return "CHTL";
        case CodeSegmentType::CHTL_JS:
            return "CHTL_JS";
        case CodeSegmentType::CSS:
            return "CSS";
        case CodeSegmentType::JAVASCRIPT:
            return "JAVASCRIPT";
        case CodeSegmentType::LOCAL_STYLE:
            return "LOCAL_STYLE";
        case CodeSegmentType::LOCAL_SCRIPT:
            return "LOCAL_SCRIPT";
        case CodeSegmentType::TEMPLATE:
            return "TEMPLATE";
        case CodeSegmentType::CUSTOM:
            return "CUSTOM";
        case CodeSegmentType::IMPORT:
            return "IMPORT";
        case CodeSegmentType::ORIGIN:
            return "ORIGIN";
        case CodeSegmentType::CONFIGURATION:
            return "CONFIGURATION";
        case CodeSegmentType::NAMESPACE:
            return "NAMESPACE";
        case CodeSegmentType::COMMENT:
            return "COMMENT";
        case CodeSegmentType::TEXT:
            return "TEXT";
        case CodeSegmentType::UNKNOWN:
        default:
            return "UNKNOWN";
    }
}

CodeSegmentType parseCodeSegmentType(const std::string& str) {
    if (str == "CHTL") return CodeSegmentType::CHTL;
    if (str == "CHTL_JS") return CodeSegmentType::CHTL_JS;
    if (str == "CSS") return CodeSegmentType::CSS;
    if (str == "JAVASCRIPT") return CodeSegmentType::JAVASCRIPT;
    if (str == "LOCAL_STYLE") return CodeSegmentType::LOCAL_STYLE;
    if (str == "LOCAL_SCRIPT") return CodeSegmentType::LOCAL_SCRIPT;
    if (str == "TEMPLATE") return CodeSegmentType::TEMPLATE;
    if (str == "CUSTOM") return CodeSegmentType::CUSTOM;
    if (str == "IMPORT") return CodeSegmentType::IMPORT;
    if (str == "ORIGIN") return CodeSegmentType::ORIGIN;
    if (str == "CONFIGURATION") return CodeSegmentType::CONFIGURATION;
    if (str == "NAMESPACE") return CodeSegmentType::NAMESPACE;
    if (str == "COMMENT") return CodeSegmentType::COMMENT;
    if (str == "TEXT") return CodeSegmentType::TEXT;
    return CodeSegmentType::UNKNOWN;
}

} // namespace chtl