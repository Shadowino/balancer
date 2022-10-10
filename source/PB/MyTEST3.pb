COMPORT.s = "COM6"


OpenConsole()
If OpenSerialPort(0, COMPORT, 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
Else
  MessageRequester("Err", "Serial port " + COMPORT + " not open!")  
  End
EndIf

  ;SerialPortTimeouts(0, 100, 100, 10, 10, 100)

Dim WTqwst.a(7)
WTqwst(0) = $50 ;WT901 adress
WTqwst(1) = $03 ;WT901 command (0x03 -> read)
WTqwst(2) = $00 ;WT901 high adress register
WTqwst(3) = $3D ;WT901 high adress register
WTqwst(4) = $00 ;00
WTqwst(5) = $03 ;count of register read
WTqwst(6) = $99 ;CRC code high
WTqwst(7) = $86 ;CRC code low

Dim ARmsg.a(7)
ARmsg(0) = $55 ;Arduino ID
ARmsg(1) = $06 ;Arduino command
ARmsg(2) = $00 ;High Arduino angle X
ARmsg(3) = $00 ;Low Arduino angle x
ARmsg(2) = $00 ;High Arduino angle y
ARmsg(3) = $00 ;Low Arduino angle Y
ARmsg(2) = $00 ;High Arduino angle Z
ARmsg(3) = $00 ;Low Arduino angle Z


ax.u = 0
ay.u = 0
az.u = 0
Dim answer.a(20)




While AvailableSerialPortInput(0) = 0
  WriteSerialPortData(0, @WTqwst(), 8)
  Delay(200)  
Wend

While 1
  While AvailableSerialPortInput(0) = 0
    WriteSerialPortData(0, @WTqwst(), 8)
    Delay(20)
  Wend
  
  ;If ReadSerialPortData(0, @bite, 1) ; связь с датчиком есть
  ;  PrintN("$" + RSet(Hex(bite), 2, "0")) 
  ;EndIf
  ans.s = ""
  ;   Debug 
;   cnt = AvailableSerialPortInput(0)
;   Delay(10)
  cnt = AvailableSerialPortInput(0) - 1
  Debug cnt
  For i = 0 To cnt
    ReadSerialPortData(0, @bite, 1)
    answer(i) = bite
    ;Print(" 0x" + RSet(Hex(bite), 2, "0")) 
    ans = ans + " 0x" + RSet(Hex(bite), 2, "0")
  Next
  ans = ans + " => " 
  ;Print(" => ")
  
  ax = ((answer(3) << 8) | answer(4)) 
  ay = ((answer(5) << 8) | answer(6))
  az = ((answer(7) << 8) | answer(8))
  ans = ans + RSet(StrF(ax / 182, 2), 6, "0") + " | " + RSet(StrF(ay  / 182 , 2), 6, "0") + " | " + RSet(StrF(az  / 182, 2), 6, "0")
  ;Print(RSet(StrF(ax / 182, 2), 6, "0") + " | " + RSet(StrF(ay  / 182 , 2), 6, "0") + " | " + RSet(StrF(az  / 182, 2), 6, "0"))
  PrintN(ans) 
  
  ;EndIf
Wend

; IDE Options = PureBasic 5.73 LTS (Windows - x64)
; CursorPosition = 20
; FirstLine = 5
; EnableXP