#pragma once

#include "common/types.hpp"
#include <string>
#include <vector>
#include <memory>
#include <regex>
#include <unordered_set>

namespace chtl {

/**
 * 代码切片
 */
struct CodeSlice {
    std::string content;        // 切片内容
    size_t startOffset;         // 起始偏移量
    size_t endOffset;           // 结束偏移量
    Position startPos;          // 起始位置
    Position endPos;            // 结束位置
    bool isComplete;            // 是否为完整片段
    CodeSegmentType suggestedType; // 建议的类型

    CodeSlice(const std::string& c, size_t start, size_t end) 
        : content(c), startOffset(start), endOffset(end), isComplete(false), 
          suggestedType(CodeSegmentType::UNKNOWN) {}
};

/**
 * CHTL最小单元
 */
struct CHTLToken {
    enum Type {
        CHTL_SELECTOR,      // {{selector}}
        CHTL_ARROW,         // ->
        CHTL_LISTEN,        // listen
        CHTL_DELEGATE,      // delegate
        CHTL_ANIMATE,       // animate
        JS_IDENTIFIER,      // 普通JS标识符
        JS_FUNCTION_CALL,   // 函数调用
        JS_PROPERTY,        // 属性访问
        UNKNOWN
    };

    Type type;
    std::string value;
    size_t startOffset;
    size_t endOffset;

    CHTLToken(Type t, const std::string& v, size_t start, size_t end)
        : type(t), value(v), startOffset(start), endOffset(end) {}
};

/**
 * 可变长度切片扫描器
 * 支持智能片段检测、完整性验证和精确切割
 */
class SliceScanner {
public:
    /**
     * 构造函数
     * @param config 编译配置
     */
    explicit SliceScanner(const CompileConfig& config = CompileConfig{});

    /**
     * 扫描源代码并生成智能切片
     * @param source 源代码
     * @param filename 文件名
     * @return 扫描结果
     */
    ScanResult scanWithSlices(const std::string& source, const std::string& filename = "");

    /**
     * 设置初始切片大小
     * @param size 切片大小
     */
    void setInitialSliceSize(size_t size);

    /**
     * 设置最大扩展次数
     * @param maxExpansions 最大扩展次数
     */
    void setMaxExpansions(size_t maxExpansions);

private:
    CompileConfig config_;
    size_t initialSliceSize_;       // 初始切片大小
    size_t maxExpansions_;          // 最大扩展次数
    std::string source_;            // 源代码
    std::string filename_;          // 文件名
    std::vector<Position> linePositions_; // 行位置映射

    /**
     * 生成初始切片
     * @return 初始切片列表
     */
    std::vector<CodeSlice> generateInitialSlices();

    /**
     * 检查切片完整性并扩展
     * @param slices 切片列表
     * @return 调整后的切片列表
     */
    std::vector<CodeSlice> validateAndExpandSlices(std::vector<CodeSlice>& slices);

    /**
     * 检查片段是否完整
     * @param slice 代码切片
     * @param nextSlice 下一个切片（可选）
     * @return 是否完整
     */
    bool isSliceComplete(const CodeSlice& slice, const CodeSlice* nextSlice = nullptr);

    /**
     * 扩展切片到完整状态
     * @param slice 要扩展的切片
     * @param availableLength 可用的扩展长度
     * @return 扩展后的切片
     */
    CodeSlice expandSlice(const CodeSlice& slice, size_t availableLength);

    /**
     * 对CHTL/CHTL JS切片进行最小单元切割
     * @param slice 代码切片
     * @return 最小单元切片列表
     */
    std::vector<CodeSlice> performMinimalUnitSlicing(const CodeSlice& slice);

    /**
     * 基于上下文合并连续的CHTL/CHTL JS片段
     * @param slices 切片列表
     * @return 合并后的切片列表
     */
    std::vector<CodeSlice> mergeContextualSlices(const std::vector<CodeSlice>& slices);

    /**
     * 将切片转换为代码片段
     * @param slices 切片列表
     * @return 代码片段列表
     */
    std::vector<CodeSegment> convertSlicesToSegments(const std::vector<CodeSlice>& slices);

    /**
     * 检测代码类型
     * @param content 代码内容
     * @return 代码类型
     */
    CodeSegmentType detectCodeType(const std::string& content);

    /**
     * 检查是否为CHTL语法开始
     * @param content 内容
     * @return 是否为CHTL语法
     */
    bool isCHTLSyntaxStart(const std::string& content);

    /**
     * 检查是否为CHTL JS语法
     * @param content 内容
     * @return 是否为CHTL JS语法
     */
    bool isCHTLJSSyntax(const std::string& content);

    /**
     * 检查是否为CSS语法
     * @param content 内容
     * @return 是否为CSS语法
     */
    bool isCSSSyntax(const std::string& content);

    /**
     * 检查是否为JavaScript语法
     * @param content 内容
     * @return 是否为JavaScript语法
     */
    bool isJavaScriptSyntax(const std::string& content);

    /**
     * 查找大括号匹配
     * @param content 内容
     * @param startPos 开始位置
     * @return 匹配的结束位置，-1表示未找到
     */
    int findMatchingBrace(const std::string& content, size_t startPos);

    /**
     * 标记化CHTL JS代码
     * @param content 内容
     * @return CHTL令牌列表
     */
    std::vector<CHTLToken> tokenizeCHTLJS(const std::string& content);

    /**
     * 检查是否应该合并两个切片
     * @param current 当前切片
     * @param next 下一个切片
     * @return 是否应该合并
     */
    bool shouldMergeSlices(const CodeSlice& current, const CodeSlice& next);

    /**
     * 合并两个切片
     * @param first 第一个切片
     * @param second 第二个切片
     * @return 合并后的切片
     */
    CodeSlice mergeSlices(const CodeSlice& first, const CodeSlice& second);

    /**
     * 计算位置信息
     * @param offset 字符偏移量
     * @return 位置信息
     */
    Position calculatePosition(size_t offset);

    /**
     * 预处理源代码（移除注释等）
     */
    void preprocessSource();

    /**
     * 构建行位置映射
     */
    void buildLinePositions();

    /**
     * 跳过空白字符
     * @param content 内容
     * @param pos 位置
     * @return 跳过后的位置
     */
    size_t skipWhitespace(const std::string& content, size_t pos);

    /**
     * 检查字符是否为标识符字符
     * @param ch 字符
     * @return 是否为标识符字符
     */
    bool isIdentifierChar(char ch);

    /**
     * 检查字符是否为空白字符
     * @param ch 字符
     * @return 是否为空白字符
     */
    bool isWhitespace(char ch);
};

/**
 * CHTL JS语法特征检测器
 */
class CHTLJSFeatureDetector {
public:
    /**
     * 检测CHTL选择器 {{selector}}
     */
    static bool hasCHTLSelector(const std::string& content);

    /**
     * 检测CHTL箭头操作符 ->
     */
    static bool hasCHTLArrow(const std::string& content);

    /**
     * 检测CHTL监听器语法
     */
    static bool hasCHTLListen(const std::string& content);

    /**
     * 检测CHTL委托语法
     */
    static bool hasCHTLDelegate(const std::string& content);

    /**
     * 检测CHTL动画语法
     */
    static bool hasCHTLAnimate(const std::string& content);

    /**
     * 检测所有CHTL JS特征
     */
    static bool hasAnyCHTLJSFeature(const std::string& content);
};

} // namespace chtl