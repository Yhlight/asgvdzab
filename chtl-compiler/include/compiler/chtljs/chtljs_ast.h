#ifndef CHTLJS_AST_H
#define CHTLJS_AST_H

#include <string>
#include <vector>
#include <memory>
#include "compiler/chtljs/chtljs_tokens.h"

namespace chtl {

// CHTL JS AST节点类型枚举
enum class CHTLJSASTNodeType {
    // 基础节点
    ROOT,                      // 根节点
    
    // 选择器相关
    ENHANCED_SELECTOR,         // 增强选择器 {{...}}
    
    // 操作符
    ARROW_ACCESS,              // 箭头访问 ->
    
    // 特殊函数
    LISTEN_FUNCTION,           // listen函数
    DELEGATE_FUNCTION,         // delegate函数
    ANIMATE_FUNCTION,          // animate函数
    
    // 虚对象
    VIR_DECLARATION,           // vir声明
    VIR_ACCESS,               // 虚对象访问
    
    // 通用节点
    IDENTIFIER,               // 标识符
    STRING_LITERAL,           // 字符串字面量
    NUMBER_LITERAL,           // 数字字面量
    OBJECT_LITERAL,           // 对象字面量
    ARRAY_LITERAL,           // 数组字面量
    FUNCTION_EXPRESSION,      // 函数表达式
    
    // 表达式
    MEMBER_ACCESS,            // 成员访问
    FUNCTION_CALL,            // 函数调用
    ASSIGNMENT,               // 赋值
    
    // 语句
    EXPRESSION_STATEMENT,     // 表达式语句
    BLOCK_STATEMENT,          // 块语句
    
    // JavaScript代码块
    JS_CODE_BLOCK            // 纯JavaScript代码块
};

// CHTL JS AST基础节点
class CHTLJSASTNode {
public:
    CHTLJSASTNode(CHTLJSASTNodeType type, size_t line = 0, size_t column = 0)
        : type_(type), line_(line), column_(column) {}
    
    virtual ~CHTLJSASTNode() = default;
    
    CHTLJSASTNodeType getType() const { return type_; }
    size_t getLine() const { return line_; }
    size_t getColumn() const { return column_; }
    
    // 子节点管理
    void addChild(std::unique_ptr<CHTLJSASTNode> child) {
        children_.push_back(std::move(child));
    }
    
    const std::vector<std::unique_ptr<CHTLJSASTNode>>& getChildren() const {
        return children_;
    }
    
protected:
    CHTLJSASTNodeType type_;
    size_t line_;
    size_t column_;
    std::vector<std::unique_ptr<CHTLJSASTNode>> children_;
};

// 根节点
class CHTLJSRootNode : public CHTLJSASTNode {
public:
    CHTLJSRootNode() : CHTLJSASTNode(CHTLJSASTNodeType::ROOT) {}
};

// 增强选择器节点 {{...}}
class EnhancedSelectorNode : public CHTLJSASTNode {
public:
    EnhancedSelectorNode(const std::string& selector, size_t line = 0, size_t column = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::ENHANCED_SELECTOR, line, column),
          selector_(selector) {}
    
    const std::string& getSelector() const { return selector_; }
    
    // 判断选择器类型
    bool isClassSelector() const { 
        return !selector_.empty() && selector_[0] == '.'; 
    }
    
    bool isIdSelector() const { 
        return !selector_.empty() && selector_[0] == '#'; 
    }
    
    bool hasIndexAccess() const {
        return selector_.find('[') != std::string::npos;
    }
    
    // 获取索引（如果有）
    int getIndex() const {
        size_t start = selector_.find('[');
        size_t end = selector_.find(']');
        if (start != std::string::npos && end != std::string::npos) {
            std::string index_str = selector_.substr(start + 1, end - start - 1);
            return std::stoi(index_str);
        }
        return -1;
    }
    
    // 获取基础选择器（不含索引）
    std::string getBaseSelector() const {
        size_t bracket = selector_.find('[');
        if (bracket != std::string::npos) {
            return selector_.substr(0, bracket);
        }
        return selector_;
    }
    
private:
    std::string selector_;
};

// 箭头访问节点 ->
class ArrowAccessNode : public CHTLJSASTNode {
public:
    ArrowAccessNode(size_t line = 0, size_t column = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::ARROW_ACCESS, line, column) {}
    
    void setLeft(std::unique_ptr<CHTLJSASTNode> left) {
        if (!children_.empty()) {
            children_[0] = std::move(left);
        } else {
            addChild(std::move(left));
        }
    }
    
