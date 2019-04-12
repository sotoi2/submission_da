;
; DA1A.asm
; Author : Ivan
;


; Replace with your application code

.ORG 00
LDI R26, 0 // I will use this to adc the third result register

LDI R25, 0XAB // My example number (High) Decimal (43981) - Together with R24
LDI R24, 0XCD // My example number (Low) 

LDI R22, 0X0A // My example multiplier (10)



LDI R20, 0 // Leave the result 
LDI R19, 0 // registers equal to 0
LDI R18, 0 // for results preparation


// RESULT SHOULD BE 6B602


// This will iterate a total of 10 times
loop:

ADD R18, R24 // The leftmost only needs an ADD because there is now way the carry flag is set initially 
ADC R19, R25 // ADC is needed to receive potential carries from the leftmost register
ADC R20, R26 // Similar to the previous instruction, necessary for any carries

DEC R22     // Decrement our "counter" (multiplier)
BRNE loop   // Branch if R22 is not equal to Z flag being set
jmp l2      // Begin an infinite loop
l2:
