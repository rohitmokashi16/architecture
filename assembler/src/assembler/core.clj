(ns assembler.core (:use clojure.core.match) (:use assembler.opcodes))

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
      (defn stripTrailingWhiteSpace[line]
        (defn strip[line] 
          (if (= (.charAt line 0) " ") 
            (strip (.substring line 1))
            line
          )
        )
        (strip line)  
      ) 
      (defn itype[line] )
      (defn rtype[line] )
      (defn jtype[line] )
      (defn directive[line] )
      (defn matchType[line] 
        (if (re-matches #"(addi|subi)\ " line) "directive" "error")
      )
      (println getrex)
    )
  )
)

