package com.models;

import org.eclipse.jgit.revwalk.RevCommit;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;

public class Results {
    private final Map<String, String> leftCommit;
    private final Map<String, String> rightCommit;
    private Map<String, List<Changed>> changedFiles;
    private Map<String, List<NotChanged>> notChangedFiles;

    public Results(RevCommit commit1, RevCommit commit2, List<Changed> rList, List<NotChanged> nList) {

        this.changedFiles= new HashMap<>();
        this.changedFiles.put("Changed", rList);

        this.notChangedFiles = new HashMap<>();
        this.notChangedFiles.put("Not Changed", nList);

        this.leftCommit = new HashMap<>();
        this.leftCommit.put("Hash", commit1.getName());
        this.leftCommit.put("Date", convertTime(commit1.getCommitTime()));

        this.rightCommit = new HashMap<>();
        this.rightCommit.put("Hash", commit2.getName());
        this.rightCommit.put("Date", convertTime(commit2.getCommitTime()));
    }

    public Results(String hash1, String hash2) {
        this.leftCommit = new HashMap<>();
        this.leftCommit.put("Hash", hash1);
        this.leftCommit.put("Date", "");

        this.rightCommit = new HashMap<>();
        this.rightCommit.put("Hash", hash2);
        this.rightCommit.put("Date", "");

        this.notChangedFiles = new HashMap<>();
        this.notChangedFiles.put("Not found", new ArrayList<NotChanged>());

        this.changedFiles = new HashMap<>();
        this.changedFiles.put("Not Found", new ArrayList<Changed>());
    }

    private String convertTime(int epoch){
        long e = epoch;
        return(new java.text.SimpleDateFormat("dd-mm-yyyy HH:mm:ss").format(new java.util.Date (e*1000)));
    }

    public Map<String, List<Changed>> getChangedFiles() { return changedFiles; }
    public Map<String, List<NotChanged>> getNotChangedFiles() { return notChangedFiles; }
    public Map<String, String> getLeftCommit() { return leftCommit; }
    public Map<String, String> getRightCommit() {
        return rightCommit;
    }
}
