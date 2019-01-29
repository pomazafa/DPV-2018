.586
.model FLAT, C , stdcall
includelib libucrt.lib 
includelib StaticLib.lib
includelib kernel32.lib 
ExitProcess PROTO : DWORD 
.stack 4096
outputStr PROTO : DWORD
outputIs PROTO : DWORD
outputInt PROTO : DWORD
issubstr PROTO : DWORD, : DWORD
getlength PROTO : DWORD
getnod PROTO : DWORD, : DWORD
getnok PROTO : DWORD, : DWORD
.CONST
L1 DWORD 3
L2 DWORD 5
L3 DWORD 2
L4 DWORD 40
L5 DB " (sq(3, 5) + 2 - 4) / 2 ", 0
L6 DB "a", 0
L7 DB "gav", 0
L8 DB "нок и нод 10 и 12, длина этой строки", 0
L9 DWORD 10
L10 DWORD 12
L11 DWORD 0
.data
sqz DWORD 0
mainn1 DWORD 0
mainn2 DWORD 0
mainn3 DWORD 0
mainstr1 DB 255 dup(0)
mainstr2 DB 255 dup(0)
mainc DWORD 0
.CODE
sq PROC sqx : DWORD, sqy : DWORD
PUSH sqx
PUSH sqy
POP EBX
POP EAX
MOV EDX, 0
MUL EBX
MOV EBX, EAX
PUSH EBX
POP sqz
MOV EAX, sqz
ret
sq ENDP
main PROC
PUSH L2
PUSH L1
CALL sq
PUSH EAX
PUSH L3
POP EDX
POP EBX
ADD EBX, EDX
PUSH EBX
PUSH L4
POP EDX
POP EBX
SUB EBX, EDX
PUSH EBX
PUSH L3
POP EBX
POP EAX
MOV EDX, 0
DIV EBX
MOV EBX, EAX
PUSH EBX
POP mainn3
PUSH L2
PUSH L1
CALL sq
PUSH EAX
POP mainn1
PUSH OFFSET L5
CALL outputStr
PUSH mainn3
CALL outputInt 
MOV ESI, OFFSET L6
MOV EDI, OFFSET mainstr1
MOV ECX, 2
REP MOVSB
MOV ESI, OFFSET L7
MOV EDI, OFFSET mainstr2
MOV ECX, 4
REP MOVSB
PUSH OFFSET mainstr2
PUSH OFFSET mainstr1
CALL issubstr
PUSH EAX
POP mainc
PUSH mainc
CALL outputIs
MOV ESI, OFFSET L8
MOV EDI, OFFSET mainstr1
MOV ECX, 37
REP MOVSB
PUSH OFFSET mainstr1
CALL outputStr
PUSH L9
POP mainn1
PUSH L10
POP mainn2
PUSH mainn2
PUSH mainn1
CALL getnod
PUSH EAX
POP mainn3
PUSH mainn3
CALL outputInt 
PUSH mainn2
PUSH mainn1
CALL getnok
PUSH EAX
POP mainn3
PUSH mainn3
CALL outputInt 
PUSH OFFSET mainstr1
CALL getlength
PUSH EAX
POP mainn1
PUSH mainn1
CALL outputInt 
MOV EAX, L11
ret
MAIN ENDP
end main
