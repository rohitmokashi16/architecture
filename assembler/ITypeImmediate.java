import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.Set;
import java.util.HashMap;
public class ITypeImmediate extends IType {
    protected static HashMap<String, Integer> IMap = new HashMap<String, Integer>();
    static
    {
        IMap.put("beqz", 4);
        IMap.put("bnez", 5);
        IMap.put("addi", 8);
        IMap.put("addui", 9);
        IMap.put("subi", 10);
        IMap.put("subui", 11);
        IMap.put("andi", 12);
        IMap.put("ori", 13);
        IMap.put("xori", 14);
        IMap.put("lhi", 15);
        IMap.put("trap", 17);
        IMap.put("jr", 18);
        IMap.put("jalr", 19);
        IMap.put("slli", 20);
        IMap.put("srli", 22);
        IMap.put("srai", 23);
        IMap.put("seqi", 24);
        IMap.put("snei", 25);
        IMap.put("slti", 26);
        IMap.put("sgti", 27);
        IMap.put("slei", 28);
        IMap.put("sgei", 29);
        IMap.put("lb", 32);
        IMap.put("lh", 33);
        IMap.put("lw", 35);
        IMap.put("lbu", 36);
        IMap.put("lhu", 37);
        IMap.put("lf", 38);
        IMap.put("ld", 39);
        IMap.put("sb", 40);
        IMap.put("sh", 41);
        IMap.put("sw", 43);
        IMap.put("sf", 46);
        IMap.put("sd", 47);
    }
  private static int BNEZ=5, BEQZ=4;
  
  /* 
     * Function    : parseInstruction
     * Description : Parses the IType instructions without displacements.
     */
  public static ITypeImmediate parseInstruction(String instruction, int address) {
        ITypeImmediate instruct = new ITypeImmediate();
        // set the icode
        instruct.icode = IMap.get(Instruction.getIcodeName(instruction));
        // grab the string from the registers.java class that we will use to match registers
        Pattern registerPattern = Pattern.compile(Registers.registerString());
        Matcher registerMatcher = registerPattern.matcher(instruction);
        // match the first register if there is one 
        if (registerMatcher.find()) {
          instruct.rd = Registers.registerValue(registerMatcher.group(0));
          // I clear off the bits we've already read
          instruction = instruction.substring(registerMatcher.end(0));
        }
        // match the second register if we can
        if (registerMatcher.find()) {
          instruct.r1 = Registers.registerValue(registerMatcher.group(0));
        }
        // This regex attempts to match a label name. The [^\\s\\d] means not a space or digit character
        // This ensures that the label is infact a valid label and does not start with a number.
        Pattern symbolPattern = Pattern.compile("\\b([^\\d\\s][^\\s]*)\\b$");
        Matcher symbolMatcher = symbolPattern.matcher(instruction);
        Pattern literalPattern = Pattern.compile("\\b(0x)?\\d+\\b");
        Matcher literalMatcher = literalPattern.matcher(instruction);

        if(symbolMatcher.find()) {
          
          SymbolTable symbolTable = SymbolTable.getInstance();
          
            instruct.immediateValue = symbolTable.getSymbol(symbolMatcher.group(0).replace(" ", ""));
            if(instruct.icode == BEQZ || instruct.icode == BNEZ) {
              instruct.r1 = instruct.rd;
              instruct.rd = 0;
              instruct.immediateValue = instruct.immediateValue - (address + 4); 
            }
        } else if(literalMatcher.find()) { 
          instruct.immediateValue = Tools.processNumber(literalMatcher.group(0).replace(" ", ""));
        } else {
          instruct.r1 = instruct.rd;
          instruct.rd = 0;
        }

        return instruct;
  }
}
