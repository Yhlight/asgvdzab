package com.chtl.scanner;

import com.chtl.model.CodeFragment;
import com.chtl.model.CodeFragmentType;

import java.util.ArrayList;
import java.util.List;

public class CHTLUnifiedScanner {
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
                // remaining template
                String slice = source.substring(pos);
                fragments.add(new CodeFragment(CodeFragmentType.TEMPLATE, pos, length, slice));
                break;
            }

            if (nextTag > pos) {
                String slice = source.substring(pos, nextTag);
                fragments.add(new CodeFragment(CodeFragmentType.TEMPLATE, pos, nextTag, slice));
            }

            int tagStart = nextTag;
            int startTagClose = lower.indexOf('>', tagStart);
            if (startTagClose < 0) {
                String slice = source.substring(tagStart);
                fragments.add(new CodeFragment(CodeFragmentType.TEMPLATE, tagStart, length, slice));
                break;
            }
            int afterStart = startTagClose + 1;
            CodeFragmentType type;
            int endTagClose;
            if (isStyle) {
                String startTagText = lower.substring(tagStart, afterStart);
                boolean scoped = startTagText.contains("scoped");
                type = scoped ? CodeFragmentType.LOCAL_STYLE_BLOCK : CodeFragmentType.GLOBAL_STYLE_BLOCK;
                int endTagStart = lower.indexOf("</style>", afterStart);
                if (endTagStart < 0) {
                    endTagClose = length;
                } else {
                    endTagClose = endTagStart + "</style>".length();
                }
            } else {
                type = CodeFragmentType.SCRIPT_BLOCK;
                int endTagStart = lower.indexOf("</script>", afterStart);
                if (endTagStart < 0) {
                    endTagClose = length;
                } else {
                    endTagClose = endTagStart + "</script>".length();
                }
            }

            String slice = source.substring(tagStart, endTagClose);
            fragments.add(new CodeFragment(type, tagStart, endTagClose, slice));
            pos = endTagClose;
        }
        return fragments;
    }
}