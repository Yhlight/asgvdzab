@echo off
REM CHTL IDE-Compatible Build Script
REM This script prepares the project for IDE compilation

echo ===================================
echo CHTL IDE-Compatible Build
echo ===================================
echo.

cd /d "%~dp0\..\.."

REM Step 1: Setup environment
echo [STEP 1] Setting up environment...
mkdir target\classes 2>nul
mkdir lib 2>nul

REM Step 2: Download minimal dependencies
echo [STEP 2] Downloading dependencies...
if not exist "lib\antlr4-runtime-4.13.1.jar" (
    echo Downloading ANTLR runtime...
    curl -L -o "lib\antlr4-runtime-4.13.1.jar" "https://repo1.maven.org/maven2/org/antlr/antlr4-runtime/4.13.1/antlr4-runtime-4.13.1.jar"
)

if not exist "lib\slf4j-api-2.0.9.jar" (
    echo Downloading SLF4J API...
    curl -L -o "lib\slf4j-api-2.0.9.jar" "https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar"
)

if not exist "lib\slf4j-simple-2.0.9.jar" (
    echo Downloading SLF4J Simple...
    curl -L -o "lib\slf4j-simple-2.0.9.jar" "https://repo1.maven.org/maven2/org/slf4j/slf4j-simple/2.0.9/slf4j-simple-2.0.9.jar"
)

REM Step 3: Create project configuration for IDE
echo [STEP 3] Creating IDE configuration files...

REM Create .classpath for Eclipse
(
echo ^<?xml version="1.0" encoding="UTF-8"?^>
echo ^<classpath^>
echo     ^<classpathentry kind="src" path="src/main/java"/^>
echo     ^<classpathentry kind="con" path="org.eclipse.jdt.launching.JRE_CONTAINER/org.eclipse.jdt.internal.debug.ui.launcher.StandardVMType/JavaSE-17"/^>
echo     ^<classpathentry kind="lib" path="lib/antlr4-runtime-4.13.1.jar"/^>
echo     ^<classpathentry kind="lib" path="lib/slf4j-api-2.0.9.jar"/^>
echo     ^<classpathentry kind="lib" path="lib/slf4j-simple-2.0.9.jar"/^>
echo     ^<classpathentry kind="output" path="target/classes"/^>
echo ^</classpath^>
) > .classpath

REM Create .project for Eclipse
(
echo ^<?xml version="1.0" encoding="UTF-8"?^>
echo ^<projectDescription^>
echo     ^<name^>chtl-compiler^</name^>
echo     ^<buildSpec^>
echo         ^<buildCommand^>
echo             ^<name^>org.eclipse.jdt.core.javabuilder^</name^>
echo         ^</buildCommand^>
echo     ^</buildSpec^>
echo     ^<natures^>
echo         ^<nature^>org.eclipse.jdt.core.javanature^</nature^>
echo     ^</natures^>
echo ^</projectDescription^>
) > .project

REM Create IntelliJ IDEA configuration
mkdir .idea 2>nul
(
echo ^<?xml version="1.0" encoding="UTF-8"?^>
echo ^<module type="JAVA_MODULE" version="4"^>
echo   ^<component name="NewModuleRootManager" inherit-compiler-output="false"^>
echo     ^<output url="file://$MODULE_DIR$/target/classes" /^>
echo     ^<content url="file://$MODULE_DIR$"^>
echo       ^<sourceFolder url="file://$MODULE_DIR$/src/main/java" isTestSource="false" /^>
echo     ^</content^>
echo     ^<orderEntry type="inheritedJdk" /^>
echo     ^<orderEntry type="sourceFolder" forTests="false" /^>
echo     ^<orderEntry type="library" name="antlr4-runtime-4.13.1" level="project" /^>
echo     ^<orderEntry type="library" name="slf4j-api-2.0.9" level="project" /^>
echo     ^<orderEntry type="library" name="slf4j-simple-2.0.9" level="project" /^>
echo   ^</component^>
echo ^</module^>
) > chtl-compiler.iml

REM Step 4: Create build.gradle for Gradle users
echo [STEP 4] Creating Gradle build file...
(
echo plugins {
echo     id 'java'
echo     id 'application'
echo }
echo.
echo group = 'com.chtl'
echo version = '1.0.0'
echo.
echo repositories {
echo     mavenCentral^(^)
echo }
echo.
echo dependencies {
echo     implementation 'org.antlr:antlr4-runtime:4.13.1'
echo     implementation 'org.slf4j:slf4j-api:2.0.9'
echo     implementation 'org.slf4j:slf4j-simple:2.0.9'
echo }
echo.
echo application {
echo     mainClass = 'com.chtl.cli.CHTLCLI'
echo }
echo.
echo java {
echo     sourceCompatibility = JavaVersion.VERSION_17
echo     targetCompatibility = JavaVersion.VERSION_17
echo }
echo.
echo tasks.withType^(JavaCompile^) {
echo     options.encoding = 'UTF-8'
echo }
) > build.gradle

REM Step 5: Run fixes
echo [STEP 5] Running automatic fixes...
if exist FixImports.class (
    java FixImports >nul 2>&1
)
if exist FixMoreIssues.class (
    java FixMoreIssues >nul 2>&1
)

echo.
echo ===================================
echo IDE Setup Complete!
echo ===================================
echo.
echo For IntelliJ IDEA:
echo   1. File -^> Open -^> Select this directory
echo   2. Import as existing project
echo   3. SDK: Java 17 or higher
echo.
echo For Eclipse:
echo   1. File -^> Import -^> Existing Projects into Workspace
echo   2. Select this directory
echo.
echo For VS Code:
echo   1. Open folder in VS Code
echo   2. Install Java Extension Pack
echo   3. Reload window
echo.
echo For Gradle:
echo   gradle build
echo.
echo Note: Some features may be limited due to missing ANTLR grammar files.
echo.
pause