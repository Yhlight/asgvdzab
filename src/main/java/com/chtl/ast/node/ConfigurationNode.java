package com.chtl.ast.node;import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.CHTLASTVisitor;


import com.chtl.ast.*;
import java.util.HashMap;
import java.util.Map;

/**
 * 配置节点
 * 对应CHTL中的[Configuration] { }
 */
public class ConfigurationNode extends AbstractCHTLASTNode {
    private Map<String, String> settings;
    private Map<String, String[]> nameGroup; // Name配置块
    
    public ConfigurationNode() {
        super(NodeType.CONFIGURATION);
        this.settings = new HashMap<>();
        this.nameGroup = new HashMap<>();
        
        // 设置默认值
        initDefaultSettings();
    }
    
    private void initDefaultSettings() {
        settings.put("INDEX_INITIAL_COUNT", "0");
        settings.put("DISABLE_NAME_GROUP", "false");
        settings.put("DEBUG_MODE", "false");
    }
    
    public Map<String, String> getSettings() {
        return settings;
    }
    
    public void setSetting(String key, String value) {
        settings.put(key, value);
    }
    
    public String getSetting(String key) {
        return settings.get(key);
    }
    
    public Map<String, String[]> getNameGroup() {
        return nameGroup;
    }
    
    public void setNameMapping(String key, String[] values) {
        nameGroup.put(key, values);
    }
    
    public void setNameMapping(String key, String value) {
        nameGroup.put(key, new String[]{value});
    }
    
    public String[] getNameMapping(String key) {
        return nameGroup.get(key);
    }
    
    @Override
    public void accept(CHTLASTVisitor visitor) {
        visitor.visitConfiguration(this);
    }
}