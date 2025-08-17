#include "Common/ImportProcessor.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace Chtl {

ImportProcessor::ImportProcessor(Context* context)
    : context_(context),
      resolver_(context->getImportResolver()) {
}

ImportProcessor::~ImportProcessor() {
}

std::vector<ASTNodePtr> ImportProcessor::processImport(ImportStatementNode* importNode) {
    std::vector<ASTNodePtr> results;
    
    // 根据导入类型处理
    switch (importNode->getImportType()) {
        case ImportStatementNode::HTML:
        case ImportStatementNode::STYLE:
        case ImportStatementNode::JAVASCRIPT: {
            auto node = processResourceImport(importNode);
            if (node) {
                results.push_back(node);
            }
            break;
        }
        
        case ImportStatementNode::CHTL:
            return processChtlImport(importNode);
            
        case ImportStatementNode::CJMOD:
            return processCJmodImport(importNode);
    }
    
    return results;
}

ASTNodePtr ImportProcessor::processResourceImport(ImportStatementNode* importNode) {
    // 构建导入信息
    ImportInfo info;
    info.path = importNode->getPath();
    info.type = importNode->getImportType();
    info.alias = importNode->getAlias();
    
    // 解析路径
    std::string resolvedPath = resolver_->resolveImportPath(info);
    if (resolvedPath.empty()) {
        reportError("Cannot resolve import path: " + info.path, importNode->getToken());
        return nullptr;
    }
    
    // 检查循环依赖
    if (resolver_->checkCircularDependency(context_->getSourceInfo().fileName, resolvedPath)) {
        reportError("Circular dependency detected: " + resolvedPath, importNode->getToken());
        return nullptr;
    }
    
    // 检查重复导入
    if (resolver_->isAlreadyImported(context_->getSourceInfo().fileName, resolvedPath)) {
        // 跳过重复导入
        return nullptr;
    }
    
    // 记录导入关系
    resolver_->recordImport(context_->getSourceInfo().fileName, resolvedPath);
    
    // 如果没有as语法，跳过
    if (importNode->getAlias().empty()) {
        return nullptr;
    }
    
    // 读取文件内容
    std::string content = readFile(resolvedPath);
    if (content.empty()) {
        reportError("Cannot read file: " + resolvedPath, importNode->getToken());
        return nullptr;
    }
    
    // 创建带名原始嵌入节点
    OriginBlockNode::OriginType originType = convertImportTypeToOriginType(importNode->getImportType());
    return createNamedOriginNode(content, originType, importNode->getAlias());
}

std::vector<ASTNodePtr> ImportProcessor::processChtlImport(ImportStatementNode* importNode) {
    std::vector<ASTNodePtr> results;
    
    std::string path = importNode->getPath();
    
    // 检查是否是通配符导入
    if (path.find('*') != std::string::npos) {
        return processWildcardImport(path, ImportStatementNode::CHTL);
    }
    
    // 构建导入信息
    ImportInfo info;
    info.path = path;
    info.type = ImportStatementNode::CHTL;
    info.targetName = importNode->getTargetName();
    info.isCustom = importNode->getTargetType().find("Custom") != std::string::npos;
    info.isTemplate = importNode->getTargetType().find("Template") != std::string::npos;
    
    // 解析路径
    std::string resolvedPath = resolver_->resolveImportPath(info);
    if (resolvedPath.empty()) {
        reportError("Cannot resolve import path: " + info.path, importNode->getToken());
        return results;
    }
    
    // 检查循环依赖
    if (resolver_->checkCircularDependency(context_->getSourceInfo().fileName, resolvedPath)) {
        reportError("Circular dependency detected: " + resolvedPath, importNode->getToken());
        return results;
    }
    
    // 记录导入关系
    resolver_->recordImport(context_->getSourceInfo().fileName, resolvedPath);
    
    // 如果有特定目标，从CHTL文件中提取
    if (!importNode->getTargetName().empty()) {
        return extractFromChtl(resolvedPath, importNode->getTargetType(), importNode->getTargetName());
    }
    
    // 否则导入整个文件
    // TODO: 实现完整CHTL文件解析和导入
    
    return results;
}

