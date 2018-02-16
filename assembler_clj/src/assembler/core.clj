(ns assembler.core
  (:use [clojure.tools :only [trace]]
	[clojure.string :only [split join]]
	)
)
(defn -main[& args]
  (if (zero? (count args)) (do
      (println "pass a dlx assembly file to the program as a parameter")
    )
    (do
      (def lines
        (with-open [rdr (clojure.java.io/reader (first args))]
          (reduce conj [] (line-seq rdr)))
      )
      (assembler.parser/pass1 0 lines 0)
    )
  )
)
(def x 3)


