;*****************************************************************
;* HW01_121044029_Mehmet_Gurol_CAY                               *
;*                                                               *
;* File:  main.asm                                               *
;* Author:Mehmet Gürol ÇAY                                       *
;*                                                               *
;* Freescale CodeWarrior for the HC12 Program directory          *
;*                                                               *
;*****************************************************************

; export symbols
                    XDEF Entry, _Startup            ; export 'Entry' symbol
                    ABSENTRY Entry                  ; for absolute assembly: mark this as application entry point



; Include derivative-specific definitions 
		                INCLUDE 'derivative.inc' 

ROMStart            EQU   $4000             ; absolute address to place my code/constant data

; variable/data section 
PlusSign            EQU   $2B
MinusSign           EQU   $2D
DotSign             EQU   $2E
AssingSign          EQU   $3D
WhiteSpace          EQU   $20
DecPartNum          EQU   $2                 ;Decimal part has max two number
IntPartNum          EQU   $3                 ;Integer part has max three number
SubstractedValue    EQU   $30               ; substracted 30, because of numbers is hexadecimal
Zero                EQU   $0


; variable/data address section
StringAddres        EQU   $1200
FirstIntPart        EQU   $1500
FirstDecPart        EQU   $1510
SecondIntPart       EQU   $1520
SecondDecPart       EQU   $1525
Counter             EQU   $1530
ResultIntPart       EQU   $1535
ResultDecPart       EQU   $1540
FirstIntDigitNum    EQU   $1550
SecondIntDigitNum   EQU   $1560
Carry               EQU   $1600
Flag                EQU   $1605
Temp                EQU   $1606
TempAdd             EQU   $1610
TempCount           EQU   $1615
LowerDigitNum       EQU   $1620

                    ORG   RAMStart
; Input initialize

                    ORG   StringAddres
InputStr            FCC   "2.12 + 24.36 ="

                    ORG   Counter
                    DC.B  0
                    
                    ORG   Carry
                    DC.B  0
                    
                    ORG   Flag
                    DC.B  0
                    
                    ORG   FirstIntDigitNum
                    DC.B  0
                    
                    ORG   SecondIntDigitNum
                    DC.B  0
                    
                    ORG   Temp
                    DC.B  0
              
                    ORG   TempCount
                    DC.B  0
                    
                    ORG   LowerDigitNum
                    DC.B  0

; code section

                    ORG   ROMStart

Entry:
_Startup:
                    LDS   #RAMEnd+1         ; initialize the stack pointer

                    CLI                     ; enable interrupts
                    LDX   #StringAddres     ; load string adress to X accumulator
                    LDY   #FirstIntPart     ; load first integer part addres to accumulator Y
                    JSR   ParseInputs       ; call subroutin called ParseInputs
                    JSR   FixValues         ; it gets 30 out of every value
                    MOVB  #$0,Flag          ; clear flag
                    MOVB  #$0,Counter       ; clear counter
                    JSR   FixDecValues      ; it gets 30 out of every value
                    JSR   SumDecOperation   ; call subroutin is called SumDecOperation for addition decimal part
                    MOVB  #$0,Counter       ; clear counter
                    JSR   SumIntOperation   ;              
                    JSR   StoreResult       ; the result stored at $1500
              

                    JMP   $                        
;ParseInputs subroutin
ParseInputs:
                    LDAA  0,X               ; get an element from input string 
                    CMPA  #WhiteSpace       ; check white space
                    BNE   StoreFirstValue   ; when character isn't white space, go to StoreFirstValue
GO:                 INX                     ; increment input string address
                    JMP   ParseInputs       ; get other element 
StoreFirstValue:
                    CMPA  #PlusSign         ; check plus sign
                    BEQ   GetSecondInput    ; if char is plus sign get the second value
                    CMPA  #DotSign          ; check dot sign
                    BEQ   GetDecimalPart    ; if char is dot sign get the decimal part of first value
                    STAA  0,Y               ; store number which is included address by Y accumulator
                    INY                     ; increment address on the Y
                    INC   FirstIntDigitNum  ; count the digit of the first integer part
                    JMP   GO                ; go back to get another element in the input string
GetDecimalPart:
                    LDY   #FirstDecPart     ; load addres decimal part of first value
