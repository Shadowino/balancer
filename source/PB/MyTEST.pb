COMPORT.s = "COM20"


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

Global Dim ARmsg.a(10)
ARmsg(0) = $55 ;Arduino ID
ARmsg(1) = $06 ;Arduino command
ARmsg(2) = $04 ;Lenght message
ARmsg(3) = $00 ;High Arduino angle X
ARmsg(4) = $00 ;Low Arduino angle x
ARmsg(5) = $00 ;High Arduino angle y
ARmsg(6) = $00 ;Low Arduino angle Y
ARmsg(7) = $00 ;High Arduino angle Z
ARmsg(8) = $00 ;Low Arduino angle Z

Global TODEG = 182.041
Global SendArd = 0;
Global ax.u = 0
Global ay.w = 0
Global az.u = 0
Global Dim answer.a(20)



; OpenSerialPort(1, "COM19", 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
; WriteSerialPortString(1, "AY DIF"+Chr(10))

Procedure SerialConnect(x.i) ;procedure thread
  PrintN("SerialConnect")
  While 1
    Delay(1)
    If SendArd = 1 ; send arduino message if available
      ConsoleColor(10, 0)
      Print("Arduino MSG Sending...")
      Print("\tsend...")
      For i = 0 To 8
        Print(" | " + Hex(ARmsg(i)))
      Next
      WriteSerialPortData(0, @ARmsg(), 8)
      PrintN("\tOK")
      ConsoleColor(7,0)
      SendArd = 0
      Delay(100)  
    Else
      
      
      While AvailableSerialPortInput(0) = 0 ; send qwestion for WT901
        WriteSerialPortData(0, @WTqwst(), 8)
;         Print("send:  ") ;output console
;         Debug "send"
        Delay(100)
      Wend
      
      ans.s = "" ;answer output console
      cnt = AvailableSerialPortInput(0) - 1 ; count available bytes
;       Debug cnt
      ;     ReadSerialPortData(0, answer, 10)
      For i = 0 To cnt ; read available data and add this in console output
        ReadSerialPortData(0, @bite, 1)
        answer(i) = bite
          ans = ans + " 0x" + RSet(Hex(bite), 2, "0") ; console output bytes
      Next
;       Debug "recv"4
      ans = ans + " => "  ;
      prevAy = ay;
      ax = ((answer(3) << 8) | answer(4)) ; parsing WT901 answer
      ay = ((answer(5) << 8) | answer(6))
      az = ((answer(7) << 8) | answer(8))
      ;     WriteSerialPortString(1, StrF(ay/182.0416,3) + " " + StrF((prevAy-ay)/182.0416,3) +Chr(10))
      ; add angle values in console output
      ;     ans = ans + RSet(StrF(ax / TODEG, 2), 6, "0") + " | " + RSet(StrF(ay  / TODEG , 1), 6, "0") + " | " + RSet(StrF(az  / TODEG, 2), 6, "0")
      ans = ans + RSet(StrF(ax  / TODEG , 1), 6, " ") + " " + RSet(StrF(ay  / TODEG , 1), 6, " ") + " " + RSet(StrF(az  / TODEG , 1), 6, " ")
      PrintN(ans) ;output console
    EndIf
    
  Wend
EndProcedure

;window init
OpenWindow(0, 0, 0, 230, 180, "Control Panel", #PB_Window_ScreenCentered | #PB_Window_SystemMenu)
TextGadget(0,10,10, 100, 20, "Angle X", #PB_Text_Border)
TextGadget(1,10,40, 100, 20, "Angle Y", #PB_Text_Border)
TextGadget(2,10,70, 100, 20, "Angle Z (optional)", #PB_Text_Border)
StringGadget(3,120,10, 100, 20, "0")
StringGadget(4,120,40, 100, 20, "0")
StringGadget(5,120,70, 100, 20, "0", #PB_String_Numeric)
ButtonGadget(6,10,100,210,40,"Send Parameters")
ButtonGadget(7,10,150,100,20,"Start")
ButtonGadget(8,120,150,100,20,"Stop")
; CanvasGadget(9,230,0,670,180)
SerialTH = CreateThread(@SerialConnect(),1) ; start thread Serial event parser
; drawTH = CreateThread(@draw(),1) ; start thread Serial event parser


Repeat 
  eve = WaitWindowEvent() ; wait window event optimizate
  If eve = #PB_Event_Gadget
    Select EventGadget()
      Case 6 ; "Send Parameters" button, create and send arduno message
        ARmsg(1) = $06 ; create arduino message, | see message struct on line 25 |
        ARmsg(3) = $00FF & Int(ValF(GetGadgetText(3)) * 182)
        ARmsg(4) = ($FF00 & Int(ValF(GetGadgetText(3)) * 182)) >> 8
        ARmsg(5) = $00FF & Int(ValF(GetGadgetText(4)) * 182)
        ARmsg(6) = ($FF00 & Int(ValF(GetGadgetText(4)) * 182)) >> 8
        ;         ARmsg(4) = $00FF & Int(20.9 * 182)
        ;         ARmsg(5) = ($FF00 & Int(20.9 * 182)) >> 8
        ARmsg(7) = $00FF & (Val(GetGadgetText(5)) * 182)
        ARmsg(8) = ($FF00 & (Val(GetGadgetText(5)) * 182)) >> 8
        SendArd = 1 ; flag available message
        ConsoleColor(10, 0) 
        Debug ARmsg(3)
        Debug ARmsg(4)
        Debug ARmsg(5)
        Debug ARmsg(6)
        PrintN("Arduino MSG Ready") ; console Output debug
        ConsoleColor(7,0)
      Case 7 ; startmessage
        ARmsg(1) = $01 ; start command
        SendArd = 1
      Case 8 ; stop message
        ARmsg(1) = $02 ; stop command
        SendArd = 1
    EndSelect
  EndIf
  
  
  
Until eve = #PB_Event_CloseWindow


KillThread(SerialTH) ; kill Serial event thread
; KillThread(drawTH) ; kill Serial event thread


<<<<<<< HEAD

; IDE Options = PureBasic 5.73 LTS (Windows - x64)
||||||| d29c8cc
<<<<<<< HEAD
; IDE Options = PureBasic 5.73 LTS (Windows - x64)
; CursorPosition = 142
; FirstLine = 109
||||||| 3d92543
; IDE Options = PureBasic 5.11 (Windows - x86)
; CursorPosition = 79
; FirstLine = 76
=======
; IDE Options = PureBasic 5.11 (Windows - x86)
<<<<<<< HEAD
; CursorPosition = 77
; FirstLine = 79
>>>>>>> eaa59e702f5c21a35c014259da30ed8a1d24b446
||||||| eaa59e7
; CursorPosition = 77
; FirstLine = 79
=======
; CursorPosition = 91
; FirstLine = 72
>>>>>>> b6a0edddb0d7688f2fc9c1e5a57b26013f7e9272
=======

; IDE Options = PureBasic 5.11 (Windows - x86)
; CursorPosition = 82
; FirstLine = 58
>>>>>>> 7955a0dc359bb158ead292050611b47767582f04
; Folding = -
; EnableXP