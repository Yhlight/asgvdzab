#include "compiler/base_compiler.h"
#include <atomic>

namespace chtl {
namespace compiler {

// 静态成员初始化
std::atomic<size_t> BaseCompiler::idCounter_{0};

std::string BaseCompiler::generateUniqueId(const std::string& prefix) {
    size_t id = idCounter_.fetch_add(1, std::memory_order_relaxed);
    return prefix + "_" + std::to_string(id);
}

} // namespace compiler
} // namespace chtl