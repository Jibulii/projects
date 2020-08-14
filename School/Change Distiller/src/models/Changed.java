package com.models;

import java.util.List;

public class Changed {
    private final String path;
    private final String fileName;
    private final List<List<String>> changeList;

    public Changed(String path, String fileName, List<List<String>> changes) {
        
        this.path = path;
        this.fileName = fileName;
        this.changeList = changes;
    }
    public List<List<String>> getchangeList(){
    	return changeList;
    }
    public String getpath(){
    	return path;
    }
    public String getfileName(){
    	return fileName;
    }

}

