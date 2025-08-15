package com.chtl.cjmod;
import java.io.File;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.chtl.cjmod.ModuleSigner.KeyPairInfo;
import com.chtl.cjmod.ModuleSigner.SignatureInfo;
import com.chtl.cjmod.ModuleSigner.VerificationResult;


import java.io.*;
import java.nio.file.*;
import java.security.*;
import java.security.spec.*;
import java.util.*;
import javax.crypto.*;
import java.util.Base64;

/**
 * 模块签名和验证工具
 * 使用RSA算法对模块进行数字签名
 */
public class ModuleSigner {
    
    private static final String ALGORITHM = "RSA";
    private static final String SIGNATURE_ALGORITHM = "SHA256withRSA";
    private static final int KEY_SIZE = 2048;
    
    // 签名文件路径
    private static final String SIGNATURE_FILE = "META-INF/MODULE.SF";
    private static final String CERT_FILE = "META-INF/MODULE.CERT";
    private static final String PUBLIC_KEY_FILE = "META-INF/PUBLIC.KEY";
    
    /**
     * 密钥对
     */
    public static class KeyPairInfo {
        private final PublicKey publicKey;
        private final PrivateKey privateKey;
        private final String fingerprint;
        
        public KeyPairInfo(PublicKey publicKey, PrivateKey privateKey) {
            this.publicKey = publicKey;
            this.privateKey = privateKey;
            this.fingerprint = calculateFingerprint(publicKey);
        }
        
        // Getters
        public PublicKey getPublicKey() { return publicKey; }
        public PrivateKey getPrivateKey() { return privateKey; }
        public String getFingerprint() { return fingerprint; }
        
        private static String calculateFingerprint(PublicKey key) {
            try {
                MessageDigest md = MessageDigest.getInstance("SHA-256");
                byte[] hash = md.digest(key.getEncoded());
                return bytesToHex(hash);
            } catch (NoSuchAlgorithmException e) {
                throw new RuntimeException("Failed to calculate fingerprint", e);
            }
        }
    }
    
    /**
     * 签名信息
     */
    public static class SignatureInfo {
        private final String algorithm;
        private final String signer;
        private final Date timestamp;
        private final String publicKeyFingerprint;
        private final Map<String, String> fileHashes;
        private final String signature;
        
        public SignatureInfo(String algorithm, String signer, Date timestamp,
                           String publicKeyFingerprint, Map<String, String> fileHashes,
                           String signature) {
            this.algorithm = algorithm;
            this.signer = signer;
            this.timestamp = timestamp;
            this.publicKeyFingerprint = publicKeyFingerprint;
            this.fileHashes = fileHashes;
            this.signature = signature;
        }
        
        // Getters
        public String getAlgorithm() { return algorithm; }
        public String getSigner() { return signer; }
        public Date getTimestamp() { return timestamp; }
        public String getPublicKeyFingerprint() { return publicKeyFingerprint; }
        public Map<String, String> getFileHashes() { return fileHashes; }
        public String getSignature() { return signature; }
    }
    
    /**
     * 验证结果
     */
    public static class VerificationResult {
        private final boolean valid;
        private final SignatureInfo signatureInfo;
        private final List<String> errors;
        private final List<String> warnings;
        
        public VerificationResult(boolean valid, SignatureInfo signatureInfo) {
            this.valid = valid;
            this.signatureInfo = signatureInfo;
            this.errors = new ArrayList<>();
            this.warnings = new ArrayList<>();
        }
        
        public void addError(String error) { errors.add(error); }
        public void addWarning(String warning) { warnings.add(warning); }
        
        // Getters
        public boolean isValid() { return valid; }
        public SignatureInfo getSignatureInfo() { return signatureInfo; }
        public List<String> getErrors() { return errors; }
        public List<String> getWarnings() { return warnings; }
    }
    
    /**
     * 生成密钥对
     */
    public KeyPairInfo generateKeyPair() throws Exception {
        KeyPairGenerator keyGen = KeyPairGenerator.getInstance(ALGORITHM);
        keyGen.initialize(KEY_SIZE);
        KeyPair pair = keyGen.generateKeyPair();
        
        return new KeyPairInfo(pair.getPublic(), pair.getPrivate());
    }
    
