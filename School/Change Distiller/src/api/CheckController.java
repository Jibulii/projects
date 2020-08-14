package com.api;

import com.distutil.MiscUtil;
import com.models.Check;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class CheckController {

    private CheckController() {
        //not called
    }

    @RequestMapping("/check")
    public static Check check() {
        try {
            if (MiscUtil.checkResults()) {
                return new Check("Changes ready", true);
            } else {
                return new Check("Changes not ready", false);
            }
        }
        catch(NullPointerException e){
            return new Check("Error", false);
        }
    }
}