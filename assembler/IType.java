import java.util.HashMap;
import java.util.Set;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

public class IType extends Instruction
{
    protected int immediateValue, r1, rd;
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

    public static Set<String> getIcodes() {
        return IMap.keySet();
    }
    
    /* 
     * Function    : parseInstruction
     * Description : Parses the IType instruction by checking what kind of addressing and displacments are needed.
     */
    public static IType parseInstruction(String instruction, int location) {
      Matcher leftImmediateMatcher = ITypeLeftImmediate.pattern().matcher(instruction);
      Matcher leftDisplacementMatcher = ITypeLeftDisplacement.pattern().matcher(instruction);  
      Matcher rightDisplacementMatcher = ITypeRightDisplacement.pattern().matcher(instruction);
        if (leftDisplacementMatcher.find()) {
          return ITypeLeftDisplacement.parseInstruction(instruction);
        } else if (leftImmediateMatcher.find()) {
          return ITypeLeftImmediate.parseInstruction(instruction); 
        } else if(rightDisplacementMatcher.find()) {
          return ITypeRightDisplacement.parseInstruction(instruction);
        } else {
          return ITypeImmediate.parseInstruction(instruction, location);
        }
    }

    @Override
    public int encode() {
      return ((icode << 26) | (r1 << 21) | (rd << 16) | (immediateValue & 0xffff));
    }
}
