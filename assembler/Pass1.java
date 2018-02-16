import java.util.HashMap;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.File;
import java.util.HashSet;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
import java.io.IOException;

public class Pass1 {

    public enum Directive {
        _text,
        _data,
        _align,
        _ascii,
        _double,
        _float,
        _word,
        _label,
        _space,
        instruction
    }
    private static int currentMemAddress = 0;
    private static Pass1Output output;
    private static HashMap<Directive, Pattern> expressions = new HashMap<>();
    static {
        expressions.put(Directive._text, Pattern.compile("^\\s*\\.text(\\s(0x)?\\d+)?"));
        expressions.put(Directive._data, Pattern.compile("^\\s*\\.data(\\s(0x)?\\d+)?"));
        expressions.put(Directive._align, Pattern.compile("^\\s*\\.align\\s(0x)?\\d+"));
        expressions.put(Directive._ascii, Pattern.compile("\\.asciiz\\s\".*\"((\\s\".*\")*)?"));
        expressions.put(Directive._double, Pattern.compile("\\.double.*"));
        expressions.put(Directive._float, Pattern.compile("\\.float.*")); // rather than use a couple of nasty regular expressions here, I'm gonna put it in the match functions
        expressions.put(Directive._word, Pattern.compile("\\.word.*"));
        expressions.put(Directive._label, Pattern.compile("^(\\s*)?.*\\:\\s"));
        expressions.put(Directive._space, Pattern.compile("\\.space\\s(0x)?\\d+"));
        // for now, this is just going to look for an opcode in the string
        expressions.put(Directive.instruction, Pattern.compile(Instruction.rexOpcodes() + "[^;]*"));
    }

    /* 
     * Function    : readFile
     * Description : Reads the file in and starts the processing.
     */
    public static Pass1Output readFile(String filename) throws IOException {
        output = new Pass1Output();
        BufferedReader br = new BufferedReader(new FileReader(new File(filename)));
        String line;
        while((line = br.readLine()) != null) {
            processLine(line);
        }
        return output;
    }
    
    /* 
     * Function    : matchText
     * Description : Checks the line to see if there is plain text.
     */
    private static boolean matchText(String line) {
        Matcher textMatcher = expressions.get(Directive._text).matcher(line);
        if (textMatcher.find()) {
            Pattern number = Pattern.compile("(0x)?\\d+");
            Matcher numMatcher = number.matcher(line);
            if (numMatcher.find()) {
                currentMemAddress = Tools.processNumber(numMatcher.group(0));
            } else {
                currentMemAddress = 0;
            }
            return true;
        }
        return false;
    }

    /* 
     * Function    : matchData
     * Description : Checks if the line has plain data in it (0x...).
     */
    private static boolean matchData(String line) {
        Matcher dataMatcher = expressions.get(Directive._data).matcher(line);
        if (dataMatcher.find()) {
            Pattern number = Pattern.compile("(0x)?\\d+");
            Matcher numMatcher = number.matcher(line);
            if (numMatcher.find()) {
                currentMemAddress = Tools.processNumber(numMatcher.group(0));
            } else {
                currentMemAddress = 0x200;
            }
            return true;
        }
        return false;
    }
    
    /* 
     * Function    : matchAlign
     * Description : Checks to see if the line is aligning.
     */
    private static boolean matchAlign(String line) {
        Matcher alignMatcher = expressions.get(Directive._align).matcher(line);
        if (alignMatcher.find()) {
            Pattern number = Pattern.compile("(0x)?\\d+");
            Matcher numMatcher = number.matcher(line);
            numMatcher.find();
            int num = Tools.processNumber(numMatcher.group(0));
            num = 1 << num;
            while (currentMemAddress % num != 0) {
                currentMemAddress++;
            }
            return true;
        }
        return false;
    }
    
    /* 
     * Function    : matchLabel
     * Description : Checks to see if the line has a label attatched to it.
     */
    private static boolean matchLabel(String line) {
        Matcher labelMatcher = expressions.get(Directive._label).matcher(line);
        if (labelMatcher.find()) {
            Pattern labeltext = Pattern.compile("^[^:]*\\b");
            Matcher textMatcher = labeltext.matcher(line);
            textMatcher.find();
            SymbolTable.getInstance().putSymbol(textMatcher.group(0), currentMemAddress);
            return true;
        }
        return false;
    }
    
    /* 
     * Function    : matchInstruction
     * Description : Checks to see if the line has an instuction on it.
     */
    private static boolean matchInstruction(String line) {
        Matcher instructionMatcher = expressions.get(Directive.instruction).matcher(line);
        if (instructionMatcher.find()) {
            output.putInstruction(currentMemAddress, instructionMatcher.group(0).trim());
            output.putOriginal(currentMemAddress, line);
            currentMemAddress += 4;
        }
        return false;
    }
    
