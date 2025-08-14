package com.example.chtl.core;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class ScanResult {
    private final String source;
    private final List<Fragment> fragments;

    public ScanResult(String source) {
        this.source = source;
        this.fragments = new ArrayList<>();
    }

    public String getSource() {
        return source;
    }

    public void addFragment(Fragment fragment) {
        this.fragments.add(fragment);
    }

    public List<Fragment> getFragments() {
        return Collections.unmodifiableList(fragments);
    }

    public List<Fragment> getFragmentsByType(FragmentType type) {
        return fragments.stream().filter(f -> f.type() == type).toList();
    }
}