GoDec1:             INX                     ; increment input string address
                    LDAA  0,X               ; get an element from input string 
                    CMPA  #WhiteSpace       ; check white space
                    BEQ   StoreDecFirst     ; if there is a dot and there is a space after the dot
                                            ; ,store the space as zero 
                    STAA  0,Y               ; char is stored to decimal part addres of first value
                    INC   Counter           ; this counter is for decimal part num, decimal part has
                                            ; maximum 2 number
                    LDAB  Counter           ; 
                    CMPB  #DecPartNum       ; check decimal part equal number to counter
                    BEQ   GO                ; if check is true go to 'GO' and get other string element
                    INY 
                    JMP   GoDec1            ; if not get the other number of decimal part

StoreDecFirst:
                    MOVB  #$30,Y            ; store zero for white space 
                    INY                     ; increment decimal part address of first value
                    INC   Counter           ; this counter is hold for decimal part num
                    LDAB  Counter           ; counter is loaded to accumulator B
                    CMPB  #DecPartNum       ; check counter equal to max decimal part num
                    BEQ   GO                ; if true go to 'GO' and get other element
                    INY                     ; if not increment decimal part address
                    JMP   GoDec1            ; and get other decimal part number
                            
GetSecondInput:                       
                    MOVB  #$0,Counter       ; clear Counter
                    LDY   #SecondIntPart    ; load second integer part addres to accumulator Y
                    INX                     ; increment input string address
StartSecond:        LDAA  0,X               ; get an element from input string
                    CMPA  #WhiteSpace       ; check white space
                    BNE   StoreSecondValue  ; when char isn't white space, go to StoreSecondValue
GOSecond:           INX                     ; increment input string address
                    JMP   StartSecond       ; go back to get another element in the input string
StoreSecondValue:
                    CMPA  #AssingSign       ; check assign sign
                    BEQ   ReturnParseInputs ; if true, end of the string
                    CMPA  #DotSign          ; check dot sign
                    BEQ   GetDecPartSecondV ; if char is dot sign, get the decimal part of second value
                    STAA  0,Y               ; store number which is included address by Y accumulator
                    INC   SecondIntDigitNum ; count the digit of the secont integer part
                    INY                     ; increment address on the Y
                    JMP   GOSecond          ; go back to get another element in the input string

GetDecPartSecondV:
                    LDY   #SecondDecPart    ; load address decimal part of second value
GoDec2:             INX                     ; increment input string address
                    LDAA  0,X               ; get an element from input string 
                    CMPA  #WhiteSpace       ; check white space
                    BEQ   StoreDecSecond    ; if there is a dot and there is a space after the dot
                                            ; ,store the space as zero
                    STAA  0,Y               ; char is stored to decimal part address of second value
                    INC   Counter           ; this counter is for decimal part num, decimal part has
                                            ; maximum 2 number
                    LDAB  Counter           ; 
                    CMPB  #DecPartNum       ; check decimal part equal number to counter
                    BEQ   GOSecond          ; if check is true go to 'GOSecond' and get other string element
                    INY                     ;
                    JMP   GoDec2            ; if not get the other number of decimal part
StoreDecSecond:
                    MOVB  #$30, Y           ; store zero for white space
                    INY                     ; increment decimal part address of first value
                    INC   Counter           ; this counter is for decimal part num
                    LDAB  Counter           ;
                    CMPB  #DecPartNum       ; check decimal part equal number to counter
                    BEQ   GOSecond          ; if check is true go to 'GOSecond' and get other string element
                    INY                     ; increment address on the Y
                    JMP   GoDec2            ; go back to get another element in the input string 
                                          
ReturnParseInputs:
                    RTS
;End of Subroutin

;FixValues subroutin
FixValues:
                    MOVB  #$0,Counter
                    LDX   #FirstIntPart
                    LDAB  FirstIntDigitNum
                    STAB  Temp                 
                    BRA   SFixVal
                    

SecondVal:          LDAA  #$1
                    CMPA  Flag
                    BEQ   ReturnFixValues                    
                    LDAB  SecondIntDigitNum
                    STAB  Temp
                    MOVB  #$1,Flag
                    MOVB  #$0,Counter
                    LDX   #SecondIntPart
                    
SFixVal             LDAA  0,X
                    SUBA  #SubstractedValue
                    STAA  0,X
                    INX
                    INC   Counter
                    LDAA  Counter
                    CMPA  Temp
                    BEQ   SecondVal
                    BRA   SFixVal
 ReturnFixValues:
                    RTS                   
;End of Subroutin

;FixDecValues subroutin
FixDecValues:     
                    MOVB  #$0,Counter
                    LDX   #FirstDecPart
                    BRA   SFixDecValues

SecondDecVal:       LDAA  #$1
                    CMPA  Flag
                    BEQ   ReturnFixDevValues
                    MOVB  #$1,Flag
                    MOVB  #$0,Counter
                    LDX   #SecondDecPart
                    
