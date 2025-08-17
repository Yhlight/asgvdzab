#pragma once

#include "Types.h"
#include <variant>

namespace CHTL {
namespace Core {

class Token {
public:
    // Token值的变体类型
    using Value = std::variant<String, int64_t, double, bool>;

    Token() = default;
    
    Token(TokenType type, const String& text, const Position& pos)
        : type_(type), text_(text), position_(pos) {}
    
    Token(TokenType type, const String& text, const Value& value, const Position& pos)
        : type_(type), text_(text), value_(value), position_(pos) {}

    // 获取token类型
    TokenType getType() const { return type_; }
    
    // 获取token文本
    const String& getText() const { return text_; }
    
    // 获取token值
    const Value& getValue() const { return value_; }
    
    // 获取位置信息
    const Position& getPosition() const { return position_; }
    
    // 设置token属性
    void setType(TokenType type) { type_ = type; }
    void setText(const String& text) { text_ = text; }
    void setValue(const Value& value) { value_ = value; }
    void setPosition(const Position& pos) { position_ = pos; }
    
    // 类型检查辅助函数
    bool isIdentifier() const { return type_ == TokenType::IDENTIFIER; }
    bool isStringLiteral() const { return type_ == TokenType::STRING_LITERAL; }
    bool isNumberLiteral() const { return type_ == TokenType::NUMBER_LITERAL; }
    bool isKeyword() const;
    bool isOperator() const;
    bool isDelimiter() const;
    bool isComment() const { return type_ == TokenType::COMMENT; }
    bool isEOF() const { return type_ == TokenType::EOF_TOKEN; }
    bool isError() const { return type_ == TokenType::ERROR; }
    
    // 获取字符串值
    String getStringValue() const;
    
    // 获取数值
    int64_t getIntValue() const;
    double getDoubleValue() const;
    
    // 获取布尔值
    bool getBoolValue() const;
    
    // 转换为字符串表示
    String toString() const;
    
    // 比较操作
    bool operator==(const Token& other) const;
    bool operator!=(const Token& other) const { return !(*this == other); }

private:
    TokenType type_ = TokenType::ERROR;
    String text_;
    Value value_;
    Position position_;
};

// Token类型到字符串的转换
String tokenTypeToString(TokenType type);

// 判断是否为关键字
bool isKeywordToken(TokenType type);

// 判断是否为操作符
bool isOperatorToken(TokenType type);

// 判断是否为分隔符
bool isDelimiterToken(TokenType type);

// 从字符串获取关键字类型
TokenType getKeywordType(const String& text);

// 创建token的便利函数
TokenPtr makeToken(TokenType type, const String& text, const Position& pos);
TokenPtr makeStringToken(const String& text, const String& value, const Position& pos);
TokenPtr makeNumberToken(const String& text, int64_t value, const Position& pos);
TokenPtr makeNumberToken(const String& text, double value, const Position& pos);
TokenPtr makeErrorToken(const String& message, const Position& pos);

} // namespace Core
} // namespace CHTL