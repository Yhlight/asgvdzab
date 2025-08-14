package com.example.chtl.core.config;

public class Configuration {
	public int INDEX_INITIAL_COUNT = 0;
	public boolean DEBUG_MODE = false;
	public boolean DISABLE_NAME_GROUP = false;
	public int OPTION_COUNT = 3;

	public NameGroup nameGroup = NameGroup.defaults();

	public static Configuration defaults(){ return new Configuration(); }
}