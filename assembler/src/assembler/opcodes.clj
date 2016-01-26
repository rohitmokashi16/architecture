(ns assembler.opcodes (:use clojure.string))
  (def opcodes {
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
(def funcodes {
      :SLLI 0
      :SRLI 2
      :SRAI 3
      :SLL 4
      :SRL 6
      :SRA 7
      :TRAP 12
      :ADD 20
      :ADDU 21
      :SUB 22
      :SUBU 23
      :AND 24
      :OR 25
      :XOR 26
      :SEQ 28
      :SNE 29
      :SLT 30
      :SGT 31
      :SLE 32
      :SGE 33
})
(defn itypeopcodes[]
  [ :ADDI :ADDUI :SUBI :SUBUI :ANDI :ORI :XORI :SGTI :SLTI :SGEI :SEQI :SNEI ]
)
(defn rtypeopcodes[]

)
(defn matchOpcodes[p]
  (join "" ["(" (reduce (fn[acc, x] (join "|" [(lower-case x) (lower-case acc)])) (map (fn[x] (name x)) p)) ")"])
)