SFixDecValues:      LDAA  0,X
                    SUBA  #SubstractedValue
                    STAA  0,X
                    INX
                    INC   Counter
                    LDAA  #DecPartNum
                    CMPA  Counter
                    BEQ   SecondDecVal
                    BRA   SFixDecValues                                     
 ReturnFixDevValues:
                    RTS
;End of Subroutin


;SumDecOperation subroutin  for decimal part addition
SumDecOperation:
                    MOVB  #$0,Carry         ; clear carry
                    MOVB  #$0,Counter       ; counter initialize with zero
                    LDX   #FirstDecPart     ; load address to X which has first decimal part address
                    LDY   #SecondDecPart    ; load address to Y which has second decimal part address
                    
;This loop goes to the last element of the decimal part                    
Loop:               INC   Counter
                    LDAA  Counter
                    CMPA  #DecPartNum
                    BEQ   setUpResultAdd
                    INX
                    INY
                    BRA   Loop
;End of loop
setUpResultAdd:
                    MOVB  #$0,Counter
                    STY   Temp              ; store address of second decimal value
                    LDY   #ResultDecPart    ; load result address
Loop2:              INC   Counter
                    LDAA  Counter
                    CMPA  #DecPartNum       ;
                    BEQ   StartSumOp
                    INY   
                    BRA   Loop2    

StartSumOp:         
                    STY   TempAdd           ; store result address at the tempAdd
                    LDY   Temp              ; load second value address
GoOn:               LDAA  0,X               ; load first digit of decimal value from X address
                    ADDA  Carry             ; add carry
                    LDAB  0,Y               ; load second decimal value from Y address
                    ABA                     ; 
                    CMPA  #10               ; check carry 
                    BGE   SetCarry          ; if carry is exist go to SetCarry
                    MOVB  #$0,Carry         ; set Carry  
                    STY   Temp              ; store address of second decimal value
                    LDY   TempAdd           ; load address of result 
                    STAA  0,Y               ; store result to Y
                    DEY                     ; decrement address of result
                    STY   TempAdd           ; store address of result
                    LDY   Temp              ; load address of second decimal value
GoOnSum:            DEX                     ; decrement address of X which is first decimal value
                    DEY                     ; decrement address of Y which is second decimal value
                    DEC   Counter           ; decrement counter is counting digit number
                    LDAA  Counter           ; load counter in accumulator A
                    CMPA  #Zero             ; check counter
                    BNE   GoOn              ; if counter is not zero go to StartSumOp
                    RTS                     ; if not return subroutine
SetCarry:           
                    SUBA  #10               ; the subtraction operation is for taking an exact number.
                    STY   Temp              ; store the second decimal value address
                    LDY   TempAdd           ; load address of result 
                    STAA  0,Y               ; store result to Y which is result address
                    DEY                     ; decrement result address
                    STY   TempAdd           ; store result address into the tempAdd
                    LDY   Temp              ; load address of second decimal part 
                    MOVB  #$1,Carry         ; set carry 
                    JMP   GoOnSum           ; go on sum operation
                    
ReturnSumDecOp:                      
                    RTS
;End of Subroutin

;SumIntOperation subroutin for integer part addition
SumIntOperation:
                    MOVB  #$0,Counter
                    LDX   #FirstIntPart
                    LDY   #SecondIntPart                     
                    
FirstLoop:          INC   Counter
                    LDAA  Counter
                    CMPA  FirstIntDigitNum
                    BEQ   SecondLoop
                    INX
                    BRA   FirstLoop

SecondLoop:         MOVB  #$0,Counter
SecondLoopGo:       INC   Counter
                    LDAA  Counter
                    CMPA  SecondIntDigitNum
                    BEQ   SetupResultAdd                    
                    INY   
                    BRA   SecondLoopGo

SetupResultAdd:     MOVB  #$0,Counter
                    LDAA  FirstIntDigitNum
                    CMPA  SecondIntDigitNum
                    BGE   SetCounter        ;if first digit num grater than second digit num go to SetCounter
                    MOVB  SecondIntDigitNum, TempCount
                    MOVB  FirstIntDigitNum, LowerDigitNum
                    BRA   ResultLoop
SetCounter:                             
                    MOVB  FirstIntDigitNum,TempCount
                    MOVB  SecondIntDigitNum, LowerDigitNum

ResultLoop:         STY   Temp              ; store second integer part address
                    LDY   #ResultIntPart    ; load result of integer part address
                    MOVB  #$0,Counter