    /**
     * 保存密钥对到文件
     */
    public void saveKeyPair(KeyPairInfo keyPair, Path publicKeyPath, Path privateKeyPath) 
            throws Exception {
        // 保存公钥
        Files.write(publicKeyPath, keyPair.getPublicKey().getEncoded());
        
        // 保存私钥（应该加密保存，这里简化处理）
        Files.write(privateKeyPath, keyPair.getPrivateKey().getEncoded());
        
        // 设置文件权限（仅Unix/Linux）
        try {
            Set<PosixFilePermission> perms = new HashSet<>();
            perms.add(PosixFilePermission.OWNER_READ);
            perms.add(PosixFilePermission.OWNER_WRITE);
            Files.setPosixFilePermissions(privateKeyPath, perms);
        } catch (UnsupportedOperationException e) {
            // Windows系统不支持POSIX权限
        }
    }
    
    /**
     * 从文件加载密钥对
     */
    public KeyPairInfo loadKeyPair(Path publicKeyPath, Path privateKeyPath) throws Exception {
        // 加载公钥
        byte[] pubKeyBytes = Files.readAllBytes(publicKeyPath);
        X509EncodedKeySpec pubSpec = new X509EncodedKeySpec(pubKeyBytes);
        KeyFactory keyFactory = KeyFactory.getInstance(ALGORITHM);
        PublicKey publicKey = keyFactory.generatePublic(pubSpec);
        
        // 加载私钥
        byte[] privKeyBytes = Files.readAllBytes(privateKeyPath);
        PKCS8EncodedKeySpec privSpec = new PKCS8EncodedKeySpec(privKeyBytes);
        PrivateKey privateKey = keyFactory.generatePrivate(privSpec);
        
        return new KeyPairInfo(publicKey, privateKey);
    }
    
    /**
     * 签名模块
     */
    public void signModule(Path modulePath, KeyPairInfo keyPair, String signer) throws Exception {
        if (!Files.exists(modulePath)) {
            throw new IllegalArgumentException("Module path not found: " + modulePath);
        }
        
        // 计算文件哈希
        Map<String, String> fileHashes = calculateFileHashes(modulePath);
        
        // 创建签名信息
        SignatureInfo signatureInfo = createSignatureInfo(
            SIGNATURE_ALGORITHM,
            signer,
            new Date(),
            keyPair.getFingerprint(),
            fileHashes
        );
        
        // 生成签名
        String signatureData = generateSignature(signatureInfo, keyPair.getPrivateKey());
        
        // 保存签名文件
        saveSignature(modulePath, signatureInfo, signatureData, keyPair.getPublicKey());
    }
    
    /**
     * 验证模块签名
     */
    public VerificationResult verifyModule(Path modulePath) throws Exception {
        if (!Files.exists(modulePath)) {
            throw new IllegalArgumentException("Module path not found: " + modulePath);
        }
        
        // 检查签名文件是否存在
        Path signaturePath = modulePath.resolve(SIGNATURE_FILE);
        Path publicKeyPath = modulePath.resolve(PUBLIC_KEY_FILE);
        
        if (!Files.exists(signaturePath) || !Files.exists(publicKeyPath)) {
            VerificationResult result = new VerificationResult(false, null);
            result.addError("Module is not signed");
            return result;
        }
        
        // 加载签名信息
        SignatureInfo signatureInfo = loadSignature(signaturePath);
        
        // 加载公钥
        PublicKey publicKey = loadPublicKey(publicKeyPath);
        
        // 验证公钥指纹
        String actualFingerprint = KeyPairInfo.calculateFingerprint(publicKey);
        if (!actualFingerprint.equals(signatureInfo.getPublicKeyFingerprint())) {
            VerificationResult result = new VerificationResult(false, signatureInfo);
            result.addError("Public key fingerprint mismatch");
            return result;
        }
        
        // 验证签名
        boolean signatureValid = verifySignature(signatureInfo, publicKey);
        if (!signatureValid) {
            VerificationResult result = new VerificationResult(false, signatureInfo);
            result.addError("Invalid signature");
            return result;
        }
        
        // 验证文件完整性
        Map<String, String> currentHashes = calculateFileHashes(modulePath);
        List<String> modifiedFiles = new ArrayList<>();
        
        for (Map.Entry<String, String> entry : signatureInfo.getFileHashes().entrySet()) {
            String file = entry.getKey();
            String expectedHash = entry.getValue();
            String actualHash = currentHashes.get(file);
            
            if (!expectedHash.equals(actualHash)) {
                modifiedFiles.add(file);
            }
        }
        
        // 检查新增文件
        List<String> newFiles = new ArrayList<>();
        for (String file : currentHashes.keySet()) {
            if (!signatureInfo.getFileHashes().containsKey(file) &&
                !file.startsWith("META-INF/")) {
                newFiles.add(file);
            }
        }
        
        VerificationResult result = new VerificationResult(
            modifiedFiles.isEmpty() && newFiles.isEmpty(), 
            signatureInfo
        );
        
        if (!modifiedFiles.isEmpty()) {
            result.addError("Modified files: " + String.join(", ", modifiedFiles));
        }
        
        if (!newFiles.isEmpty()) {
            result.addWarning("New files detected: " + String.join(", ", newFiles));
        }
        
        return result;
    }
    
