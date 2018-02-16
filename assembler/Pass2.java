import java.util.Set;
import java.util.HashMap;
import java.util.Formatter;
import java.util.ArrayList;
public class Pass2
{
    public static void doPass2(Pass1Output output) {
        ArrayList<Integer> locations = output.getLocations();
        for(int i : locations) {
            System.out.println(getOutput(output, i));
        }
    }
    public static String getOutput(Pass1Output output, int address) {
        return (String.format("%08x", address) + ": " + output.getContentsAtMemoryLocation(address).toLowerCase() +  "      #" + output.getOriginal(address));
    }
}
