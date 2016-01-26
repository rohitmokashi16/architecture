(ns assembler.symboltable)

(defn buildSymbolTable[contents]
  (reduce (fn[acc, x] (conj acc (processLine (buildSubTable contents x) x))) [] contents)
)
(defn buildSubTable[contents, line]
  (filter (fn[l] (<= (indexOf contents l) (indexOf contents line))) contents)
)
(defn processLine[table, line]
  (def mem (currentMemoryAddress table))
)
(defn currentMemoryAddress[table]

)
(defn currentInstructionAddress[table]

)
