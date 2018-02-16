import java.util.HashSet;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.Set;
public abstract class Instruction
{
    protected int icode;
    public static String rexCodes;
    public static String rexOpcodes() {
        if(rexCodes == null) {
            Set<String> jcodes = JType.getIcodes(), icodes = IType.getIcodes(), rcodes = RType.getIcodes();
            HashSet<String> codes = new HashSet<>();
            codes.addAll(jcodes);
            codes.addAll(icodes);
            codes.addAll(rcodes);
            rexCodes = rexOpcodes(codes);
        }
        return rexCodes;
    }
    protected void setIcode(int icode) {
        this.icode = icode;
    }
    protected int getIcode(int icode) {
        return this.icode;
    }
    public static String rexOpcodes(Set<String> codeSet) {
        String codes = "(";
        String[] codeArray = codeSet.toArray(new String[codeSet.size()]);
        for(int i = 0; i < codeArray.length - 1; i++) {
            codes += "\\b" + codeArray[i] + "\\b|";
        }
        codes += "\\b" + codeArray[codeArray.length - 1] + "\\b)";
        return codes;
    }  
    // Trying to do this with type coercion
    //
    public static Instruction parseInstruction(String instruction, int address) {
        String icode = getIcodeName(instruction);
        // we need the location of the instruction to be able to calculate pc relative addresses
        if(IType.getIcodes().contains(icode)) {
            return IType.parseInstruction(instruction, address);
        } else if (RType.getIcodes().contains(icode)) {
            return RType.parseInstruction(instruction);
        } else /** if (JType.getIcodes().contains(instruction.icode)) **/{
            return JType.parseInstruction(instruction, address);
        }
    }

    public static String getIcodeName(String instruction) {
        Pattern icodePattern = Pattern.compile(rexOpcodes());
        Matcher matcher = icodePattern.matcher(instruction);
        matcher.find();
        return matcher.group(0);

    }
    // We need an address for PC relative addressing
    public abstract int encode();
}
