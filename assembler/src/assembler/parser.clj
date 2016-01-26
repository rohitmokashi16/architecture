(ns assembler.parser (:use assembler.opcodes))

(def patterns {
    :text (re-pattern "^\\.text?(\\ \\d+)")
    :data (re-pattern "^\\.data?(\\ \\d+)")
    :align (re-pattern "^\\.align\\ \\d+")
    :asciiz (re-pattern "^\\.asciiz\\ \".*\"((,\".*\")*)?")
    :float (re-pattern "^\\.float\\ \\d+((,\\d+)*)?")
    :word (re-pattern "^\\.word\\ \\d+((,\\d+)*)?")
    :space (re-pattern "^\\.space\\ \\d+")
    :label (re-pattern "^.*: ^.*")
    :instruction (re-pattern (clojure.string/join ["^" (matchOpcodes (keys opcodes)) ".*"]))
  }
)

(defn h2d[x] 
  (.parseInt Integer x)
)

(defn ^:dynamic pass1[line, contents, memLocation]
  (if (>= line (count contents)) 
    {}
    (do
      (def txt (get contents line)) 
      (cond              
        (re-matches (patterns :text) txt) (do
          (def n (re-find (re-pattern "\\d+")))
          (if (nil? n) (pass1 (+ 1 line) contents 0) (pass1 (+ 1 line) contents n))
        )
        (re-matches (patterns :data) txt) (do
          (def n (re-find (re-pattern "\\d+")))
          (if (nil? n) (pass1 (+ 1 line) contents (h2d "0x200")) (pass1 (+ 1 line) contents n))                          
        )
        (re-matches (patterns :align) txt) (do
          (def n (re-find (re-pattern "\\d+") txt))
          (def r (while (not (zero? (mod memLocation n))) (def memLocation (+ 1 memLocation))))
          (pass1 (+ 1 line) contents memLocation)
        )
        (re-matches (patterns :instruction) txt) (do
            (println (clojure.string/join ["on line " line ": " txt "storing in: " memLocation]))
            (assoc (pass1 (+ 1 line) contents (+ 4 memLocation)) memLocation (get contents line)) 
        )
        :else (pass1 (+ 1 line) contents memLocation) 
      )
    )       
  )
)


(defn runpass1[contents]
  (pass1 0 contents 0)
)
; convert a number hex to decimal