std::vector<ASTNodePtr> ImportProcessor::processCJmodImport(ImportStatementNode* importNode) {
    std::vector<ASTNodePtr> results;
    
    // 构建导入信息
    ImportInfo info;
    info.path = importNode->getPath();
    info.type = ImportStatementNode::CJMOD;
    
    // 解析路径
    std::string resolvedPath = resolver_->resolveImportPath(info);
    if (resolvedPath.empty()) {
        reportError("Cannot resolve import path: " + info.path, importNode->getToken());
        return results;
    }
    
    // 检查循环依赖
    if (resolver_->checkCircularDependency(context_->getSourceInfo().fileName, resolvedPath)) {
        reportError("Circular dependency detected: " + resolvedPath, importNode->getToken());
        return results;
    }
    
    // 记录导入关系
    resolver_->recordImport(context_->getSourceInfo().fileName, resolvedPath);
    
    // 解析CJMOD文件
    return parseCJmod(resolvedPath);
}

std::string ImportProcessor::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

ASTNodePtr ImportProcessor::createNamedOriginNode(const std::string& content, 
                                                  OriginBlockNode::OriginType type,
                                                  const std::string& name) {
    Token token;
    token.type = TokenType::ORIGIN_MARK;
    token.value = "[Origin]";
    token.line = 0;
    token.column = 0;
    
    auto originNode = std::make_shared<OriginBlockNode>(token);
    originNode->setOriginType(type);
    originNode->setContent(content);
    originNode->setName(name);
    
    return originNode;
}

std::vector<ASTNodePtr> ImportProcessor::processWildcardImport(const std::string& pattern,
                                                               ImportStatementNode::ImportType type) {
    std::vector<ASTNodePtr> results;
    
    // 获取匹配的文件列表
    auto files = resolver_->resolveWildcardImport(pattern);
    
    for (const auto& file : files) {
        // 检查循环依赖
        if (resolver_->checkCircularDependency(context_->getSourceInfo().fileName, file)) {
            reportError("Circular dependency detected: " + file, Token());
            continue;
        }
        
        // 记录导入关系
        resolver_->recordImport(context_->getSourceInfo().fileName, file);
        
        // 根据类型处理文件
        if (type == ImportStatementNode::CHTL) {
            // TODO: 解析CHTL文件并导入
        } else if (type == ImportStatementNode::CJMOD) {
            auto nodes = parseCJmod(file);
            results.insert(results.end(), nodes.begin(), nodes.end());
        }
    }
    
    return results;
}

std::vector<ASTNodePtr> ImportProcessor::extractFromChtl(const std::string& path, 
                                                         const std::string& targetType,
                                                         const std::string& targetName) {
    std::vector<ASTNodePtr> results;
    
    // TODO: 实现CHTL文件解析和指定元素提取
    // 这需要使用Parser来解析CHTL文件，然后从AST中提取指定的元素
    
    return results;
}

std::vector<ASTNodePtr> ImportProcessor::parseCJmod(const std::string& path) {
    std::vector<ASTNodePtr> results;
    
    // TODO: 实现CJMOD文件解析
    // CJMOD是二进制格式，需要专门的解析器
    
    return results;
}

OriginBlockNode::OriginType ImportProcessor::convertImportTypeToOriginType(ImportStatementNode::ImportType type) {
    switch (type) {
        case ImportStatementNode::HTML:
            return OriginBlockNode::HTML;
        case ImportStatementNode::STYLE:
            return OriginBlockNode::STYLE;
        case ImportStatementNode::JAVASCRIPT:
            return OriginBlockNode::JAVASCRIPT;
        default:
            return OriginBlockNode::CUSTOM;
    }
}

bool ImportProcessor::isValidResourceFile(const std::string& path, ImportStatementNode::ImportType type) {
    std::string ext = fs::path(path).extension().string();
    
    switch (type) {
        case ImportStatementNode::HTML:
            return ext == ".html" || ext == ".htm";
        case ImportStatementNode::STYLE:
            return ext == ".css";
        case ImportStatementNode::JAVASCRIPT:
            return ext == ".js";
        default:
            return false;
    }
}

void ImportProcessor::reportError(const std::string& message, const Token& token) {
    if (context_) {
        Diagnostic diag;
        diag.level = DiagnosticLevel::ERROR;
        diag.message = message;
        diag.line = token.line;
        diag.column = token.column;
        context_->addDiagnostic(diag);
    }
    std::cerr << "Import Error: " << message << std::endl;
}

} // namespace Chtl