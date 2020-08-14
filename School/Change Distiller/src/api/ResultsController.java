package com.api;
import com.models.Check;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import java.io.*;
import com.distutil.MiscUtil;

@RestController
public class ResultsController {

    private ResultsController() {
        //not called
    }

    @RequestMapping(value = "/getresults", produces = "application/json")
    public String getresults() throws IOException {
        Check test = CheckController.check();
        try (InputStream is = new FileInputStream(MiscUtil.FPATH + MiscUtil.FJSON);
             BufferedReader buf = new BufferedReader(new InputStreamReader(is))) {
            if (test.getContent()) {
                return buf.readLine();
            } else {
                return "{ \"Error\":\"NotReadyError\"}";
            }
        }
    }
}