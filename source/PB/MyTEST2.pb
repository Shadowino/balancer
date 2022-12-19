Dim Buff.a(7)
Buff(0) = $50
Buff(1) = $03
Buff(2) = $00
Buff(3) = $3D
Buff(4) = $00
Buff(5) = $03
Buff(6) = $99
Buff(7) = $86


Global COMPORT.s = "COM"
STATUS = 0
;STATUS:
; 0 -> wait
; 1 -> COM CONECTED

Declare UclosePort()
Declare UopenPort()
Declare UcheckPort()

Procedure UopenPort()
  
  UclosePort()
  
  If OpenSerialPort(0, COMPORT, 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
    PrintN("OK")
    SetGadgetText(4,"STOP")
  Else
    PrintN("ERROR!")
  EndIf
  
EndProcedure

Procedure UclosePort()
  If IsSerialPort(0)  
    PrintN(COMPORT + " -> Closed")  
    CloseSerialPort(0)
  Else
    PrintN(COMPORT + " -> Already Closed")  
  EndIf
  
EndProcedure

Procedure UcheckPort()
  ConsoleColor(7, 0)
  PrintN("Start COM scans")
  ClearGadgetItems(3)
  PrintN("COM list clear")
  For a = 1 To 64
    If OpenSerialPort(0, "COM" + Str(a), 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
      ConsoleColor(10, 0)
      PrintN("COM" + Str(a) + " -> Enable")
      AddGadgetItem(3, -1,"COM" + Str(a)) 
      CloseSerialPort(0)
    EndIf
  Next  
  ConsoleColor(7, 0)
EndProcedure



Dim nm1.s(3)
nm1(0) = "green "
nm1(1) = "yellow "
nm1(2) = "red "
nm1(3) = "blue "
Dim nm2.s(3)
nm2(0) = "window "
nm2(1) = "system "
nm2(2) = "programm "
nm2(3) = "robot "


WINPAR = #PB_Window_ScreenCentered | #PB_Window_SystemMenu
OpenConsole("CONSOLE")
OpenWindow(0, 0,0,800,600,nm1(Random(3))+nm2(Random(3)), WINPAR)
PrintN()
TextGadget(0, 10, 10, 250, 20, "ANGLE XºYºZº", #PB_Text_Border)
TextGadget(1, 10, 40, 250, 20, "TEXT", #PB_Text_Border)
TextGadget(2, 10, 70, 250, 20, "TEXT", #PB_Text_Border)
ComboBoxGadget(3, 540, 10, 250, 20)
ButtonGadget(4, 540, 40, 250, 20, "START", #PB_Text_Border)
ButtonGadget(5, 540, 70, 250, 20, "TEST COM", #PB_Text_Border)
ButtonGadget(6, 540, 100, 250, 20, "Find Serial", #PB_Text_Border)

; OpenSerialPort

For a = 1 To 64
  If OpenSerialPort(0, "COM" + Str(a), 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
    AddGadgetItem(3, -1,"COM" + Str(a)) 
    CloseSerialPort(0)
  EndIf
Next

Repeat
  eve = WaitWindowEvent()
  
  If eve = #PB_Event_Gadget
    Select EventGadget()
      Case 0
        
      Case 1
        
      Case 3
        UclosePort()
        COMPORT = GetGadgetText(3)
        PrintN("Selected " + COMPORT)
      Case 4
        If IsSerialPort(0)
          ConsoleColor(7, 0)
          Print(COMPORT + " ->" )
          ConsoleColor(12, 0)
          PrintN("STOP")
          ConsoleColor(7, 0)
          CloseSerialPort(0)
          SetGadgetText(4,"START")
        Else
          Print(COMPORT + " -> starting... ")
          If OpenSerialPort(0, COMPORT, 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
            ConsoleColor(10, 0)
            PrintN("OK")
            ConsoleColor(7, 0)
            SetGadgetText(4,"STOP")
          Else
            ConsoleColor(12, 0)
            PrintN("ERROR")
            ConsoleColor(7, 0)
          EndIf
        EndIf
      Case 5
        If IsSerialPort(0)
          PrintN(COMPORT + " -> Open")
        Else
          PrintN(COMPORT + " -> Close")
        EndIf
      Case 6
        UcheckPort()
    EndSelect
    
    
    
  EndIf
  
  
  
  
Until eve = #PB_Event_CloseWindow


; IDE Options = PureBasic 5.11 (Windows - x64)
; CursorPosition = 99
; FirstLine = 70
; Folding = -
; EnableXP
; DPIAware