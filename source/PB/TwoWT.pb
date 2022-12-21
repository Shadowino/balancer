
PORTA.s = "COM3"
PORTB.s = "COM4"

OpenConsole("diff")


OpenSerialPort(0, PORTA, 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
If Not IsSerialPort(0)
  PrintN("ERROR: Serial port " + PORTA + Chr(10) + "not initial")
EndIf
OpenSerialPort(1, PORTB, 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
If Not IsSerialPort(1)
  PrintN("ERROR: Serial port " + PORTB + Chr(10) + "not initial")
EndIf


; IDE Options = PureBasic 5.11 (Windows - x86)
; CursorPosition = 16
; EnableXP