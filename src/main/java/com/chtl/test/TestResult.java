package com.chtl.test;

/**
 * 测试结果
 */
public class TestResult {
    private final String testName;
    private TestStatus status = TestStatus.PENDING;
    private String message;
    private String output;
    private Exception exception;
    private long executionTime;
    
    public TestResult(String testName) {
        this.testName = testName;
    }
    
    // Getters and setters
    public String getTestName() {
        return testName;
    }
    
    public TestStatus getStatus() {
        return status;
    }
    
    public void setStatus(TestStatus status) {
        this.status = status;
    }
    
    public String getMessage() {
        return message;
    }
    
    public void setMessage(String message) {
        this.message = message;
    }
    
    public String getOutput() {
        return output;
    }
    
    public void setOutput(String output) {
        this.output = output;
    }
    
    public Exception getException() {
        return exception;
    }
    
    public void setException(Exception exception) {
        this.exception = exception;
    }
    
    public long getExecutionTime() {
        return executionTime;
    }
    
    public void setExecutionTime(long executionTime) {
        this.executionTime = executionTime;
    }
}