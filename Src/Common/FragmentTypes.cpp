#include "Common/FragmentTypes.h"
#include <algorithm>

namespace Chtl {

// === FragmentContainer实现 ===

void FragmentContainer::AddFragment(std::unique_ptr<CodeFragment> fragment) {
    if (fragment) {
        Fragments_.push_back(std::move(fragment));
    }
}

std::vector<ChtlFragment*> FragmentContainer::GetChtlFragments() const {
    std::vector<ChtlFragment*> result;
    for (const auto& fragment : Fragments_) {
        if (auto chtlFragment = dynamic_cast<ChtlFragment*>(fragment.get())) {
            result.push_back(chtlFragment);
        }
    }
    return result;
}

std::vector<ChtlJsFragment*> FragmentContainer::GetChtlJsFragments() const {
    std::vector<ChtlJsFragment*> result;
    for (const auto& fragment : Fragments_) {
        if (auto chtlJsFragment = dynamic_cast<ChtlJsFragment*>(fragment.get())) {
            result.push_back(chtlJsFragment);
        }
    }
    return result;
}

std::vector<CssFragment*> FragmentContainer::GetCssFragments() const {
    std::vector<CssFragment*> result;
    for (const auto& fragment : Fragments_) {
        if (auto cssFragment = dynamic_cast<CssFragment*>(fragment.get())) {
            result.push_back(cssFragment);
        }
    }
    return result;
}

std::vector<JsFragment*> FragmentContainer::GetJsFragments() const {
    std::vector<JsFragment*> result;
    for (const auto& fragment : Fragments_) {
        if (auto jsFragment = dynamic_cast<JsFragment*>(fragment.get())) {
            result.push_back(jsFragment);
        }
    }
    return result;
}

// === FragmentUtils实现 ===

std::string FragmentUtils::FragmentTypeToString(FragmentType type) {
    switch (type) {
        case FragmentType::CHTL: return "CHTL";
        case FragmentType::CHTL_JS: return "CHTL_JS";
        case FragmentType::CSS: return "CSS";
        case FragmentType::JAVASCRIPT: return "JAVASCRIPT";
        case FragmentType::HTML: return "HTML";
        case FragmentType::UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}

FragmentType FragmentUtils::StringToFragmentType(const std::string& str) {
    if (str == "CHTL") return FragmentType::CHTL;
    if (str == "CHTL_JS") return FragmentType::CHTL_JS;
    if (str == "CSS") return FragmentType::CSS;
    if (str == "JAVASCRIPT") return FragmentType::JAVASCRIPT;
    if (str == "HTML") return FragmentType::HTML;
    return FragmentType::UNKNOWN;
}

bool FragmentUtils::IsScriptFragment(FragmentType type) {
    return type == FragmentType::CHTL_JS || type == FragmentType::JAVASCRIPT;
}

bool FragmentUtils::IsStyleFragment(FragmentType type) {
    return type == FragmentType::CSS;
}

} // namespace Chtl