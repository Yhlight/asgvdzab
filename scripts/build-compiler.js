#!/usr/bin/env node

const fs = require('fs');
const path = require('path');
const { execSync } = require('child_process');

console.log('Building CHTL Compiler for VSCode Extension...\n');

// Paths
const projectRoot = path.join(__dirname, '..');
const javaSourceRoot = path.join(projectRoot, '..', 'src', 'main', 'java');
const moduleSourceRoot = path.join(projectRoot, '..', 'module');
const outputDir = path.join(projectRoot, 'compiler');
const classesDir = path.join(outputDir, 'classes');
const modulesDir = path.join(outputDir, 'modules');

// Ensure output directories exist
if (!fs.existsSync(outputDir)) {
    fs.mkdirSync(outputDir, { recursive: true });
}
if (!fs.existsSync(classesDir)) {
    fs.mkdirSync(classesDir, { recursive: true });
}
if (!fs.existsSync(modulesDir)) {
    fs.mkdirSync(modulesDir, { recursive: true });
}

// Step 1: Compile Java sources
console.log('Step 1: Compiling Java sources...');
try {
    // Find all Java files
    const javaFiles = findFiles(javaSourceRoot, '.java');
    
    if (javaFiles.length === 0) {
        console.error('No Java files found!');
        process.exit(1);
    }
    
    console.log(`Found ${javaFiles.length} Java files`);
    
    // Compile Java files
    const javac = process.platform === 'win32' ? 'javac.exe' : 'javac';
    const compileCommand = `${javac} -d "${classesDir}" -encoding UTF-8 ${javaFiles.map(f => `"${f}"`).join(' ')}`;
    
    console.log('Compiling...');
    execSync(compileCommand, { 
        stdio: 'inherit',
        cwd: projectRoot 
    });
    
    console.log('✓ Java compilation successful\n');
} catch (error) {
    console.error('✗ Java compilation failed:', error.message);
    process.exit(1);
}

// Step 2: Create JAR file
console.log('Step 2: Creating JAR file...');
try {
    const manifestPath = path.join(outputDir, 'MANIFEST.MF');
    const manifestContent = `Manifest-Version: 1.0
Main-Class: com.chtl.cli.CHTLCLI
Class-Path: .
`;
    
    fs.writeFileSync(manifestPath, manifestContent);
    
    const jar = process.platform === 'win32' ? 'jar.exe' : 'jar';
    const jarPath = path.join(outputDir, 'chtl-compiler.jar');
    const jarCommand = `${jar} cfm "${jarPath}" "${manifestPath}" -C "${classesDir}" .`;
    
    execSync(jarCommand, { 
        stdio: 'inherit',
        cwd: projectRoot 
    });
    
    console.log(`✓ JAR created: ${jarPath}\n`);
    
    // Clean up manifest and classes
    fs.unlinkSync(manifestPath);
    // Keep classes for debugging
    // fs.rmSync(classesDir, { recursive: true });
} catch (error) {
    console.error('✗ JAR creation failed:', error.message);
    process.exit(1);
}

// Step 3: Package modules
console.log('Step 3: Packaging modules...');
try {
    const modules = fs.readdirSync(moduleSourceRoot).filter(f => 
        fs.statSync(path.join(moduleSourceRoot, f)).isDirectory()
    );
    
    for (const moduleName of modules) {
        console.log(`Packaging module: ${moduleName}`);
        
        const moduleDir = path.join(moduleSourceRoot, moduleName);
        const moduleFiles = findFiles(moduleDir, '.chtl');
        
        if (moduleFiles.length === 0) {
            console.log(`  No CHTL files found in ${moduleName}, skipping...`);
            continue;
        }
        
        // Create module package
        const modulePackage = {
            name: moduleName,
            version: '1.0.0',
            files: {}
        };
        
        // Read all module files
        for (const file of moduleFiles) {
            const relativePath = path.relative(moduleDir, file);
            const content = fs.readFileSync(file, 'utf8');
            modulePackage.files[relativePath] = content;
        }
        
        // Write module package
        const packagePath = path.join(modulesDir, `${moduleName}.cmod`);
        fs.writeFileSync(packagePath, JSON.stringify(modulePackage, null, 2));
        
        console.log(`  ✓ Created ${moduleName}.cmod`);
    }
    
    console.log('\n✓ All modules packaged successfully\n');
} catch (error) {
    console.error('✗ Module packaging failed:', error.message);
    process.exit(1);
}

// Step 4: Copy runtime resources
console.log('Step 4: Copying runtime resources...');
try {
    // Copy CHTL grammar documentation
    const grammarSrc = path.join(projectRoot, '..', 'CHTL语法文档.md');
    const grammarDest = path.join(outputDir, 'grammar.md');
    
    if (fs.existsSync(grammarSrc)) {
        fs.copyFileSync(grammarSrc, grammarDest);
        console.log('✓ Copied grammar documentation');
    }
    
    // Create compiler info
    const info = {
        version: '1.0.0',
        buildDate: new Date().toISOString(),
        modules: fs.readdirSync(modulesDir).filter(f => f.endsWith('.cmod'))
    };
    
    fs.writeFileSync(
        path.join(outputDir, 'compiler-info.json'),
        JSON.stringify(info, null, 2)
    );
    
    console.log('✓ Created compiler info\n');
} catch (error) {
    console.error('✗ Resource copying failed:', error.message);
}

console.log('Build completed successfully!');
console.log(`Output directory: ${outputDir}`);

// Helper function to find files
function findFiles(dir, extension) {
    const files = [];
    
    function walk(currentDir) {
        const entries = fs.readdirSync(currentDir);
        
        for (const entry of entries) {
            const fullPath = path.join(currentDir, entry);
            const stat = fs.statSync(fullPath);
            
            if (stat.isDirectory()) {
                walk(fullPath);
            } else if (entry.endsWith(extension)) {
                files.push(fullPath);
            }
        }
    }
    
    walk(dir);
    return files;
}