    void setRight(std::unique_ptr<CHTLJSASTNode> right) {
        if (children_.size() > 1) {
            children_[1] = std::move(right);
        } else {
            while (children_.size() < 1) {
                addChild(nullptr);
            }
            addChild(std::move(right));
        }
    }
    
    CHTLJSASTNode* getLeft() const { 
        return children_.empty() ? nullptr : children_[0].get(); 
    }
    
    CHTLJSASTNode* getRight() const { 
        return children_.size() < 2 ? nullptr : children_[1].get(); 
    }
};

// listen函数节点
class ListenFunctionNode : public CHTLJSASTNode {
public:
    ListenFunctionNode(size_t line = 0, size_t column = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::LISTEN_FUNCTION, line, column) {}
    
    void setSelector(std::unique_ptr<CHTLJSASTNode> selector) {
        if (children_.size() > 0) {
            children_[0] = std::move(selector);
        } else {
            addChild(std::move(selector));
        }
    }
    
    void setEvent(std::unique_ptr<CHTLJSASTNode> event) {
        while (children_.size() < 1) addChild(nullptr);
        if (children_.size() > 1) {
            children_[1] = std::move(event);
        } else {
            addChild(std::move(event));
        }
    }
    
    void setHandler(std::unique_ptr<CHTLJSASTNode> handler) {
        while (children_.size() < 2) addChild(nullptr);
        if (children_.size() > 2) {
            children_[2] = std::move(handler);
        } else {
            addChild(std::move(handler));
        }
    }
    
    CHTLJSASTNode* getSelector() const {
        return children_.empty() ? nullptr : children_[0].get();
    }
    
    CHTLJSASTNode* getEvent() const {
        return children_.size() < 2 ? nullptr : children_[1].get();
    }
    
    CHTLJSASTNode* getHandler() const {
        return children_.size() < 3 ? nullptr : children_[2].get();
    }
};

// delegate函数节点
class DelegateFunctionNode : public CHTLJSASTNode {
public:
    DelegateFunctionNode(std::unique_ptr<CHTLJSASTNode> parent,
                        std::unique_ptr<CHTLJSASTNode> config,
                        size_t line = 0, size_t column = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::DELEGATE_FUNCTION, line, column) {
        addChild(std::move(parent));   // 父元素
        addChild(std::move(config));   // 配置对象
    }
    
    CHTLJSASTNode* getParent() const {
        return children_.empty() ? nullptr : children_[0].get();
    }
    
    CHTLJSASTNode* getConfig() const {
        return children_.size() < 2 ? nullptr : children_[1].get();
    }
};

// animate函数节点
class AnimateFunctionNode : public CHTLJSASTNode {
public:
    AnimateFunctionNode(std::unique_ptr<CHTLJSASTNode> config,
                       size_t line = 0, size_t column = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::ANIMATE_FUNCTION, line, column) {
        addChild(std::move(config));   // 动画配置对象
    }
    
    CHTLJSASTNode* getConfig() const {
        return children_.empty() ? nullptr : children_[0].get();
    }
};

// vir声明节点
class VirDeclarationNode : public CHTLJSASTNode {
public:
    VirDeclarationNode(const std::string& name,
                      std::unique_ptr<CHTLJSASTNode> value,
                      size_t line = 0, size_t column = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::VIR_DECLARATION, line, column),
          name_(name) {
        addChild(std::move(value));
    }
    
    const std::string& getName() const { return name_; }
    
    CHTLJSASTNode* getValue() const {
        return children_.empty() ? nullptr : children_[0].get();
    }
    
    // 存储提取的函数键
    void addFunctionKey(const std::string& key) {
        function_keys_.push_back(key);
    }
    
    const std::vector<std::string>& getFunctionKeys() const {
        return function_keys_;
    }
    
private:
    std::string name_;
    std::vector<std::string> function_keys_;
};

// 虚对象访问节点
class VirAccessNode : public CHTLJSASTNode {
public:
    VirAccessNode(const std::string& vir_name,
                 const std::string& function_name,
                 size_t line = 0, size_t column = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::VIR_ACCESS, line, column),
          vir_name_(vir_name), function_name_(function_name) {}
    
    const std::string& getVirName() const { return vir_name_; }
    const std::string& getFunctionName() const { return function_name_; }
    
private:
    std::string vir_name_;
    std::string function_name_;
};

// 标识符节点
class CHTLJSIdentifierNode : public CHTLJSASTNode {
public:
    CHTLJSIdentifierNode(const std::string& name, size_t line = 0, size_t column = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::IDENTIFIER, line, column),
          name_(name) {}
    
