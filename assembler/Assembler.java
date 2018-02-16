import java.util.HashMap;
import java.util.ArrayList;
import java.util.regex.*;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;

public class Assembler
{
    /* 
     * Function    : main
     * Description : The main method that executes the assembler, calls the passes.
     */
    public static void main(String[] args)
    {
        if (args[0] == null || !(args[0]).endsWith(".dlx"))
        {
            System.out.println("Invalid File Name");
            System.exit(0);
        }
        SymbolTable symbolTable = SymbolTable.getInstance();
        try {
            Pass2.doPass2(Pass1.readFile(args[0]));
        } catch (IOException ex) {

        }
    }
}

