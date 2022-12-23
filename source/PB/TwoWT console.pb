; 
PORTA.s = "COM22"
PORTB.s = "COM21"
PORTC.s = "COM19"

Global Dim SPA.a(20)
Global Dim SPB.a(20)

Global Dim WTqwst.a(7)
WTqwst(0) = $50 ;WT901 adress
WTqwst(1) = $03 ;WT901 command (0x03 -> read)
WTqwst(2) = $00 ;WT901 high adress register
WTqwst(3) = $3D ;WT901 high adress register
WTqwst(4) = $00 ;00
WTqwst(5) = $03 ;count of register read
WTqwst(6) = $99 ;CRC code high
WTqwst(7) = $86 ;CRC code low

SPAx.w 
SPAy.w 
SPAz.w
SPBx.w 
SPBy.w 
SPBz.w 

Global TODEG = 182.041
Procedure.s cr(angle)
  ProcedureReturn RSet(StrF(angle / TODEG , 1), 6, " ")  
EndProcedure

Pattern$ = "Text (*.txt)|*.txt;|All files (*.*)|*.*"
wrlog.s = SaveFileRequester("genm", "csv.txt",Pattern$,2)

OpenConsole()
PrintN("Start")

OpenSerialPort(0, PORTA, 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
If IsSerialPort(0) : PrintN(PORTA + " Open") : EndIf
OpenSerialPort(1, PORTB, 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
If IsSerialPort(1) : PrintN(PORTB + " Open") : EndIf
OpenSerialPort(2, PORTC, 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
If IsSerialPort(2) : PrintN(PORTC + " Open") : EndIf
CreateFile(0, wrlog)
If IsFile(0) : PrintN(wrlog + " create") : EndIf

Input()
If wrlog = ""
  MessageRequester("ER", "no path")  
  CloseConsole()
  End
EndIf


PrintN("File:" + wrlog)


PrintN("Work Start")

While 1
  While AvailableSerialPortInput(0) = 0
    WriteSerialPortData(0, @WTqwst(), 8)
    Delay(5)
  Wend
  While AvailableSerialPortInput(1) = 0
    WriteSerialPortData(1, @WTqwst(), 8)
    Delay(5)
  Wend
  ans.s = ""
  
  cnt = AvailableSerialPortInput(0) - 1 ; count available bytes
  For i = 0 To cnt ; read available data and add this in console output
    ReadSerialPortData(0, @bite, 1)
    If i < 20 : SPA(i) = bite : EndIf
  Next  
  cnt = AvailableSerialPortInput(1) - 1 ; count available bytes
  For i = 0 To cnt ; read available data and add this in console output
    ReadSerialPortData(1, @bite, 1)
    If i < 20 : SPB(i) = bite : EndIf
  Next
  
  SPAx = ((SPA(3) << 8) | SPA(4)) ; parsing WT901 SPA
  SPAy = ((SPA(5) << 8) | SPA(6))
  SPAz = ((SPA(7) << 8) | SPA(8))
  SPBx = ((SPB(3) << 8) | SPB(4)) ; parsing WT901 SPB
  SPBy = ((SPB(5) << 8) | SPB(6))
  SPBz = ((SPB(7) << 8) | SPB(8))
  
;   ans = cr(SPAx) + " ;" + cr(SPAy) + ";" + cr(SPBx) + ";" + cr(SPBy)
  ans = cr(SPAy) + ";" + cr(-SPBy)
;   ans = 
  PrintN(ans)
  If IsFile(0)
    WriteStringN(0, ReplaceString(ans, ".", ","))
  Else
    Break
  EndIf
  If IsSerialPort(2)
    WriteSerialPortString(2, ReplaceString(ans, ";", " ") + Chr(10))
  EndIf
  
Wend  

If IsFile(0)
  CloseFile(0)
EndIf

If IsSerialPort(0)
  CloseSerialPort(0)  
EndIf
If IsSerialPort(1)
  CloseSerialPort(2)  
EndIf
If IsSerialPort(2)
  CloseSerialPort(2)  
EndIf
CloseConsole()
End

; IDE Options = PureBasic 5.11 (Windows - x86)
; CursorPosition = 65
; FirstLine = 58
; Folding = -
; EnableXP