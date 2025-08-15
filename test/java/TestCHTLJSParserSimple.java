import com.chtl.parser.CHTLJSParser;
import com.chtl.ast.chtljs.*;
import java.util.List;

public class TestCHTLJSParserSimple {
    public static void main(String[] args) {
        String code = "animate({ duration: 1000 });";
        
        System.out.println("解析代码: " + code);
        
        CHTLJSParser parser = new CHTLJSParser();
        List<CHTLJSASTNode> ast = parser.parse(code);
        
        System.out.println("错误: " + parser.getErrors());
        System.out.println("AST节点数: " + ast.size());
        
        if (!ast.isEmpty()) {
            CHTLJSASTNode node = ast.get(0);
            System.out.println("节点类型: " + node.getNodeType());
        }
    }
}