package com.distutil;

import com.models.Changed;
import com.models.NotChanged;
import com.models.Results;
import org.eclipse.jgit.api.Git;
import org.eclipse.jgit.api.errors.GitAPIException;
import org.eclipse.jgit.lib.ObjectId;
import org.eclipse.jgit.revwalk.RevCommit;
import org.eclipse.jgit.treewalk.TreeWalk;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


public class GitUtil {

    public static void main(String[] args) throws IOException, GitAPIException{
        try{
            String eg = args[0];
            String a = args[1];
            String b = args[2];
            String seq = args[3];
            if(gitinit(eg, a, b, seq)){
                MiscUtil.logMsg("Analyze ready.");
            }
            else{
                MiscUtil.logMsg("Analyze failed.");
            }

        }
        catch(Exception e){
            MiscUtil.logMsg("Something went wrong.");
        }
    }

    public static boolean gitinit(String url, String a, String b, String seq)
            throws IOException, GitAPIException{
        String eg = MiscUtil.cleanRef(url);
        File dir = new File(MiscUtil.FPATH + "target/" + eg);
        File repo = new File("./" + eg);
        Git git = Git.cloneRepository()
                .setURI(url)
                .setDirectory(repo)
                .call();
        MiscUtil.logMsg("Having repository: " + git.getRepository());
        if (seq.equals("false")) {
            MiscUtil.logMsg("Analyzing the commit-pair non-sequentially.");
            Results r = gitcommits(git, a, b);
            MiscUtil.writeJson(r);
        } else {
            List<String> hashes = getHashList(git, a, b);
            if (!hashes.isEmpty()) {
                MiscUtil.logMsg("Analyzing the commit-pair sequentially.");
                MiscUtil.writeJson(sequential(git, hashes));
            } else {
                MiscUtil.writeJson();
                return false;
            }
        }
        git.getRepository().close();
        MiscUtil.cleanUp(dir);
        return true;

    }

    private static Results gitcommits(Git git, String hash1, String hash2) throws IOException, GitAPIException{
        List<Changed> rList;
        List<NotChanged> nList;
        boolean found1 = findCommit(git, hash1);
        boolean found2 = findCommit(git, hash2);
        if (found1 && found2) {
            MiscUtil.logMsg("Commits found.");
            RevCommit commit1 = getCommit(git, hash1);
            RevCommit commit2 = getCommit(git, hash2);
            rList = treewalkLeft(git, commit1.getTree(), commit2);
            nList = MiscUtil.getNotChangedList(rList);
            rList = MiscUtil.cleanChanged(rList, nList);
            return (new Results(commit1, commit2, rList, nList));

        }
        return (new Results(hash1, hash2));
    }

    private static List<Changed> treewalkLeft(Git git,
                                     ObjectId treeId, RevCommit commit2)throws IOException{
        List<Changed> rList = new ArrayList<>();
        try (TreeWalk treeWalk = new TreeWalk(git.getRepository())) {
            treeWalk.reset(treeId);
            while (treeWalk.next()) {
                if (treeWalk.isSubtree()) {
                    treeWalk.enterSubtree();
                } else {
                    byte[] data = git.getRepository().open(treeWalk.getObjectId(0)).getBytes();
                    MiscUtil.writeByte(data, true);
                    File fileRight = treewalkRight(git, commit2.getTree(), treeWalk.getPathString());
                    List<List<String>> changes;
                    if (!fileRight.getName().equals("notfound.txt")){
                        changes = DistillerUtil.distillerTestFunction();
                    }
                    else{
                        changes = new ArrayList<>();
                    }
                    rList.add(new Changed(treeWalk.getPathString(),treeWalk.getNameString(), changes));
                }
            }
        }
        return rList;
    }
    private static File treewalkRight(Git git, ObjectId treeId, String leftFilePath)throws IOException{
        try (TreeWalk treeWalk = new TreeWalk(git.getRepository())) {
            treeWalk.reset(treeId);
            while (treeWalk.next()) {
                if (treeWalk.isSubtree()) {
                    treeWalk.enterSubtree();
                } else {
                    if (leftFilePath.equals(treeWalk.getPathString())){
                        byte[] data = git.getRepository().open(treeWalk.getObjectId(0)).getBytes();
                        MiscUtil.writeByte(data, false);
                        return (new File(MiscUtil.FPATH + "right.txt"));
                    }
                }
            }
        }
        return new File(MiscUtil.FPATH + "notfound.txt");
    }

    private static List<String> getHashList(Git git, String hash1, String hash2)throws GitAPIException{
        boolean found1 = findCommit(git, hash1);
        boolean found2 = findCommit(git, hash2);
        if (found1 && found2) {
            return resolveHList(git, hash1, hash2);
        }
        return new ArrayList<>();
    }

    private static List<String> resolveHList(Git git, String hash1, String hash2) throws GitAPIException{
        List<String> hList = new ArrayList<>();
        boolean toBeAdded = false;
        RevCommit commit1 = getCommit(git, hash1);
        RevCommit commit2 = getCommit(git, hash2);
        if (checkTimes(commit1, commit2)){
            Iterable<RevCommit> logs = git.log().call();
            for (RevCommit commit : logs) {
                String commitID = commit.getName();
                if (toBeAdded){
                    hList.add(commitID);
                }
                if (commitID.equals(hash1)) {
                    hList.add(commitID);
                    toBeAdded = true;
                }
                else if (commitID.equals(hash2)){
                    hList.add(commitID);
                    MiscUtil.logMsg("Hashlist parsed successfully.");
                    return hList;
                }
            }
        }
        return hList;
    }

    private static RevCommit getCommit(Git git, String hash) throws GitAPIException{
        RevCommit found = null;
        Iterable<RevCommit> logs = git.log().call();

        for (RevCommit commit : logs) {
            String commitID = commit.getName();
            if (commitID.equals(hash)) {
                found = commit;
            }
        }

        return found;

    }
    private static boolean findCommit(Git git, String hash) throws GitAPIException{
        Iterable<RevCommit> logs = git.log().call();

        for (RevCommit commit : logs) {
            String commitID = commit.getName();
            if (commitID.equals(hash)) {
                return true;
            }
        }
        return false;
    }

    private static boolean checkTimes(RevCommit commit1, RevCommit commit2){
        return (commit1.getCommitTime() > commit2.getCommitTime());
    }

    private static List<Results> sequential(Git git, List<String> hashlist)throws IOException, GitAPIException{
        List<Results> rList = new ArrayList<>();
        String hash1 = "";
        String hash2;
        int total = hashlist.size()-1;
        int i = 1;
        for (String s : hashlist){

            if (hash1.equals("")){
                hash1 = s;
            }
            else{
                hash2 = s;
                if (!hash2.equals(hash1)) {
                    MiscUtil.logMsg("Analyzing: " + i + "/" +total);
                    i+=1;
                    rList.add(gitcommits(git, hash1, hash2));
                }
                hash1 = s;
            }
        }
        return rList;
    }

}
