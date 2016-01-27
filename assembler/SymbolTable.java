import java.util.HashMap;
import java.io.BufferedReader;
import java.io.FileReader;



public class SymbolTable {

  private HashMap<String, Integer> map;
  private static SymbolTable instance;
  
  public SymbolTable() {
      map = new HashMap<>();
  }

  public static SymbolTable getInstance() {
    if(instance == null) {
      instance = new SymbolTable();
    }
    return instance;
  }

  public void readFile(String filename) {
      
  }
  
  public int lookupSymbol(String symbolName) {
    return map.get(symbolName);
  }
}
