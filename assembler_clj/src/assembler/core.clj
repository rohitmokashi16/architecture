(ns assembler.core (:use clojure.tools.trace) (:use clojure.string) (:use clojure.core.match) (:use assembler.parser) (:use assembler.opcodes) (:use assembler.registers))


(defn -main[& x]
  (defn makeFilename[x]

  )
  (if (zero? (count x)) (do
      (println "pass a dlx assembly file to the program as a parameter")
      1
    )
    (do
      (def lines
        (with-open [rdr (clojure.java.io/reader (first x))]
          (reduce conj [] (line-seq rdr)))
      )
    (println (pass1 0 lines 0))
    )
)
)

