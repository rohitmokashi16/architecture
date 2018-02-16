(ns assembler.opcodes (:use [clojure.string :only [join split]]))


(defn itypeopcodes[]
  { :BEQZ 4
		:BNEZ 5
		:ADDI 8
		:ADDUI 9
		:SUBI 10
		:ANDI 12
		:ORI 13
		:XORI 14
		:LHI 15
		:TRAP 17
		:JR 18
		:JALR 19
		:SLLI 20
		:SRLI 22
		:SRAI 23
		:SEQI 24
		:SNEI 25
		:SLTI 26
		:SGTI 27
		:SLEI 28
		:SGEI 29
		:LB 32
		:LH 33
		:LW 35
		:LBU 36
		:LHU 37
		:LF 38
		:LD 39
		:SB 40
		:SH 41
		:SW 43
		:SF 46
		:SD 47
		}
)
(def jtypeopcodes
	{ :J  2
		:JAL 3
	}
)
(def rtypeopcodes
	{ :NOP 0
		:SLL 0
		:SRL 0
		:SRA 0
		:ADD 0
		:ADDU 0
		:SUB 0
		:SUBU 0
		:AND 0
		:XOR 0
		:OR 0
		:SEQ 0
		:SNE 0
		:SLT 0
		:SGT 0
		:SLE 0
		:SGE 0
		:MOVF 0
		:MOVD 0
		:MOVFP2I 0
		:MOVI2FP 0
		:ADDF 1
		:SUBF 1
		:MULTF 1
		:DIVF 1
		:ADDD 1
		:SUBD 1
		:MULTD 1
		:DIVD 1
		:CVTF2D 1
		:CVTF2I 1
  	:CVTD2F 1
		:CVTD2I 1
		:CVTI2F 1
		:CVTI2D 1
		:MULT 1
		:DIV 1
		:MULTU 1
		:DIVU 1
	}
)


(def rtypefuncodes
	{
		:NOP 0
		:SLL 4
		:SRL 6
		:SRA 7
		:ADD 32
		:ADDU 33
		:SUB 34
		:SUBU 35
		:AND 36
		:XOR 38
		:OR 37
		:SEQ 40
		:SNE 41
		:SLT 42
		:SGT 43
		:SLE 44
		:SGE 45
		:MOVF 50
		:MOVD 51
		:MOVFP2I 52
		:MOVI2FP 53
		:ADDF 0
		:SUBF 1
		:MULTF 2
		:DIVF 3
		:ADDD 4
		:SUBD 5
		:MULTD 6
		:DIVD 7
		:CVTF2D 8
		:CVTF2I 9
  	:CVTD2F 10
		:CVTD2I 11
		:CVTI2F 12
		:CVTI2D 13
		:MULT 14
		:DIV 15
		:MULTU 22
		:DIVU 23
	}
)


(defn gettype[opcode]
	(cond
		(contains? rtypeopcodes opcode) :RTYPE
		(contains? jtypeopcodes opcode) :JTYPE
		(contains? itypeopcodes opcode) :ITYPE
	)
)
(gettype :DIVU)

(defn opcodeList[]
	(let [opcodes (merge (merge rtypeopcodes jtypeopcodes) itypeopcodes)]
	  (join ["(" (reduce (fn[acc, x] (join [acc "\\b" x "\\b|"])) "" (map (fn[x] (.toLowerCase (name x))) (keys opcodes))) "\\bnop\\b)"])
	)
)
