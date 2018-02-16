import java.util.HashMap;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class RType extends Instruction {

    private int funcode, r1, r2, rd;
    private static HashMap<String, Integer> RMap = new HashMap<String, Integer>();
    static
    {
        RMap.put("nop", 0);
        RMap.put("sll", 0);
        RMap.put("srl", 0);
        RMap.put("sra", 0);
        RMap.put("add", 0);
        RMap.put("addu", 0);
        RMap.put("sub", 0);
        RMap.put("subu", 0);
        RMap.put("and", 0);
        RMap.put("or", 0);
        RMap.put("xor", 0);
        RMap.put("seq", 0);
        RMap.put("sne", 0);
        RMap.put("slt", 0);
        RMap.put("sgt", 0);
        RMap.put("sle", 0);
        RMap.put("sge", 0);
        RMap.put("movf", 0);
        RMap.put("movd", 0);
        RMap.put("movfp2i", 0);
        RMap.put("movi2fp", 0);
        RMap.put("addf", 1);
        RMap.put("subf", 1);
        RMap.put("multf", 1);
        RMap.put("divf", 1);
        RMap.put("addd", 1);
        RMap.put("subd", 1);
        RMap.put("multd", 1);
        RMap.put("divd", 1);
        RMap.put("cvtf2d", 1);
        RMap.put("cvtf2i", 1);
        RMap.put("cvtd2f", 1);
        RMap.put("cvtd2i", 1);
        RMap.put("cvti2f", 1);
        RMap.put("cvti2d", 1);
        RMap.put("mult", 1);
        RMap.put("div", 1);
        RMap.put("multu", 1);
        RMap.put("divu", 1);
    }
    private static HashMap<String, Integer> zeroMap = new HashMap<>();
    private static HashMap<String, Integer> oneMap = new HashMap<>();
    static
    {
        zeroMap.put("nop", 0);
        zeroMap.put("sll", 4);
        zeroMap.put("srl", 6);
        zeroMap.put("sra", 7);
        zeroMap.put("add", 32);
        zeroMap.put("addu", 33);
        zeroMap.put("sub", 34);
        zeroMap.put("subu", 35);
        zeroMap.put("and", 36);
        zeroMap.put("or", 37);
        zeroMap.put("xor", 38);
        zeroMap.put("seq", 40);
        zeroMap.put("sne", 41);
        zeroMap.put("slt", 42);
        zeroMap.put("sgt", 43);
        zeroMap.put("sle", 44);
        zeroMap.put("sge", 45);
        zeroMap.put("movf", 50);
        zeroMap.put("movd", 51);
        zeroMap.put("movfp2i", 52);
        zeroMap.put("movi2fp", 53);

        oneMap.put("addf", 0);
        oneMap.put("subf", 1);
        oneMap.put("multf", 2);
        oneMap.put("divf", 3);
        oneMap.put("addd", 4);
        oneMap.put("subd", 5);
        oneMap.put("multd", 6);
        oneMap.put("divd", 7);
        oneMap.put("cvtf2d", 8);
        oneMap.put("cvtf2i", 9);
        oneMap.put("cvtd2f", 10);
        oneMap.put("cvtd2i", 11);
        oneMap.put("cvti2f", 12);
        oneMap.put("cvti2d", 13);
        oneMap.put("mult", 14);
        oneMap.put("div", 15);
        oneMap.put("multu", 22);
        oneMap.put("divu", 23);
    }

    /* 
     * Function    : parseInstruction
     * Description : Parses the RType instruction and returns the encoding.
     */
    public static RType parseInstruction(String instruction) {
        RType instruct = new RType();
        int icode = RMap.get(Instruction.getIcodeName(instruction));
        instruct.setIcode(icode);
        if(icode == 1) {
            instruct.funcode = oneMap.get(Instruction.getIcodeName(instruction));
        } else {
            instruct.funcode = zeroMap.get(Instruction.getIcodeName(instruction));
        }
        // now break off the registers
        Pattern registers = Pattern.compile(Registers.registerString());
        Matcher registerMatcher = registers.matcher(instruction);
        if (registerMatcher.find()) {
          instruct.rd = Registers.registerValue(registerMatcher.group(0));
        }
        if (registerMatcher.find()) {
          instruct.r1 = Registers.registerValue(registerMatcher.group(0));
        }
        if(registerMatcher.find()) {
            instruct.r2 = Registers.registerValue(registerMatcher.group(0));
        }
        return instruct;
    }
    
    public static Set<String> getIcodes() {
        return RMap.keySet();
    }
    
    @Override
    public int encode()
    {
        return ((icode << 26) | (r1 << 21) | (r2 << 16) | (rd << 11) | funcode);
    }
}
