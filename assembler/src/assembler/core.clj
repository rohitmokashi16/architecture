(ns assembler.core (:use clojure.string) (:use clojure.core.match) (:use assembler.opcodes))

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

      (defn writeout[x]) 
      (defn matchType[line] 
         (def directivepattern (re-pattern "\\..*"))
         (def itypepattern (re-pattern (join "" [(getrex (itypeopcodes)) ".*"])))
         (cond  
            (re-matches directivepattern line) "directive" 
            (re-matches itypepattern line) "itype"
            :else "other" 
        ) 
      )
      (println (getrex (itypeopcodes)))
      (println (map (fn[line] (matchType line)) lines))
    )
  )
)