    /* 
     * Function    : matchAsciiz
     * Description : Checks to see if the line has Ascii chars on it.
     */
    public static boolean matchAsciiz(String line) {
        Matcher asciizMatcher = expressions.get(Directive._ascii).matcher(line);
        if (asciizMatcher.find()) {
            Pattern strings = Pattern.compile("\"[^\"]*\"");
            Matcher matcher = strings.matcher(asciizMatcher.group(0));
            while(matcher.find()) {
                String matchedString = matcher.group(0);
                matchedString = matchedString.replace("\"", "");
                String hexString = "";
                char[] charArray = matchedString.toCharArray();
                for ( char c : charArray) {
                    hexString += String.format("%02X", (byte)c);
                }
                hexString += "00"; // append the null terminator
                output.putData(currentMemAddress, hexString);
                output.putOriginal(currentMemAddress, matcher.group(0));
                currentMemAddress += matchedString.length() + 1;
            }
            //System.out.println("Ascii strings: " + matcher.toString());
            return true;
        }
        return false;
    }
    
    /* 
     * Function    : matchSpace
     * Description : Checks to see if the line has spaces on it.
     */
    public static boolean matchSpace(String line) {
        Matcher spaceMatcher = expressions.get(Directive._space).matcher(line);
        if(spaceMatcher.find()) {
          Pattern spacePattern = Pattern.compile("(0x)?\\d+");
          Matcher matcher  = spacePattern.matcher(spaceMatcher.group(0));
          matcher.find();
          currentMemAddress += Tools.processNumber(matcher.group(0)); 
          return true;
        }
        return false;
    }
    
    /* 
     * Function    : matchFloat
     * Description : Checks to see if the line has floating point characters.
     */
    public static boolean matchFloat(String line) {
        Matcher floatMatcher = expressions.get(Directive._float).matcher(line);
        if(floatMatcher.find()) {
            Pattern floatPattern = Pattern.compile("-?((\\d+(\\.\\d+))|(\\.\\d+)|\\d+)");
            Matcher matcher = floatPattern.matcher(floatMatcher.group(0));
            while (matcher.find()) {
                output.putData(currentMemAddress, String.format("%08x", Float.floatToIntBits(Float.parseFloat(matcher.group(0)))));
                output.putOriginal(currentMemAddress, " " + matcher.group(0));
                currentMemAddress += 4;
            }
            return true;
        }
        return false;
    }

    public static boolean matchWord(String line) {
      Matcher wordMatcher = expressions.get(Directive._word).matcher(line);
        if(wordMatcher.find()) {
            //System.out.println("Line: " + line + "matched word regex");

            Pattern wordPattern = Pattern.compile("\\-?(0x)?\\d+");
            Matcher matcher = wordPattern.matcher(wordMatcher.group(0));
            while(matcher.find()) {
                output.putData(currentMemAddress, String.format("%08x", Tools.processNumber(matcher.group(0))));
                output.putOriginal(currentMemAddress, " " + Tools.processNumber(matcher.group(0)));
                currentMemAddress += 4;
            }
            return true;
        }
        return false;
    }

    /* 
     * Function    : matchDouble
     * Description : Checks to see if the line has a double on it.
     */
    public static boolean matchDouble(String line) {
        Matcher doubleMatcher = expressions.get(Directive._double).matcher(line);
        if(doubleMatcher.find()) {
            Pattern doublePattern = Pattern.compile("-?((\\d+(\\.\\d+))|(\\.\\d+)|\\d+)");
            Matcher matcher = doublePattern.matcher(doubleMatcher.group(0));
            while(matcher.find()) {
                double dValue = Double.parseDouble(matcher.group(0));
                String stringValue = Long.toHexString(Double.doubleToLongBits(dValue));
                output.putData(currentMemAddress, stringValue);
                output.putOriginal(currentMemAddress, " " + matcher.group(0));
                currentMemAddress += 8;
            }
            return true;
        }
        return false;
    }

    /* 
     * Function    : processLine
     * Description : Performs the check of which type of data is contained.
     */
    public static void processLine(String line) {
        matchLabel(line);
        if (matchText(line))
            return;
        if (matchData(line))
            return;
        if (matchAlign(line))
            return;
        if (matchSpace(line)) 
            return;
        if (matchInstruction(line))
            return;
        if (matchAsciiz(line))
            return;
        if (matchFloat(line))
            return;
        if (matchWord(line))
            return;
        if (matchDouble(line))
            return;
    }
}
