public class SymbolTable {
  private static SymbolTable instance;
  public SymbolTable() {
    
  }
  public static SymbolTable getInstance() {
    if(instance == null) {
      instance = new SymbolTable();
    }
    return instance;
  }
}
