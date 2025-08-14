# CHTL Compiler (Java 17)

- Build: `mvn -q -e -DskipTests package`
- Run: `java -jar target/chtl-compiler-0.1.0-SNAPSHOT-shaded.jar -o out.html input.chtl`

架构：Scanner -> CompilerDispatcher -> {CHTL, CHTL JS, CSS, JS} -> ResultMerger -> HTML 输出。