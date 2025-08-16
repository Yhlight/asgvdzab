#pragma once

#include <string>
#include <vector>
#include <unordered_set>

namespace chtl {

/**
 * 原始嵌入块信息
 */
struct RawEmbeddingBlock {
    std::string type;           // 类型，如 "Html", "Vue", "React"
    std::string name;           // 名称（可选）
    std::string content;        // 内容
    size_t startPos;           // 开始位置
    size_t endPos;             // 结束位置
    bool isReference;          // 是否为引用（以;结尾）
    
    RawEmbeddingBlock() : startPos(0), endPos(0), isReference(false) {}
};

/**
 * 原始嵌入解析器状态
 */
enum class ParserState {
    NORMAL,                    // 正常状态
    ORIGIN_BRACKET,           // 遇到 [Origin]
    ORIGIN_TYPE,              // 解析 @Type
    ORIGIN_NAME,              // 解析可选名称
    ORIGIN_CONTENT,           // 解析内容块
    ORIGIN_REFERENCE          // 原始嵌入引用
};

/**
 * 基于状态机的原始嵌入解析器
 * 
 * 正确处理原始嵌入的边界，避免误匹配其他内容
 */
class RawEmbeddingParser {
public:
    RawEmbeddingParser();
    
    /**
     * 解析文本中的所有原始嵌入块
     * @param text 要解析的文本
     * @return 原始嵌入块列表
     */
    std::vector<RawEmbeddingBlock> parseRawEmbeddings(const std::string& text);
    
    /**
     * 移除文本中的所有原始嵌入块，用于约束检查
     * @param text 原始文本
     * @return 移除原始嵌入后的文本
     */
    std::string removeRawEmbeddingBlocks(const std::string& text);
    
    /**
     * 检查字符串是否为有效的原始嵌入
     * @param text 要检查的文本
     * @return 是否为有效的原始嵌入
     */
    bool isValidRawEmbedding(const std::string& text);
    
    /**
     * 获取所有找到的原始嵌入类型
     * @return 类型集合
     */
    std::unordered_set<std::string> getFoundTypes() const { return foundTypes_; }

private:
    std::unordered_set<std::string> foundTypes_;
    
    /**
     * 跳过注释和字符串内容
     * @param text 文本
     * @param pos 当前位置
     * @return 跳过注释和字符串后的位置
     */
    size_t skipCommentsAndStrings(const std::string& text, size_t pos);
    
    /**
     * 跳过空白字符
     * @param text 文本
     * @param pos 当前位置
     * @return 跳过空白后的位置
     */
    size_t skipWhitespace(const std::string& text, size_t pos);
    
    /**
     * 解析标识符
     * @param text 文本
     * @param pos 开始位置
     * @param identifier 输出标识符
     * @return 解析后的位置
     */
    size_t parseIdentifier(const std::string& text, size_t pos, std::string& identifier);
    
    /**
     * 查找匹配的右大括号
     * @param text 文本
     * @param startPos 开始位置（左大括号位置）
     * @return 匹配的右大括号位置，未找到返回string::npos
     */
    size_t findMatchingBrace(const std::string& text, size_t startPos);
    
    /**
     * 检查是否匹配字符串
     * @param text 文本
     * @param pos 位置
     * @param target 目标字符串
     * @return 是否匹配
     */
    bool matchString(const std::string& text, size_t pos, const std::string& target);
};

} // namespace chtl