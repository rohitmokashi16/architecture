import java.util.regex.Pattern;
import java.util.regex.Matcher;
import java.util.HashMap;
public class ITypeRightDisplacement extends IType { 
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

  
  public static Pattern pattern() {
    return Pattern.compile(Instruction.rexOpcodes(IMap.keySet()) + "\\s" + Registers.registerString() + ",\\s(0x)?\\d+\\(" + Registers.registerString() +"\\)");
  }
  
   /* 
     * Function    : parseInstruction
     * Description : Parses the IType instruction that uses left displacement.
     */
  public static ITypeRightDisplacement parseInstruction(String instruction) {
    ITypeRightDisplacement instruct = new ITypeRightDisplacement();
    instruct.icode = IMap.get(Instruction.getIcodeName(instruction));
    Pattern registerPattern = Pattern.compile(Registers.registerString());
    Matcher registerMatcher = registerPattern.matcher(instruction);
    registerMatcher.find();
    instruct.rd = Registers.registerValue(registerMatcher.group(0));
    registerMatcher.find();
    instruct.r1 = Registers.registerValue(registerMatcher.group(0));
    Pattern immediatePattern = Pattern.compile("\\b(0x)?\\d+");
    Matcher immediateMatcher = immediatePattern.matcher(instruction);
    immediateMatcher.find();
    instruct.immediateValue = Tools.processNumber(immediateMatcher.group(0));
    return instruct;
  }





}
