(ns assembler.opcodes)
  (def defs {
                :SPECIAL 0
                :FPARITH 1
                :J 2 
                :JAL 3
                :BEQZ 4
                :BNEZ 5
                :BFPT 6
                :BFPF 7
                :ADDI 8
                :ADDUI 9
                :SUBI 10
                :SUBUI 11
                :ANDI 12
                :ORI 13
                :XORI 14
                :LHI 15
                :RFE 16
                :TRAP 17
                :JR 18
                :JALR 19
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
  })
(defn getrex[]
  (clojure.string/join ")" [(clojure.string/join "(" ["" (reduce (fn[acc, x] (clojure.string/join "|" [x acc])) (map (fn[x] (name x)) (keys assembler.opcodes/defs)))]) ""])
 
)
