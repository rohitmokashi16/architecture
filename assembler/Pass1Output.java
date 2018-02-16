import java.util.HashMap;
import java.util.ArrayList;
import java.util.Collections;
public class Pass1Output {
    private HashMap<Integer,String> instructions;
    private HashMap<Integer,String> data;
    private HashMap<Integer,String> originals;
    public Pass1Output() {
        instructions = new HashMap<>();
        data = new HashMap<>();
        originals = new HashMap<>();
    }
    public void putInstruction(int address, String instruction) {
        instructions.put(address, instruction);
    }
    public void putData(int address, String content) {
        data.put(address, content);
    }
    public void putOriginal(int address, String content) {
        originals.put(address,content);
    }
    public String getOriginal(int address) {
        return originals.get(address);
    }
    public ArrayList<Integer> getLocations() {
        ArrayList<Integer> keys = new ArrayList<>();
        keys.addAll(instructions.keySet());
        keys.addAll(data.keySet());
        Collections.sort(keys);
        return keys;
    }
    public String getContentsAtMemoryLocation(int address) {
        if(instructions.containsKey(address)) {
            return String.format("%08x", Instruction.parseInstruction(instructions.get(address), address).encode());
        } else if (data.containsKey(address)) {
            return data.get(address);
        } else {
            return "";
        }
    }
    public String toString() {
        return "Instructions: " + instructions.toString() + "Data: " + data.toString();
    }
}