    const std::string& getName() const { return name_; }
    
private:
    std::string name_;
};

// 字符串字面量节点
class CHTLJSStringLiteralNode : public CHTLJSASTNode {
public:
    CHTLJSStringLiteralNode(const std::string& value, size_t line = 0, size_t column = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::STRING_LITERAL, line, column),
          value_(value) {}
    
    const std::string& getValue() const { return value_; }
    
private:
    std::string value_;
};

// 数字字面量节点
class CHTLJSNumberLiteralNode : public CHTLJSASTNode {
public:
    CHTLJSNumberLiteralNode(double value, size_t line = 0, size_t column = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::NUMBER_LITERAL, line, column),
          value_(value) {}
    
    double getValue() const { return value_; }
    
private:
    double value_;
};

// 对象字面量节点
class ObjectLiteralNode : public CHTLJSASTNode {
public:
    ObjectLiteralNode(size_t line = 0, size_t column = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::OBJECT_LITERAL, line, column) {}
    
    // 添加属性
    void addProperty(const std::string& key, std::unique_ptr<CHTLJSASTNode> value) {
        properties_.push_back({key, std::move(value)});
    }
    
    const std::vector<std::pair<std::string, std::unique_ptr<CHTLJSASTNode>>>& 
    getProperties() const {
        return properties_;
    }
    
private:
    std::vector<std::pair<std::string, std::unique_ptr<CHTLJSASTNode>>> properties_;
};

// 数组字面量节点
class ArrayLiteralNode : public CHTLJSASTNode {
public:
    ArrayLiteralNode(size_t line = 0, size_t column = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::ARRAY_LITERAL, line, column) {}
    
    void addElement(std::unique_ptr<CHTLJSASTNode> element) {
        addChild(std::move(element));
    }
};

// 函数表达式节点
class FunctionExpressionNode : public CHTLJSASTNode {
public:
    FunctionExpressionNode(const std::vector<std::string>& params,
                          std::unique_ptr<CHTLJSASTNode> body,
                          bool is_arrow = false,
                          size_t line = 0, size_t column = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::FUNCTION_EXPRESSION, line, column),
          parameters_(params), is_arrow_(is_arrow) {
        addChild(std::move(body));
    }
    
    const std::vector<std::string>& getParameters() const { return parameters_; }
    bool isArrowFunction() const { return is_arrow_; }
    
    CHTLJSASTNode* getBody() const {
        return children_.empty() ? nullptr : children_[0].get();
    }
    
private:
    std::vector<std::string> parameters_;
    bool is_arrow_;
};

// 成员访问节点
class MemberAccessNode : public CHTLJSASTNode {
public:
    MemberAccessNode(std::unique_ptr<CHTLJSASTNode> object,
                    const std::string& property,
                    size_t line = 0, size_t column = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::MEMBER_ACCESS, line, column),
          property_(property) {
        addChild(std::move(object));
    }
    
    CHTLJSASTNode* getObject() const {
        return children_.empty() ? nullptr : children_[0].get();
    }
    
    const std::string& getProperty() const { return property_; }
    
private:
    std::string property_;
};

// 函数调用节点
class FunctionCallNode : public CHTLJSASTNode {
public:
    FunctionCallNode(std::unique_ptr<CHTLJSASTNode> function,
                    size_t line = 0, size_t column = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::FUNCTION_CALL, line, column) {
        addChild(std::move(function));
    }
    
    void addArgument(std::unique_ptr<CHTLJSASTNode> arg) {
        addChild(std::move(arg));
    }
    
    CHTLJSASTNode* getFunction() const {
        return children_.empty() ? nullptr : children_[0].get();
    }
    
    std::vector<CHTLJSASTNode*> getArguments() const {
        std::vector<CHTLJSASTNode*> args;
        for (size_t i = 1; i < children_.size(); ++i) {
            args.push_back(children_[i].get());
        }
        return args;
    }
};

// JavaScript代码块节点
class JSCodeBlockNode : public CHTLJSASTNode {
public:
    JSCodeBlockNode(const std::string& code, size_t line = 0, size_t column = 0)
        : CHTLJSASTNode(CHTLJSASTNodeType::JS_CODE_BLOCK, line, column),
          code_(code) {}
    
    const std::string& getCode() const { return code_; }
    
private:
    std::string code_;
};

} // namespace chtl

#endif // CHTLJS_AST_H