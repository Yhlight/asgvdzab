package com.example.chtl.core.scanner;

import com.example.chtl.core.config.Configuration;

public class ScannerConfig {
	public final Configuration configuration;
	public ScannerConfig(Configuration configuration){ this.configuration = configuration; }
	public static ScannerConfig defaults(){ return new ScannerConfig(Configuration.defaults()); }
}