    /**
     * 计算文件哈希
     */
    private Map<String, String> calculateFileHashes(Path modulePath) throws Exception {
        Map<String, String> hashes = new HashMap<>();
        
        Files.walk(modulePath)
            .filter(Files::isRegularFile)
            .filter(path -> !path.toString().contains("META-INF"))
            .forEach(path -> {
                try {
                    String relativePath = modulePath.relativize(path).toString()
                        .replace(File.separatorChar, '/');
                    String hash = calculateFileHash(path);
                    hashes.put(relativePath, hash);
                } catch (Exception e) {
                    throw new RuntimeException("Failed to hash file: " + path, e);
                }
            });
        
        return hashes;
    }
    
    /**
     * 计算单个文件的哈希
     */
    private String calculateFileHash(Path file) throws Exception {
        MessageDigest digest = MessageDigest.getInstance("SHA-256");
        
        try (InputStream is = Files.newInputStream(file)) {
            byte[] buffer = new byte[8192];
            int len;
            while ((len = is.read(buffer)) > 0) {
                digest.update(buffer, 0, len);
            }
        }
        
        return bytesToHex(digest.digest());
    }
    
    /**
     * 创建签名信息
     */
    private SignatureInfo createSignatureInfo(String algorithm, String signer, Date timestamp,
                                            String publicKeyFingerprint, Map<String, String> fileHashes) {
        return new SignatureInfo(algorithm, signer, timestamp, publicKeyFingerprint, fileHashes, null);
    }
    
    /**
     * 生成签名
     */
    private String generateSignature(SignatureInfo info, PrivateKey privateKey) throws Exception {
        // 构建待签名数据
        StringBuilder data = new StringBuilder();
        data.append("Algorithm: ").append(info.getAlgorithm()).append("\n");
        data.append("Signer: ").append(info.getSigner()).append("\n");
        data.append("Timestamp: ").append(info.getTimestamp().getTime()).append("\n");
        data.append("PublicKeyFingerprint: ").append(info.getPublicKeyFingerprint()).append("\n");
        
        // 添加文件哈希（按文件名排序以确保一致性）
        TreeMap<String, String> sortedHashes = new TreeMap<>(info.getFileHashes());
        for (Map.Entry<String, String> entry : sortedHashes.entrySet()) {
            data.append("File: ").append(entry.getKey()).append(" = ").append(entry.getValue()).append("\n");
        }
        
        // 签名
        Signature signature = Signature.getInstance(SIGNATURE_ALGORITHM);
        signature.initSign(privateKey);
        signature.update(data.toString().getBytes("UTF-8"));
        
        byte[] signatureBytes = signature.sign();
        return Base64.getEncoder().encodeToString(signatureBytes);
    }
    
