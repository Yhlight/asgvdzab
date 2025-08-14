from __future__ import annotations

from dataclasses import dataclass, field
from enum import Enum, auto
from typing import Dict, List, Optional


class FragmentKind(Enum):
    CHTL = auto()
    CHTL_JS = auto()
    CSS = auto()
    JS = auto()


@dataclass
class Fragment:
    kind: FragmentKind
    content: str
    start: int
    end: int
    metadata: Dict[str, str] = field(default_factory=dict)


@dataclass
class CompilationUnit:
    html: str = ""
    css: str = ""
    js: str = ""
    warnings: List[str] = field(default_factory=list)
    errors: List[str] = field(default_factory=list)


@dataclass
class CompilationBundle:
    units: List[CompilationUnit]

    def merge(self) -> CompilationUnit:
        html_parts: List[str] = []
        css_parts: List[str] = []
        js_parts: List[str] = []
        warnings: List[str] = []
        errors: List[str] = []

        for unit in self.units:
            if unit.html:
                html_parts.append(unit.html)
            if unit.css:
                css_parts.append(unit.css)
            if unit.js:
                js_parts.append(unit.js)
            if unit.warnings:
                warnings.extend(unit.warnings)
            if unit.errors:
                errors.extend(unit.errors)

        return CompilationUnit(
            html="".join(html_parts),
            css="\n".join(css_parts),
            js="\n".join(js_parts),
            warnings=warnings,
            errors=errors,
        )