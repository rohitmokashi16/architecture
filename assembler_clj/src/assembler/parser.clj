(ns assembler.parser (:require assembler.opcodes))

(def patterns {
								:text (re-pattern "^\\.text?(\\ \\d+)")
								:data (re-pattern "^\\.data.*")
								:align (re-pattern "^\\.align\\ \\d+")
								:asciiz (re-pattern "(.*\\ )?\\.asciiz\\s\".*\"(,\\ \".*\")*")
								:float (re-pattern "(.*:\\ )?\\.float.*")
								:double (re-pattern "(.*:\\ )?\\.double.*")
								:word (re-pattern "(.*:\\s+)?\\.word.*")
								:space (re-pattern "^\\.space\\s\\d+")
								:label (re-pattern "^.*:.*")
								:instruction (re-pattern (clojure.string/join [(opcodeList) ".*"]))
								}
	)
(def wordsize 4)

(defn h2d[x]
	(Integer/parseInt x)
	)

(defn parseNum[x]
	(if (re-matches (re-pattern "^-(0x)?(\\d|a|b|c|d|e|f)+") x)
		(* -1 (parseNum (.substring x 1)))
		((fn[hex, number]
			 (if hexter,
				 (Integer/parseInt (.substring (.replace number " " "") 2) 16)
				 (Integer/parseInt (.replace number " " ""))
				 )
			 )
		 (re-find (re-pattern "0x(\\d|a|b|c|d|e|f)+") x) x
		 )
		)
	)
(defn pass1[lines line table]
	(if (>= line (count lines))
		table
		(do

		)
	)
)
(defn calcAlignAddress[current desired]
	(if (zero? (mod current (Math/pow 2 desired)))
		current
		(calcAlignAddress (+ 1 current) desired)
		)
	)
(defn stringToHex[currentString desiredConversion]

	)

(defn stringToHex[currentString desiredConversion size]



(defn runpass1[contents]
	(pass1 0 contents 0)
	)

; convert a number hex to decimal

