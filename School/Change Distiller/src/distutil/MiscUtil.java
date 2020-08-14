package com.distutil;

import com.google.gson.Gson;
import com.models.Changed;
import com.models.NotChanged;
import com.models.Results;
import org.apache.commons.io.FileUtils;

import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Logger;

public class MiscUtil {

    private MiscUtil() {
        //not called
    }

    public static final String FPATH = File.separator + "root" + File.separator;
    public static final String FJSON = "results.json";
    private static final String RNOTFOUND = "Right.txt not found.";

    public static void logMsg(String msg){
        Logger logger = Logger.getLogger("CD-Logger");
        logger.info(msg);
    }

    static void cleanUp(File file) throws IOException{
        FileUtils.deleteDirectory(file);
        logMsg("Git-Directory deleted.");
        Runtime rt = Runtime.getRuntime();
        File dir = new File(".");
        String cmd = "rm -rf " + file.getName();
        Process proc = rt.exec(cmd, null, dir);
        try (BufferedReader stdInput = new BufferedReader(new
                InputStreamReader(proc.getInputStream()));
             BufferedReader stdError = new BufferedReader(new
                     InputStreamReader(proc.getErrorStream()))){
            logMsg(cmd);
            // read the output from the command
            logMsg("Here is the standard output of the command:\n");
            String s = null;
            while ((s = stdInput.readLine()) != null) {
                logMsg(s);
            }

            // read any errors from the attempted command
            logMsg("Here is the standard error of the command (if any):\n");
            while ((s = stdError.readLine()) != null) {
                logMsg(s);
            }
        }
        catch (IOException e){
            logMsg("IOEXPECTION");
        }
    }

    static void writeByte(byte[] bytes, boolean w) {
        String filePath;
        if (w){
            filePath = FPATH + "left.txt";
        }
        else {
            filePath = FPATH + "right.txt";
        }
        File file = new File(filePath);
        try (PrintWriter writer = new PrintWriter(file)){
            writer.print("");
        }
        catch (FileNotFoundException e ){
            logMsg(RNOTFOUND);
        }

        try (OutputStream os = new FileOutputStream(filePath)){
            os.write(bytes);
        }
        catch (Exception e) {
            logMsg("Exception: " + e.getMessage());
        }
    }

    static void writeJson(Results r){
        Gson gson = new Gson();
        String json = gson.toJson(r);
        String path = FPATH + FJSON;
        File file = new File(path);
        try (PrintWriter writer = new PrintWriter(file)){
            writer.print("");
            writer.print(json);
        }
        catch (FileNotFoundException e ){
            logMsg(RNOTFOUND);
        }
    }

    static void writeJson(List<Results> r){
        Gson gson = new Gson();
        String json = gson.toJson(r);
        String path = FPATH + FJSON;
        File file = new File(path);
        try (PrintWriter writer = new PrintWriter(file)){
            writer.print("");
            writer.print(json);
        }
        catch (FileNotFoundException e ){
            logMsg(RNOTFOUND);
        }
    }

    static void writeJson(){
        Gson gson = new Gson();
        String json = gson.toJson("Error");
        String path = FPATH + FJSON;
        File file = new File(path);
        try (PrintWriter writer = new PrintWriter(file)){
            writer.print("");
            writer.print(json);
        }
        catch (FileNotFoundException e ){
            logMsg(RNOTFOUND);
        }
    }

    static List<NotChanged> getNotChangedList(List<Changed> cList){
        List<NotChanged> nList = new ArrayList<>();
        for (Changed c : cList){
            if (c.getchangeList().isEmpty()){
                nList.add(new NotChanged(c.getpath(), c.getfileName()));
            }
        }
        return nList;
    }

    static List<Changed> cleanChanged(List<Changed> cList, List<NotChanged> nList){
        List<Changed> cListNew = new ArrayList<>();
        boolean check;
        for (Changed c : cList){
            check = false;
            for (NotChanged n : nList){
                if (c.getpath().equals(n.getpath()) && c.getfileName().equals(n.getfileName())){
                    check = true;
                }
            }
            if (!check){
                cListNew.add(c);
            }
        }
        return cListNew;
    }
    static String cleanRef(String line) {
        String[] bits = line.split("/");
        return bits[bits.length - 1].replace(".git", "");
    }

    public static boolean checkResults(){
        File file = new File(FPATH+FJSON);
        return file.length() != 0;
    }

}

