(ns assembler.parser (:use assembler.opcodes))

(def patterns {
    :text (re-pattern "^\\.text?(\\ \\d+)")
    :data (re-pattern "^\\.data?(\\ \\d+)")
    :align (re-pattern "^\\.align\\ \\d+")
    :asciiz (re-pattern "(.*\\ )?\\.asciiz\\ \".*\"(,\\ \".*\")*")
    :float (re-pattern "(.*:\\ )?\\.float\\ \\d+(,\\ -?\\d+)*")
    :word (re-pattern "(.*:\\s+)?\\.word\\ -?\\d+(,\\ -?\\d+)*")
    :space (re-pattern "^\\.space\\ \\d+")
    :label (re-pattern "^.*: ^.*")
    :instruction (re-pattern (clojure.string/join ["(.*:\\ )?\\ *" (matchOpcodes (keys opcodes)) ".*"]))
  }
)

(defn h2d[x] 
  (Integer/parseInt x)
)

(defn pass1[line, contents, memLocation]
  (if (>= line (count contents)) 
    {}
    (do
      (let [txt (get contents line)] 
      (cond              
        (re-matches (patterns :text) txt) (do
          (let [n (re-find (re-pattern "-?\\d+"))]
            (if (nil? n) (pass1 (+ 1 line) contents 0) (pass1 (+ 1 line) contents n))
          )
        )
        (re-matches (patterns :data) txt) (do
          (let [n (re-find (re-pattern "\\d+"))]
            (if (nil? n) (pass1 (+ 1 line) contents (h2d "0x200")) (pass1 (+ 1 line) contents n))                          
          )
        )
        (re-matches (patterns :align) txt) (do
          (let [n (h2d (re-find (re-pattern "-?\\d+") txt))]
            (pass1 (+ 1 line) contents (+ memLocation (mod memLocation n)))
          )
        )
        (re-matches (patterns :word) txt) (do 
          (let [numbers (clojure.string/split (first (re-find (re-pattern "\\ -?\\d+(,\\ -?\\d+)*") txt)) #",")]
      ;   (def result (pass1 (+ 1 line) contents (+ (* 8 (count words)) memLocation)))
            ((fn[c, y,nums, result] 
               (if (= y c)
                  result
                  (recur c (+ 1 y) nums (assoc result (+ memLocation (* 8 y)) (get nums y)))
                )
              )
               (count numbers)
               0 
              numbers 
              (pass1 (+ 1 line) contents (+ (* 8 (count numbers)) memLocation))
            )
          )
        ) 
        (re-matches (patterns :space) txt) (do
          (let [n (h2d (re-find (re-pattern "\\d+") txt))]
            (pass1 (+ line 1) contents (+ memLocation n)) 
          )
        )
        (re-matches (patterns :instruction) txt) (do
          (assoc (pass1 (+ 1 line) contents (+ 4 memLocation)) memLocation 
                 (first 
                    (re-find 
                      (re-pattern (clojure.string/join [(matchOpcodes (keys opcodes)) ".*"]) 
                                  ) txt))
                 )
                                                   )
        :else (pass1 (+ 1 line) contents memLocation) 
          )
        )
      )       
    )
  )


(defn runpass1[contents]
  (pass1 0 contents 0)
  )
; convert a number hex to decimal

