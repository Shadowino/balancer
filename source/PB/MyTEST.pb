COMPORT.s = "COM10"


OpenConsole()
PrintN("Programm Started")

If OpenSerialPort(0, COMPORT, 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
Else
  MessageRequester("Err", "Serial port " + COMPORT + " not open!")  
  End
EndIf

; SerialPortTimeouts(0, 100, 100, 10, 10, 100)

Global Dim WTqwst.a(7)
WTqwst(0) = $50 ;WT901 adress
WTqwst(1) = $03 ;WT901 command (0x03 -> read)
WTqwst(2) = $00 ;WT901 high adress register
WTqwst(3) = $3D ;WT901 high adress register
WTqwst(4) = $00 ;00
WTqwst(5) = $03 ;count of register read
WTqwst(6) = $99 ;CRC code high
WTqwst(7) = $86 ;CRC code low

Global Dim ARmsg.a(7)
ARmsg(0) = $55 ;Arduino ID
ARmsg(1) = $06 ;Arduino command
ARmsg(2) = $00 ;High Arduino angle X
ARmsg(3) = $00 ;Low Arduino angle x
ARmsg(4) = $00 ;High Arduino angle y
ARmsg(5) = $00 ;Low Arduino angle Y
ARmsg(6) = $00 ;High Arduino angle Z
ARmsg(7) = $00 ;Low Arduino angle Z

Global SendArd = 0;
Global ax.u = 0
Global ay.u = 0
Global az.u = 0
Global Dim answer.a(20)



; While AvailableSerialPortInput(0) = 0
;   WriteSerialPortData(0, @WTqwst(), 8)
;   Delay(200)  
; Wend

Procedure SerialConnect(x.i)
  While 1
    If SendArd = 1
      ConsoleColor(10, 0)
      Print("Arduino MSG Sending...")
      Print("\tsend..." + Str(ARmsg(2)) +" | "+ Str(ARmsg(3)))
      WriteSerialPortData(0, @ARmsg(), 7)
      PrintN("\tOK")
      ConsoleColor(7,0)
      SendArd = 0
    EndIf
    
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
;     Debug cnt
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
EndProcedure


OpenWindow(0, 0, 0, 530, 180, "Control Panel", #PB_Window_ScreenCentered | #PB_Window_SystemMenu)
SerialTH = CreateThread(@SerialConnect(),1)
TextGadget(0,10,10, 250, 20, "Angle X", #PB_Text_Border)
TextGadget(1,10,40, 250, 20, "Angle Y", #PB_Text_Border)
TextGadget(2,10,70, 250, 20, "Angle Z (optional)", #PB_Text_Border)
StringGadget(3,270,10, 250, 20, "0", #PB_String_Numeric)
StringGadget(4,270,40, 250, 20, "0", #PB_String_Numeric)
StringGadget(5,270,70, 250, 20, "0", #PB_String_Numeric)
ButtonGadget(6,10,100,510,40,"Send Parameters")
ButtonGadget(7,10,150,250,20,"Start")
ButtonGadget(8,270,150,250,20,"Stop")

Repeat 
  eve = WaitWindowEvent()
  If eve = #PB_Event_Gadget
    Select EventGadget()
      Case 6
        ARmsg(1) = $06
        ARmsg(2) = $00FF & (Val(GetGadgetText(3)) * 182)
        ARmsg(3) = ($FF00 & (Val(GetGadgetText(3)) * 182)) >> 8
        ARmsg(4) = $00FF & (Val(GetGadgetText(4)) * 182)
        ARmsg(5) = ($FF00 & (Val(GetGadgetText(4)) * 182)) >> 8
        ARmsg(6) = $00FF & (Val(GetGadgetText(5)) * 182)
        ARmsg(7) = ($FF00 & (Val(GetGadgetText(5)) * 182)) >> 8
        SendArd = 1
        ConsoleColor(10, 0)
        PrintN("Arduino MSG Ready")
        ConsoleColor(7,0)
      Case 7
        SendArd = 1
        ARmsg(1) = $01
      Case 8
        SendArd = 1
        ARmsg(1) = $02
    EndSelect
  EndIf
  
  
  
Until eve = #PB_Event_CloseWindow


KillThread(SerialTH)

; IDE Options = PureBasic 5.11 (Windows - x86)
; CursorPosition = 50
; FirstLine = 50
; Folding = -
; EnableXP