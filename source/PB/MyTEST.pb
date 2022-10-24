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

Global TODEG = 182.041
Global SendArd = 0;
Global ax.u = 0
Global ay.w = 0
Global az.u = 0
Global Dim answer.a(20)


Dim graph.f(700)
Procedure draw(x.i)
  While 1
    StartDrawing(CanvasOutput(9))
    Box(0,0, 670,180, RGB(10,50,10))
    
    
    StopDrawing()
  Wend  
EndProcedure

OpenSerialPort(1, "COM19", 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
WriteSerialPortString(1, "AY DIF"+Chr(10))

Procedure SerialConnect(x.i) ;procedure thread
  While 1
    Delay(1)
    If SendArd = 1 ; send arduino message if available
      ConsoleColor(10, 0)
      Print("Arduino MSG Sending...")
      Print("\tsend..." + Str(ARmsg(2)) +" | "+ Str(ARmsg(3)))
      WriteSerialPortData(0, @ARmsg(), 7)
      PrintN("\tOK")
      ConsoleColor(7,0)
      SendArd = 0
    EndIf
    
    While AvailableSerialPortInput(0) = 0 ; send qwestion for WT901
      WriteSerialPortData(0, @WTqwst(), 8)
      Delay(20)
    Wend
    
    ans.s = "" ;answer output console
    cnt = AvailableSerialPortInput(0) - 1 ; count available bytes
    For i = 0 To cnt ; read available data and add this in console output
      ReadSerialPortData(0, @bite, 1)
      answer(i) = bite
;       ans = ans + " 0x" + RSet(Hex(bite), 2, "0") ; console output bytes
    Next
    ans = ans + " => "  ;
    prevAy = ay;
    ax = ((answer(3) << 8) | answer(4)) ; parsing WT901 answer
    ay = ((answer(5) << 8) | answer(6))
    az = ((answer(7) << 8) | answer(8))
    WriteSerialPortString(1, StrF(ay/182.0416,3) + " " + StrF((prevAy-ay)/182.0416,3) +Chr(10))
    ; add angle values in console output
;     ans = ans + RSet(StrF(ax / TODEG, 2), 6, "0") + " | " + RSet(StrF(ay  / TODEG , 1), 6, "0") + " | " + RSet(StrF(az  / TODEG, 2), 6, "0")
    ans = ans + RSet(StrF(ay  / TODEG , 1), 6, " ")
    PrintN(ans) ;output console
    
  Wend
EndProcedure

;window init
OpenWindow(0, 0, 0, 230, 180, "Control Panel", #PB_Window_ScreenCentered | #PB_Window_SystemMenu)
TextGadget(0,10,10, 100, 20, "Angle X", #PB_Text_Border)
TextGadget(1,10,40, 100, 20, "Angle Y", #PB_Text_Border)
TextGadget(2,10,70, 100, 20, "Angle Z (optional)", #PB_Text_Border)
StringGadget(3,120,10, 100, 20, "0", #PB_String_Numeric)
StringGadget(4,120,40, 100, 20, "0", #PB_String_Numeric)
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
        ARmsg(2) = $00FF & (Val(GetGadgetText(3)) * 182)
        ARmsg(3) = ($FF00 & (Val(GetGadgetText(3)) * 182)) >> 8
        ARmsg(4) = $00FF & (Val(GetGadgetText(4)) * 182)
        ARmsg(5) = ($FF00 & (Val(GetGadgetText(4)) * 182)) >> 8
        ARmsg(6) = $00FF & (Val(GetGadgetText(5)) * 182)
        ARmsg(7) = ($FF00 & (Val(GetGadgetText(5)) * 182)) >> 8
        SendArd = 1 ; flag available message
        ConsoleColor(10, 0) 
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


; IDE Options = PureBasic 5.11 (Windows - x86)
; CursorPosition = 86
; FirstLine = 69
; Folding = -
; EnableXP