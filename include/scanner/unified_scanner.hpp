#pragma once

#include "common/types.hpp"
#include <string>
#include <vector>
#include <memory>
#include <regex>

namespace chtl {

/**
 * CHTL统一扫描器 - 精准代码切割器
 * 负责将CHTL源代码精确分割为不同类型的代码片段
 */
class CHTLUnifiedScanner {
public:
    /**
     * 构造函数
     * @param config 编译配置
     */
    explicit CHTLUnifiedScanner(const CompileConfig& config = CompileConfig{});

    /**
     * 析构函数
     */
    ~CHTLUnifiedScanner() = default;

    /**
     * 扫描CHTL源代码
     * @param source CHTL源代码字符串
     * @param filename 文件名（用于错误报告）
     * @return 扫描结果
     */
    ScanResult scan(const std::string& source, const std::string& filename = "");

    /**
     * 从文件扫描CHTL源代码
     * @param filename 文件路径
     * @return 扫描结果
     */
    ScanResult scanFile(const std::string& filename);

    /**
     * 设置编译配置
     * @param config 新的编译配置
     */
    void setConfig(const CompileConfig& config);

    /**
     * 获取当前编译配置
     * @return 当前编译配置
     */
    const CompileConfig& getConfig() const;

    /**
     * 重置扫描器状态
     */
    void reset();

private:
    /**
     * 扫描器内部状态
     */
    struct ScannerState {
        std::string source;         // 源代码
        std::string filename;       // 文件名
        size_t position;            // 当前位置
        size_t line;                // 当前行号
        size_t column;              // 当前列号
        bool inComment;             // 是否在注释中
        bool inString;              // 是否在字符串中
        char stringDelimiter;       // 字符串分隔符
        int braceDepth;            // 大括号深度
        std::string currentContext; // 当前上下文

        ScannerState() : position(0), line(1), column(1), inComment(false), 
                        inString(false), stringDelimiter(0), braceDepth(0) {}
    };

    CompileConfig config_;          // 编译配置
    ScannerState state_;           // 扫描器状态
    ScanResult result_;            // 扫描结果

    /**
     * 预处理源代码（移除注释、处理转义字符等）
     */
    void preprocessSource();

    /**
     * 识别并提取代码片段
     */
    void extractSegments();

    /**
     * 扫描单个字符
     * @param ch 字符
     * @return 是否继续扫描
     */
    bool scanCharacter(char ch);

    /**
     * 处理注释
     */
    void handleComment();

    /**
     * 处理字符串字面量
     * @param delimiter 字符串分隔符
     */
    void handleString(char delimiter);

    /**
     * 检查是否为关键字开始
     * @param position 检查位置
     * @return 关键字类型，如果不是关键字返回空字符串
     */
    std::string checkKeywordAt(size_t position);

    /**
     * 提取块内容
     * @param startPos 起始位置
     * @param endPos 结束位置
     * @return 块内容
     */
    std::string extractBlock(size_t startPos, size_t endPos);

    /**
     * 分析块类型
     * @param keyword 关键字
     * @param content 块内容
     * @return 代码片段类型
     */
    CodeSegmentType analyzeBlockType(const std::string& keyword, const std::string& content);

    /**
     * 创建代码片段
     * @param type 片段类型
     * @param content 内容
     * @param startPos 起始位置
     * @param endPos 结束位置
     * @param context 上下文
     * @return 代码片段
     */
    CodeSegment createSegment(CodeSegmentType type, const std::string& content,
                             const Position& startPos, const Position& endPos,
                             const std::string& context = "");

    /**
     * 报告错误
     * @param message 错误消息
     * @param position 错误位置
     */
    void reportError(const std::string& message, const Position& position = Position{});

    /**
     * 报告警告
     * @param message 警告消息
     * @param position 警告位置
     */
    void reportWarning(const std::string& message, const Position& position = Position{});

    /**
     * 获取当前位置
     * @return 当前位置
     */
    Position getCurrentPosition() const;

    /**
     * 跳过空白字符
     */
    void skipWhitespace();

    /**
     * 检查字符是否为空白字符
     * @param ch 字符
     * @return 是否为空白字符
     */
    bool isWhitespace(char ch) const;

    /**
     * 检查字符是否为标识符字符
     * @param ch 字符
     * @return 是否为标识符字符
     */
    bool isIdentifierChar(char ch) const;

    /**
     * 读取标识符
     * @return 标识符字符串
     */
    std::string readIdentifier();

    /**
     * 匹配字符串
     * @param str 要匹配的字符串
     * @return 是否匹配成功
     */
    bool match(const std::string& str);

    /**
     * 查看下一个字符（不移动位置）
     * @param offset 偏移量
     * @return 字符，如果超出范围返回'\0'
     */
    char peek(size_t offset = 0) const;

    /**
     * 获取当前字符并移动位置
     * @return 当前字符
     */
    char advance();

    /**
     * 检查是否到达文件末尾
     * @return 是否到达文件末尾
     */
    bool isAtEnd() const;
};

} // namespace chtl