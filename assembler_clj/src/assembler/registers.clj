(ns assembler.registers (:use [clojure.string :only [join split]]))
(def gpregs {
  :R0 0
  :R1 1
  :R2 2
  :R3 3
  :R4 4
  :R5 5
  :R6 6
  :R7 7
  :R8 8
  :R9 9
  :R10 10
  :R11 11
  :R12 12
  :R13 13
  :R14 14
  :R15 15
  :R16 16
  :R17 17
  :R18 18
  :R19 19
  :R20 20
  :R21 21
  :R22 22
  :R23 23
  :R24 24
  :R25 25
  :R26 26
  :R27 27
  :R28 28
  :R29 29
  :R30 30
  :R31 31
  }
)
(def fpregs {
	:F0 0
	:F1 1
	:F2 2
  :F3 3
  :F4 4
	:F5 5
	:F6 6
	:F7 7
	:F8 8
	:F9 9
	:F10 10
})

(defn registerValue[v]
	(let [changed (keyword (.toUpperCase v))]
			(if (contains? gpregs changed)
				(get gpregs changed)
				(get fpregs changed)
			)
	)
)

(defn registerString[]
	(join [ "("
		(reduce (fn[acc x] (join [acc "\\b" x "\\b|"])) ""
			(map (fn[x](.toLowerCase (name x))) (keys (merge gpregs fpregs)))
		) "f11)" ]
	)
)


