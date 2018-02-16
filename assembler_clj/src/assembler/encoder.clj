(ns assembler.encoder (:require assembler.registers) (:use [clojure.string :only [join split]]))
(def ocpattern (re-pattern (assembler.opcodes/opcodeList)))
(defn inst[x]
	x
)

(defn do_inst[line symbols]
	(format "%08x" (getInstructionEncoder line symbols))
)

(defn getInstructionEncoder[line symbols]
	(let [opcode (keyword (.toUpperCase (first (re-find ocpattern line))))]
		(case (assembler.opcodes/gettype opcode)
			:ITYPE 	(itypeEncoder line symbols)
			:JTYPE 	(jtypeEncoder line symbols)
			:RTYPE 	(rtypeEncoder line symbols opcode)
		)
	)
)


(defn rtypeEncoder [line symbols opcode]
	(let [registers (map (fn[x] (first x)) (re-seq (re-pattern (assembler.registers/registerString)) line))]
		(bit-or (bit-shift-left opcode 26) (bit-shift-left (registers 0) 21) (bit-shift-left (registers 1) 16) (bit-shift-left (registers 2) 11) (get assembler.opcodes/rtypefuncodes opcode))
	)
)

(defn jtypeEncoder [line symbols]

)
(defn itypeEncoder [line symbols]
	4
)

(def x (rtypeEncoder "add r3, r1, r4" {} :ADD))
(x)


(defn enc[output]
	(let [k (sort < ( keys (merge (output :data) (output :instructions))))]

		(reduce (fn[acc x] (do

				(let [f (join [acc (format "%08x" x) ": "])]
						(cond
						 (contains? (output :data) x) (do
								(join [f (get (output :data) x) "    #" (get (output :original) x) "\n"])
						 )
						 (contains? (output :instructions) x) (do
								(join [f (do_inst (get (output :instructions) x) (output :symbols)) (get (output :original) x) "\n"])
						 )
						:else (join [f "\n"])
					  )

				)
			)
		)
		""
		k
	)
	)
)

(enc (assembler.core/-main "/Users/taylor/projects/architecture/assembler_clj/data.dlx"))