    /**
     * 验证签名
     */
    private boolean verifySignature(SignatureInfo info, PublicKey publicKey) throws Exception {
        // 重建待签名数据
        StringBuilder data = new StringBuilder();
        data.append("Algorithm: ").append(info.getAlgorithm()).append("\n");
        data.append("Signer: ").append(info.getSigner()).append("\n");
        data.append("Timestamp: ").append(info.getTimestamp().getTime()).append("\n");
        data.append("PublicKeyFingerprint: ").append(info.getPublicKeyFingerprint()).append("\n");
        
        TreeMap<String, String> sortedHashes = new TreeMap<>(info.getFileHashes());
        for (Map.Entry<String, String> entry : sortedHashes.entrySet()) {
            data.append("File: ").append(entry.getKey()).append(" = ").append(entry.getValue()).append("\n");
        }
        
        // 验证
        Signature signature = Signature.getInstance(SIGNATURE_ALGORITHM);
        signature.initVerify(publicKey);
        signature.update(data.toString().getBytes("UTF-8"));
        
        byte[] signatureBytes = Base64.getDecoder().decode(info.getSignature());
        return signature.verify(signatureBytes);
    }
    
    /**
     * 保存签名
     */
    private void saveSignature(Path modulePath, SignatureInfo info, String signature, 
                              PublicKey publicKey) throws Exception {
        // 创建META-INF目录
        Path metaInfPath = modulePath.resolve("META-INF");
        Files.createDirectories(metaInfPath);
        
        // 保存签名文件
        Path signaturePath = metaInfPath.resolve("MODULE.SF");
        Properties props = new Properties();
        props.setProperty("Algorithm", info.getAlgorithm());
        props.setProperty("Signer", info.getSigner());
        props.setProperty("Timestamp", String.valueOf(info.getTimestamp().getTime()));
        props.setProperty("PublicKeyFingerprint", info.getPublicKeyFingerprint());
        props.setProperty("Signature", signature);
        
        // 保存文件哈希
        for (Map.Entry<String, String> entry : info.getFileHashes().entrySet()) {
            props.setProperty("FileHash." + entry.getKey(), entry.getValue());
        }
        
        try (OutputStream os = Files.newOutputStream(signaturePath)) {
            props.store(os, "Module Signature");
        }
        
        // 保存公钥
        Path publicKeyPath = metaInfPath.resolve("PUBLIC.KEY");
        Files.write(publicKeyPath, publicKey.getEncoded());
    }
    
    /**
     * 加载签名
     */
    private SignatureInfo loadSignature(Path signaturePath) throws Exception {
        Properties props = new Properties();
        try (InputStream is = Files.newInputStream(signaturePath)) {
            props.load(is);
        }
        
        String algorithm = props.getProperty("Algorithm");
        String signer = props.getProperty("Signer");
        long timestamp = Long.parseLong(props.getProperty("Timestamp"));
        String publicKeyFingerprint = props.getProperty("PublicKeyFingerprint");
        String signature = props.getProperty("Signature");
        
        // 加载文件哈希
        Map<String, String> fileHashes = new HashMap<>();
        for (String key : props.stringPropertyNames()) {
            if (key.startsWith("FileHash.")) {
                String file = key.substring("FileHash.".length());
                String hash = props.getProperty(key);
                fileHashes.put(file, hash);
            }
        }
        
        return new SignatureInfo(algorithm, signer, new Date(timestamp),
                               publicKeyFingerprint, fileHashes, signature);
    }
    
    /**
     * 加载公钥
     */
    private PublicKey loadPublicKey(Path publicKeyPath) throws Exception {
        byte[] keyBytes = Files.readAllBytes(publicKeyPath);
        X509EncodedKeySpec spec = new X509EncodedKeySpec(keyBytes);
        KeyFactory keyFactory = KeyFactory.getInstance(ALGORITHM);
        return keyFactory.generatePublic(spec);
    }
    
    /**
     * 字节数组转十六进制字符串
     */
    private static String bytesToHex(byte[] bytes) {
        StringBuilder result = new StringBuilder();
        for (byte b : bytes) {
            result.append(String.format("%02x", b));
        }
        return result.toString();
    }
}