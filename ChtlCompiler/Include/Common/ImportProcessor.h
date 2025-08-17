#ifndef CHTL_COMMON_IMPORTPROCESSOR_H
#define CHTL_COMMON_IMPORTPROCESSOR_H

#include <memory>
#include <string>
#include <vector>
#include "AST/ConfigNodes.h"
#include "Common/ImportResolver.h"
#include "Common/Context.h"

namespace Chtl {

class ImportProcessor {
public:
    ImportProcessor(Context* context);
    ~ImportProcessor();
    
    // 处理导入语句，返回生成的AST节点（如果需要）
    std::vector<ASTNodePtr> processImport(ImportStatementNode* importNode);
    
    // 处理@Html/@Style/@JavaScript导入
    ASTNodePtr processResourceImport(ImportStatementNode* importNode);
    
    // 处理@Chtl导入
    std::vector<ASTNodePtr> processChtlImport(ImportStatementNode* importNode);
    
    // 处理@CJmod导入
    std::vector<ASTNodePtr> processCJmodImport(ImportStatementNode* importNode);
    
    // 读取文件内容
    std::string readFile(const std::string& path);
    
    // 创建带名原始嵌入节点（as语法）
    ASTNodePtr createNamedOriginNode(const std::string& content, 
                                    OriginBlockNode::OriginType type,
                                    const std::string& name);
    
    // 处理通配符导入
    std::vector<ASTNodePtr> processWildcardImport(const std::string& pattern,
                                                  ImportStatementNode::ImportType type);
    
    // 解析CHTL文件并提取指定的元素
    std::vector<ASTNodePtr> extractFromChtl(const std::string& path, 
                                           const std::string& targetType,
                                           const std::string& targetName);
    
    // 解析CJMOD文件
    std::vector<ASTNodePtr> parseCJmod(const std::string& path);
    
private:
    Context* context_;
    ImportResolver* resolver_;
    
    // 辅助方法
    OriginBlockNode::OriginType convertImportTypeToOriginType(ImportStatementNode::ImportType type);
    bool isValidResourceFile(const std::string& path, ImportStatementNode::ImportType type);
    void reportError(const std::string& message, const Token& token);
};

} // namespace Chtl

#endif // CHTL_COMMON_IMPORTPROCESSOR_H