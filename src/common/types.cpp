#include "common/types.hpp"

namespace chtl {

std::string toString(CodeSegmentType type) {
    switch (type) {
        // CHTL编译器处理的类型
        case CodeSegmentType::CHTL_CORE:
            return "CHTL_CORE";
        case CodeSegmentType::CHTL_TEMPLATE:
            return "CHTL_TEMPLATE";
        case CodeSegmentType::CHTL_CUSTOM:
            return "CHTL_CUSTOM";
        case CodeSegmentType::CHTL_CONFIGURATION:
            return "CHTL_CONFIGURATION";
        case CodeSegmentType::CHTL_NAMESPACE:
            return "CHTL_NAMESPACE";
        case CodeSegmentType::CHTL_IMPORT:
            return "CHTL_IMPORT";
        case CodeSegmentType::CHTL_TEXT:
            return "CHTL_TEXT";
            
        // CHTL JS编译器处理的类型
        case CodeSegmentType::CHTL_JS_SCRIPT:
            return "CHTL_JS_SCRIPT";
            
        // CSS编译器处理的类型
        case CodeSegmentType::CSS_GLOBAL:
            return "CSS_GLOBAL";
        case CodeSegmentType::CSS_LOCAL_STYLE:
            return "CSS_LOCAL_STYLE";
        case CodeSegmentType::CSS_ORIGIN:
            return "CSS_ORIGIN";
            
        // JavaScript编译器处理的类型
        case CodeSegmentType::JAVASCRIPT_STANDARD:
            return "JAVASCRIPT_STANDARD";
        case CodeSegmentType::JAVASCRIPT_ORIGIN:
            return "JAVASCRIPT_ORIGIN";
            
        // 原始嵌入处理
        case CodeSegmentType::ORIGIN_HTML:
            return "ORIGIN_HTML";
            
        // 其他
        case CodeSegmentType::COMMENT:
            return "COMMENT";
        case CodeSegmentType::UNKNOWN:
        default:
            return "UNKNOWN";
    }
}

CodeSegmentType parseCodeSegmentType(const std::string& str) {
    // CHTL编译器处理的类型
    if (str == "CHTL_CORE") return CodeSegmentType::CHTL_CORE;
    if (str == "CHTL_TEMPLATE") return CodeSegmentType::CHTL_TEMPLATE;
    if (str == "CHTL_CUSTOM") return CodeSegmentType::CHTL_CUSTOM;
    if (str == "CHTL_CONFIGURATION") return CodeSegmentType::CHTL_CONFIGURATION;
    if (str == "CHTL_NAMESPACE") return CodeSegmentType::CHTL_NAMESPACE;
    if (str == "CHTL_IMPORT") return CodeSegmentType::CHTL_IMPORT;
    if (str == "CHTL_TEXT") return CodeSegmentType::CHTL_TEXT;
    
    // CHTL JS编译器处理的类型
    if (str == "CHTL_JS_SCRIPT") return CodeSegmentType::CHTL_JS_SCRIPT;
    
    // CSS编译器处理的类型
    if (str == "CSS_GLOBAL") return CodeSegmentType::CSS_GLOBAL;
    if (str == "CSS_LOCAL_STYLE") return CodeSegmentType::CSS_LOCAL_STYLE;
    if (str == "CSS_ORIGIN") return CodeSegmentType::CSS_ORIGIN;
    
    // JavaScript编译器处理的类型
    if (str == "JAVASCRIPT_STANDARD") return CodeSegmentType::JAVASCRIPT_STANDARD;
    if (str == "JAVASCRIPT_ORIGIN") return CodeSegmentType::JAVASCRIPT_ORIGIN;
    
    // 原始嵌入处理
    if (str == "ORIGIN_HTML") return CodeSegmentType::ORIGIN_HTML;
    
    // 其他
    if (str == "COMMENT") return CodeSegmentType::COMMENT;
    return CodeSegmentType::UNKNOWN;
}

} // namespace chtl