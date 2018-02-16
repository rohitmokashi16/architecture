import java.util.HashMap;
import java.util.Set;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

public class JType extends Instruction
{
    private int immediate;
    private static Pattern pattern = Pattern.compile(Instruction.rexOpcodes(getIcodes()) + "\\s\\b.*\\b");
    private static HashMap<String, Integer> JMap = new HashMap<String, Integer>();
    static
    {
        JMap.put("j", 2);
        JMap.put("jal", 3);
    }
    
    public static Set<String> getIcodes() {
        if(JMap == null) {
            JMap = new HashMap<>();
            JMap.put("j", 2);
            JMap.put("jal", 3);
        }
        return JMap.keySet();
    }
    
    /* 
     * Function    : parseInstruction
     * Description : Parses the JType instruction, returns the encoding.
     */
    public static JType parseInstruction(String instruction, int address) {
        JType instruct = new JType();
        String icode = Instruction.getIcodeName(instruction);
        instruct.setIcode(JMap.get(icode));

        Pattern wordPattern = Pattern.compile("\\b[^\\s]*\\b$");
        Matcher wordMatcher = wordPattern.matcher(instruction);

        wordMatcher.find();
        // Advance to the next word in the pattern

        String immediateName = wordMatcher.group(0);

        // Look up the symbol
        SymbolTable symbols = SymbolTable.getInstance();
        instruct.immediate = symbols.getSymbol(immediateName) - (address + 4);

        return instruct;
    }

    @Override
    public int encode() {
        immediate = immediate & 0x03ffffff;
        return ((icode << 26) | immediate);
    }
}
