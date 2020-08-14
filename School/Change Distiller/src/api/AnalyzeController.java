package com.api;

import com.distutil.MiscUtil;
import org.eclipse.jgit.api.errors.GitAPIException;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import com.distutil.GitUtil;

import java.io.IOException;

@RestController
public class AnalyzeController {

    private AnalyzeController() {
        //not called
    }

    @RequestMapping("/analyze")
    public String analyze(@RequestParam(value = "repo", defaultValue = "") String repo,
                          @RequestParam(value = "hash1", defaultValue = "") String hash1,
                          @RequestParam(value = "hash2", defaultValue = "") String hash2,
                          @RequestParam(value = "seq", defaultValue ="") String seq)
                            throws GitAPIException, IOException, InterruptedException{
        if (!repo.equals("") && !hash1.equals("") && checkSeq(seq))  {
            String outcome;
            if (GitUtil.gitinit(repo, hash1, hash2, seq)){
                outcome = "Analyze successful.";
                MiscUtil.logMsg(outcome);
            }
            else{
                outcome = "Analyze failed";
                MiscUtil.logMsg(outcome);
            }
            return outcome;
        }
        else{
            return "Invalid parameters.";
        }
    }

    private boolean checkSeq(String seq){
        return (seq.equals("true") || seq.equals("false"));
    }
}