ResultLoopGo:       INC   Counter
                    LDAA  Counter
                    CMPA  TempCount         ; if counter is greater digit num it is done
                    BEQ   StartSumOpInt
                    INY
                    BRA   ResultLoopGo;
                    
StartSumOpInt:      LDAA  FirstIntDigitNum
                    CMPA  TempCount
                    BEQ   LoadSecond
                    STAA  Counter
                    JMP   GoOnSumInit
                    
LoadSecond:         LDAA  SecondIntDigitNum
                    STAA  Counter
                                        
GoOnSumInit:        STY   TempAdd           ; store result address
                    LDY   Temp              ; load second integer part address
GoOnSumInt:         LDAA  0,X               ; get an integer digit of integer value of X address
                    ADDA  Carry             ; add carry
                    LDAB  0,Y               ; get an integer digit of integer value of Y address
                    ABA                     ; addition A to B
                    CMPA  #10               ; check carry
                    BGE   SetCarryInt
                    MOVB  #$0,Carry         ; clear carry
                    STY   Temp              ; store address of second integer value
                    LDY   TempAdd           ; load address of result
                    STAA  0,Y               ; store result to Y address
                    DEY
                    STY   TempAdd           ; store address of result
                    LDY   Temp              ; load address of second value
                    DEX
                    DEY
                    DEC   LowerDigitNum
                    LDAA  LowerDigitNum
                    CMPA  #Zero
                    BNE   GoOnSumInt
                    JSR   FinishedAddition
                    RTS

SetCarryInt:        SUBA  #10
                    STY   Temp              ; store address of second integer value
                    LDY   TempAdd           ; load address of result
                    STAA  0,Y               ; store result to Y which is result of integer part
                    DEY
                    STY   TempAdd           ; store address of result
                    LDY   Temp              ; load address of second integer part
                    MOVB  #$1,Carry
                    JMP   GoOnSumInt
                    
FinishedAddition:
                    LDAA  FirstIntDigitNum
                    CMPA  SecondIntDigitNum
                    BGE   FirstIntAdd
                    BRA   SecondIntAdd


FirstIntAdd:        LDAA  Counter
                    CMPA  #Zero
                    BEQ   ReturnSumIntOp
                    DEC   Counter
                    LDY   TempAdd           ; load result address
                    LDAA  0,X               
                    LDAB  Carry
                    CMPB  #$1
                    BNE   GoOnFirstAdd
                    JSR   AddCarry
GoOnFirstAdd:       STAA  0,Y               ; store result                     
                    DEX
                    JMP   FirstIntAdd
SecondIntAdd
                    LDAA  Counter
                    CMPA  #Zero
                    BEQ   ReturnSumIntOp
                    DEC   Counter
                    LDX   TempAdd            ;
                    LDAA  0,Y
                    LDAB  Carry
                    CMPB  #$1
                    BNE   GoOnSecondAdd
                    JSR   AddCarry
GoOnSecondAdd:      STAA  0,X
                    DEX
                    JMP   SecondIntAdd
AddCarry:
                    ADDA  Carry
                    MOVB  #$0,Carry
                    RTS
ReturnSumIntOp:
                    RTS
;End of subroutin


;StoreResult subroutin 
StoreResult:       
                    LDX   #ResultIntPart
                    LDY   #$1500
                    MOVB  #$0,Counter
                    LDAA  FirstIntDigitNum
                    CMPA  SecondIntDigitNum
                    BGE   FirstGESecond
                    MOVB  SecondIntDigitNum,Temp
                    BRA   LoopStR
                    
FirstGESecond:      MOVB  FirstIntDigitNum,Temp
                                        
LoopStR:            LDAA  0,X
                    ADDA  #SubstractedValue             
                    STAA  0,Y
                    INX
                    INY
                    INC   Counter
                    LDAA  Counter
                    CMPA  Temp
                    BEQ   StoreResultDec                    
                    BRA   LoopStR
                    
StoreResultDec:     
                    LDAA  #DotSign
                    STAA  Y
                    INY  
                    LDX   #ResultDecPart
                    MOVB  #$0,Counter
LoopStRDec:         LDAA  0,X
                    ADDA  #SubstractedValue 
                    STAA  0,Y
                    INX
                    INY
                    INC   Counter
                    CMPA  #DecPartNum
                    BEQ   ReturnStoreResult
                    BRA   LoopStRDec                    
 ReturnStoreResult:
                    RTS
;End of subroutin

;**************************************************************
;*                 Interrupt Vectors                          *
;**************************************************************
                    ORG   $FFFE
                    DC.W  Entry           ; Reset Vector
