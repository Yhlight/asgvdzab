#include "compiler/chtl/chtl_ast.h"

namespace chtl {
namespace compiler {
namespace ast {

// ASTNode基类虚析构函数实现
ASTNode::~ASTNode() = default;

// 各个节点类的析构函数实现
DocumentNode::~DocumentNode() = default;
ElementNode::~ElementNode() = default;
TextNode::~TextNode() = default;
StyleNode::~StyleNode() = default;
ScriptNode::~ScriptNode() = default;
TemplateNode::~TemplateNode() = default;
CustomNode::~CustomNode() = default;
ImportNode::~ImportNode() = default;
NamespaceNode::~NamespaceNode() = default;
OriginNode::~OriginNode() = default;
ConfigurationNode::~ConfigurationNode() = default;
AttributeNode::~AttributeNode() = default;
CommentNode::~CommentNode() = default;

} // namespace ast
} // namespace compiler
} // namespace chtl