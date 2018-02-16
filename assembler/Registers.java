import java.util.HashMap;
import java.util.Set;
import java.util.HashSet;
public class Registers {
    private static HashMap<String, Integer> gprs = new HashMap<>();
    static {
        gprs.put("r0", 0);
        gprs.put("r1", 1);
        gprs.put("r2", 2);
        gprs.put("r3", 3);
        gprs.put("r4", 4);
        gprs.put("r5", 5);
        gprs.put("r6", 6);
        gprs.put("r7", 7);
        gprs.put("r8", 8);
        gprs.put("r9", 9);
        gprs.put("r10", 10);
        gprs.put("r11", 11);
        gprs.put("r12", 12);
        gprs.put("r13", 13);
        gprs.put("r14", 14);
        gprs.put("r15", 15);
        gprs.put("r16", 16);
        gprs.put("r17", 17);
        gprs.put("r18", 18);
        gprs.put("r19", 19);
        gprs.put("r20", 20);
        gprs.put("r21", 21);
        gprs.put("r22", 22);
        gprs.put("r23", 23);
        gprs.put("r24", 24);
        gprs.put("r25", 25);
        gprs.put("r26", 26);
        gprs.put("r27", 27);
        gprs.put("r28", 28);
        gprs.put("r29", 29);
        gprs.put("r30", 30);
        gprs.put("r31", 31);
    }
    public static HashMap<String, Integer> fprs = new HashMap<>();
    static {

        fprs.put("f0", 0);
        fprs.put("f1", 1);
        fprs.put("f2", 2);
        fprs.put("f3", 3);
        fprs.put("f4", 4);
        fprs.put("f5", 5);
        fprs.put("f6", 6);
        fprs.put("f7", 7);
        fprs.put("f8", 8);
        fprs.put("f9", 9);
        fprs.put("f10", 10);
        fprs.put("f11", 11);
        fprs.put("f12", 12);
        fprs.put("f13", 13);
        fprs.put("f14", 14);
        fprs.put("f15", 15);
        fprs.put("f16", 16);
        fprs.put("f17", 17);
        fprs.put("f18", 18);
        fprs.put("f19", 19);
        fprs.put("f20", 20);
        fprs.put("f21", 21);
        fprs.put("f22", 22);
        fprs.put("f23", 23);
        fprs.put("f24", 24);
        fprs.put("f25", 25);
        fprs.put("f26", 26);
        fprs.put("f27", 27);
        fprs.put("f28", 28);
        fprs.put("f29", 29);
        fprs.put("f30", 30);
        fprs.put("f31", 31);
    }
    public static int registerValue(String registerName) {
        if(gprs.keySet().contains(registerName)) {
            return gprs.get(registerName);
        } else if (fprs.keySet().contains(registerName)) {
            return fprs.get(registerName);
        }
        return 0;
    }
    
    public static String registerString() {
        String registerString = "(";
        HashSet<String> sumRegisters = new HashSet<String>();
        sumRegisters.addAll(fprs.keySet());
        sumRegisters.addAll(gprs.keySet());
        String[] registerKeys = sumRegisters.toArray(new String[sumRegisters.size()]);
        for(int i = 0; i < registerKeys.length - 1; i++) {
            registerString += "\\b" + registerKeys[i].toLowerCase() + "\\b|";
        }
        registerString += "\\b" + registerKeys[registerKeys.length - 1].toLowerCase() + "\\b)";
        return registerString;
    }
}
