(ns assembler.core (:use clojure.string) (:use clojure.core.match) (:use assembler.opcodes) (:use assembler.registers))


(defn -main[& x]
  (if (zero? (count x)) (do 
      (println "pass a dlx assembly file to the program as a parameter")
      1                                               
    ) 
    (do  
      (def lines 
        (with-open [rdr (clojure.java.io/reader (first x))]
          (reduce conj [] (line-seq rdr)))
      )
      
      ; 
      (defn parseI[line] 
        (def opcode (re-find (re-pattern (matchOpcodes (itypeopcodes))) line))
        ; based on the value of the immediate, we'll either have to look up in the symbol table or translate a literal value
        (def immediate (re-find #".*\\ .*$" line))
        (join " " ["immediate: " immediate "opcode: " opcode])
      )

      (defn writeout[x]) 
      ;this function figures out where exectly the directive wants to put the contents
      (defn currentMemoryLocation[lines, line]
        (reduce (fn[acc, l] 
            (if (<= (indexOf l) (indexOf line)
              (do 
                ;match a bunch of things
                (def textDirective (re-pattern "^\\.text?(\\ \d+)"))
                (def 
                (cond 
                  (re-matches textDirective l) (do
                    (re-find (re-pattern "\d+"))
                          
                  )
                  (re-matches dataDirective l) (do
                     
                  )
                )
              )
            )
          )
        )
        0
        lines
      )
    )
      
      
      (defn buildSymbolTable[lines] 


      )
      ; should get run after we build the symbol table 
      (defn parseLine[line] 
         (def directivepattern (re-pattern "^\\..*"))
         (def itypepattern (re-pattern (join "" ["^" (matchOpcodes (itypeopcodes)) ".*"])))     
          (cond  
            (re-matches directivepattern line) "directive" 
            (re-matches itypepattern line) (parseI line) 
            :else "other" 
          ) 
      )
      (println (map (fn[x] (parseLine x)) lines)) 
      )
  )
)

