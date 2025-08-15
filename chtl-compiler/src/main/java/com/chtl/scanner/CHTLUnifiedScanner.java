package com.chtl.scanner;

import com.chtl.model.CodeFragment;
import com.chtl.model.CodeFragmentType;

import java.util.ArrayList;
import java.util.List;

public class CHTLUnifiedScanner {
    private enum Context { HTML, CSS, JS }

    public List<CodeFragment> scan(String source) {
        List<CodeFragment> fragments = new ArrayList<>();
        if (source == null || source.isEmpty()) {
            return fragments;
        }
        String lower = source.toLowerCase();
        int pos = 0;
        int length = source.length();
        while (pos < length) {
            int nextStyle = lower.indexOf("<style", pos);
            int nextScript = lower.indexOf("<script", pos);
            int nextTag = -1;
            boolean isStyle = false;
            if (nextStyle >= 0 && nextScript >= 0) {
                nextTag = Math.min(nextStyle, nextScript);
                isStyle = nextStyle == nextTag;
            } else if (nextStyle >= 0) {
                nextTag = nextStyle;
                isStyle = true;
            } else if (nextScript >= 0) {
                nextTag = nextScript;
                isStyle = false;
            }

            if (nextTag == -1) {
                // remaining HTML
                scanSegment(source, pos, length, Context.HTML, fragments);
                break;
            }

            if (nextTag > pos) {
                // HTML before the tag
                scanSegment(source, pos, nextTag, Context.HTML, fragments);
            }

            int tagStart = nextTag;
            int startTagClose = lower.indexOf('>', tagStart);
            if (startTagClose < 0) {
                // malformed tag, treat as HTML
                scanSegment(source, tagStart, length, Context.HTML, fragments);
                break;
            }
            int afterStart = startTagClose + 1;

            if (isStyle) {
                // style ... </style>
                int endTagStart = lower.indexOf("</style>", afterStart);
                int endTagClose = endTagStart < 0 ? length : endTagStart + "</style>".length();
                String startTagText = lower.substring(tagStart, afterStart);
                boolean scoped = startTagText.contains("scoped");
                if (scoped) {
                    // 局部样式整体交由 CHTL 编译器
                    String slice = source.substring(tagStart, endTagClose);
                    fragments.add(new CodeFragment(CodeFragmentType.LOCAL_STYLE_BLOCK, tagStart, endTagClose, slice));
                } else {
                    // 输出起始/结束标签为 HTML_TEXT，中间 CSS 内容宽判并以 CHTL 分割
                    fragments.add(new CodeFragment(CodeFragmentType.HTML_TEXT, tagStart, afterStart, source.substring(tagStart, afterStart)));
                    int cssContentEnd = endTagStart < 0 ? endTagClose : endTagStart;
                    if (afterStart < cssContentEnd) {
                        scanSegment(source, afterStart, cssContentEnd, Context.CSS, fragments);
                    }
                    if (endTagStart >= 0) {
                        fragments.add(new CodeFragment(CodeFragmentType.HTML_TEXT, endTagStart, endTagClose, source.substring(endTagStart, endTagClose)));
                    }
                }
                pos = endTagClose;
            } else {
                // script ... </script>
                int endTagStart = lower.indexOf("</script>", afterStart);
                int endTagClose = endTagStart < 0 ? length : endTagStart + "</script>".length();
                // 输出起始/结束标签为 HTML_TEXT，中间 JS 内容宽判并以 CHTL/CHTL JS 分割
                fragments.add(new CodeFragment(CodeFragmentType.HTML_TEXT, tagStart, afterStart, source.substring(tagStart, afterStart)));
                int jsContentEnd = endTagStart < 0 ? endTagClose : endTagStart;
                if (afterStart < jsContentEnd) {
                    scanSegment(source, afterStart, jsContentEnd, Context.JS, fragments);
                }
                if (endTagStart >= 0) {
                    fragments.add(new CodeFragment(CodeFragmentType.HTML_TEXT, endTagStart, endTagClose, source.substring(endTagStart, endTagClose)));
                }
                pos = endTagClose;
            }
        }
        return fragments;
    }

    private void scanSegment(String src, int start, int end, Context ctx, List<CodeFragment> out) {
        int pos = start;
        while (pos < end) {
            int moustacheStart = indexOf(src, "{{", pos, end);
            if (moustacheStart < 0) {
                // remaining broad segment
                if (pos < end) {
                    appendBroad(src, pos, end, ctx, out);
                }
                return;
            }
            // emit broad before moustache
            if (moustacheStart > pos) {
                appendBroad(src, pos, moustacheStart, ctx, out);
            }
            // parse strict CHTL token(s)
            int tokenStart = moustacheStart;
            int tokenStop = parseOneMoustacheWithArrow(src, moustacheStart, end);
            if (tokenStop < 0) {
                // no closing, treat as broad
                appendBroad(src, moustacheStart, end, ctx, out);
                return;
            }
            // 基于上下文：合并相邻最小单元（只允许空白分隔）
            while (true) {
                int after = skipWhitespace(src, tokenStop, end);
                if (after >= end) break;
                if (startsWithAt(src, after, end, "{{")) {
                    int nextStop = parseOneMoustacheWithArrow(src, after, end);
                    if (nextStop < 0) break;
                    tokenStop = nextStop; // 合并
                } else {
                    break;
                }
            }
            CodeFragmentType type = (ctx == Context.JS) ? CodeFragmentType.CHTL_JS_TOKEN : CodeFragmentType.CHTL_TOKEN;
            out.add(new CodeFragment(type, tokenStart, tokenStop, src.substring(tokenStart, tokenStop)));
            pos = tokenStop;
        }
    }

    private void appendBroad(String src, int from, int to, Context ctx, List<CodeFragment> out) {
        if (from >= to) return;
        CodeFragmentType type;
        switch (ctx) {
            case HTML:
                type = CodeFragmentType.HTML_TEXT;
                break;
            case CSS:
                type = CodeFragmentType.CSS_CODE;
                break;
            case JS:
                type = CodeFragmentType.JS_CODE;
                break;
            default:
                type = CodeFragmentType.HTML_TEXT;
        }
        out.add(new CodeFragment(type, from, to, src.substring(from, to)));
    }

    private int parseOneMoustacheWithArrow(String src, int start, int end) {
        if (!startsWithAt(src, start, end, "{{")) return -1;
        int close = indexOf(src, "}}", start + 2, end);
        if (close < 0) return -1;
        int stop = close + 2;
        // 严判：若后续紧跟 -> 则并入同一最小单元
        if (startsWithAt(src, stop, end, "->")) {
            stop += 2;
        }
        return stop;
    }

    private boolean startsWithAt(String s, int idx, int end, String token) {
        if (idx < 0) return false;
        int remain = end - idx;
        if (remain < token.length()) return false;
        for (int i = 0; i < token.length(); i++) {
            if (s.charAt(idx + i) != token.charAt(i)) return false;
        }
        return true;
    }

    private int indexOf(String s, String needle, int from, int end) {
        int idx = s.indexOf(needle, from);
        return (idx >= 0 && idx < end) ? idx : -1;
    }

    private int skipWhitespace(String s, int from, int end) {
        int i = from;
        while (i < end) {
            char c = s.charAt(i);
            if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
                i++;
            } else {
                break;
            }
        }
        return i;
    }
}