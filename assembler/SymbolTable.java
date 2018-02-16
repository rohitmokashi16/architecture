import java.util.HashMap;
public class SymbolTable {
    private HashMap<String, Integer> symbolTable;
    private static SymbolTable instance;
    private SymbolTable() {
        symbolTable = new HashMap<>();
    }
    public static synchronized SymbolTable getInstance() {
        if(instance == null) {
            instance = new SymbolTable();
        }
        return instance;
    }
    public int getSymbol(String symbol) {
        return symbolTable.get(symbol);
    }
    public boolean containsSymbol(String symbol) {
        return symbolTable.containsKey(symbol);
    }
    public void putSymbol(String symbol, int location) {
        symbolTable.put(symbol, location);
    }
    @Override
    public String toString() {
        return symbolTable.toString();
    }
}

