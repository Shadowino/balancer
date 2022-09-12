

OpenConsole()
OpenSerialPort(0, "COM5", 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)

Dim Buff.a(7)
Buff(0) = $50
Buff(1) = $03
Buff(2) = $00
Buff(3) = $3D
Buff(4) = $00
Buff(5) = $03
Buff(6) = $99
Buff(7) = $86
Dim answer.a(10)
While AvailableSerialPortInput(0) = 0
  WriteSerialPortData(0, @Buff(), 8)
  Delay(200)  
Wend

While 1
  While AvailableSerialPortInput(0) = 0
    WriteSerialPortData(0, @Buff(), 8)
    Delay(200)  
  Wend
  If ReadSerialPortData(0, @bite, 1) ; связь с датчиком есть
    PrintN("$" + RSet(Hex(bite), 2, "0")) 
  EndIf
Wend

; IDE Options = PureBasic 5.11 (Windows - x64)
; CursorPosition = 6
; EnableXP