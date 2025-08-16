import java.io.*;
import java.nio.file.*;
import java.util.*;

public class FixCHTLJSContextFinal {
    public static void main(String[] args) throws IOException {
        System.out.println("Fixing CHTLJSContext.java final issues...");
        
        Path path = Paths.get("src/main/java/com/chtl/context/CHTLJSContext.java");
        if (!Files.exists(path)) {
            System.err.println("CHTLJSContext.java not found!");
            return;
        }
        
        List<String> lines = Files.readAllLines(path);
        List<String> fixedLines = new ArrayList<>();
        boolean inCommentedMethod = false;
        int braceDepth = 0;
        
        for (int i = 0; i < lines.size(); i++) {
            String line = lines.get(i);
            String trimmed = line.trim();
            
            // Fix commented method declarations
            if (trimmed.startsWith("// public") && trimmed.contains("DelegationInfo") && trimmed.contains("{")) {
                // Uncomment the method declaration
                line = line.replace("// public", "public");
                line = line.replace("// TODO: Fix DelegationInfo access", "");
                inCommentedMethod = true;
                braceDepth = 1;
            } else if (trimmed.startsWith("// public") && trimmed.contains("DelegationInfo") && !trimmed.contains("{")) {
                // Method declaration without opening brace
                line = line.replace("// public", "public");
                line = line.replace("// TODO: Fix DelegationInfo access", "");
                fixedLines.add(line);
                
                // Check if next line has the opening brace
                if (i + 1 < lines.size()) {
                    String nextLine = lines.get(i + 1).trim();
                    if (nextLine.equals("{") || nextLine.startsWith("{")) {
                        inCommentedMethod = true;
                        braceDepth = 0;
                    }
                }
                continue;
            }
            
            // Track brace depth
            if (inCommentedMethod) {
                for (char c : line.toCharArray()) {
                    if (c == '{') braceDepth++;
                    if (c == '}') {
                        braceDepth--;
                        if (braceDepth == 0) {
                            inCommentedMethod = false;
                        }
                    }
                }
            }
            
            fixedLines.add(line);
        }
        
        // Write the fixed content
        Files.write(path, fixedLines);
        
        // Now fix the specific issues
        String content = Files.readString(path);
        
        // Fix line 206-208
        content = content.replace(
            "// public void registerDelegation(DelegationInfo info) { // TODO: Fix DelegationInfo access\n" +
            "        delegationRegistry.computeIfAbsent(info.getParentSelector(), k -> new ArrayList<>()).add(info);\n" +
            "    }",
            "public void registerDelegation(DelegationInfo info) {\n" +
            "        delegationRegistry.computeIfAbsent(info.getParentSelector(), k -> new ArrayList<>()).add(info);\n" +
            "    }"
        );
        
        // Fix line 210-212
        content = content.replace(
            "// public Map<String, List<DelegationInfo>> getDelegationRegistry() { // TODO: Fix DelegationInfo access\n" +
            "        return Collections.unmodifiableMap(delegationRegistry);\n" +
            "    }",
            "public Map<String, List<DelegationInfo>> getDelegationRegistry() {\n" +
            "        return Collections.unmodifiableMap(delegationRegistry);\n" +
            "    }"
        );
        
        // Fix any remaining orphaned method bodies
        // Pattern: line that starts with whitespace and contains method-like code but no method declaration
        String[] lines2 = content.split("\n");
        StringBuilder fixed = new StringBuilder();
        boolean skipNext = false;
        
        for (int i = 0; i < lines2.length; i++) {
            if (skipNext) {
                skipNext = false;
                continue;
            }
            
            String line = lines2[i];
            String trimmed = line.trim();
            
            // Check for orphaned method body
            if (i > 0 && trimmed.length() > 0 && !trimmed.startsWith("//") && 
                !trimmed.startsWith("/*") && !trimmed.startsWith("*") &&
                !trimmed.startsWith("public") && !trimmed.startsWith("private") && 
                !trimmed.startsWith("protected") && !trimmed.startsWith("static") &&
                !trimmed.startsWith("@") && !trimmed.startsWith("import") &&
                !trimmed.startsWith("package") && !trimmed.startsWith("class") &&
                !trimmed.startsWith("interface") && !trimmed.startsWith("enum") &&
                !trimmed.startsWith("}")) {
                
                // Check if this looks like orphaned code
                String prevLine = i > 0 ? lines2[i-1].trim() : "";
                if (prevLine.startsWith("//") && prevLine.contains("public") && 
                    prevLine.contains("(") && prevLine.contains(")")) {
                    // This is likely an orphaned method body
                    // Skip this line and the closing brace
                    int braces = 0;
                    for (char c : line.toCharArray()) {
                        if (c == '{') braces++;
                        if (c == '}') braces--;
                    }
                    
                    if (braces >= 0 && i + 1 < lines2.length && lines2[i + 1].trim().equals("}")) {
                        skipNext = true;
                        continue;
                    }
                }
            }
            
            fixed.append(line).append("\n");
        }
        
        // Write the final fixed content
        Files.writeString(path, fixed.toString());
        
        System.out.println("Fixed CHTLJSContext.java");
    }
}