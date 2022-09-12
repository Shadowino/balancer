
;03.09.21
;чтение показаний углов с датчика WT901C-485

Global yaw2.f
Global pitch3.f
Global roll3.f
ans.i = 0
Global threadCOMopen
Global threadCOMopen1
Global daterr.b
Global comport
Global angle_axis = 0

Dim Buff.a(7)
Buff(0) = $50
Buff(1) = $03
Buff(2) = $00
Buff(3) = $3D
Buff(4) = $00
Buff(5) = $03
Buff(6) = $99
Buff(7) = $86

flags = #PB_Window_ScreenCentered | #PB_Window_SystemMenu | #PB_Window_MinimizeGadget
OpenWindow(1, 0, 0, 400, 300, "WT901C485", flags)
AddWindowTimer(1, 1, 200)
StringGadget(1, 50, 20, 100, 50, "")
StringGadget(2, 50, 120, 100, 50, "")
StringGadget(3, 50, 220, 100, 50, "")
TextGadget(4, 170, 20, 200, 50, "азимут")
TextGadget(5, 170, 120, 200, 50, "элевация")
TextGadget(6, 170, 220, 200, 50, "поляризация")
LoadFont(1, tahoma$, 20)
SetGadgetFont(1, FontID(1))
SetGadgetFont(2, FontID(1))
SetGadgetFont(3, FontID(1))
SetGadgetFont(4, FontID(1))
SetGadgetFont(5, FontID(1))
SetGadgetFont(6, FontID(1))

Procedure COMopen(com); тред мониторит если втыкнули конвертер обратно
  While 1
    Delay(5)
    If daterr = 1
      If OpenSerialPort(1, "COM" + Str(comport), 9600, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
        daterr = 0
        KillThread(threadcomopen)
      EndIf
    EndIf
  Wend
EndProcedure

Procedure COMopen1(com1); тред мониторит если втыкнули конвертер 
  While 1
    Delay(5)
    If daterr = 1
      For k = 1 To 255
        Delay(3)
        If OpenSerialPort(1, "COM" + k, 9600, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
          comport = k; запомнила номер открытого порта
          daterr = 0
          KillThread(threadCOMopen1)
          Break  
        EndIf
      Next
    EndIf
  Wend
EndProcedure

Procedure anglesget(get); данные с датчика (углы)
  While 1 
    Delay(1)
    If daterr = 0 
      If IsSerialPort(1)
        Delay(10)
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
        While AvailableSerialPortInput(1) = 0
          WriteSerialPortData(1, @Buff(), 8)
          Delay(200)  
        Wend
        If ReadSerialPortData(1, @bite, 1); связь с датчиком есть
          ans = Val("$" + RSet(Hex(bite), 2, "0"))
          If ans = $50
            answer(0) = ans
            For i = 1 To 10
              ReadSerialPortData(1, @bite, 1)
              ans = Val("$" + RSet(Hex(bite), 2, "0"))
              answer(i) = ans
              rollh.i = answer(3)
              rolll.i = answer(4)
              roll1 = (rollh << 8) | rolll
              roll2.f = roll1 / 32768 * 180
              If roll2 > 180
                roll3.f = (360 - roll2)
              Else
                roll3.f = -roll2
              EndIf
              pitchh = answer(5)
              pitchl = answer(6)
              pitch1 = (pitchh << 8) | pitchl
              pitch2.f = pitch1 / 32768 * 180 + angle_axis
              If pitch2 > 180
                pitch3.f = -(360 - pitch2)
              Else
                pitch3.f = pitch2
              EndIf
              yawh.i = answer(7)
              yawl.i = answer(8)
              yaw1 = (yawh << 8) | yawl
              yaw2.f = (yaw1 / 32768 * 180) + azimuthcorr
              If yaw2 > 360
                yaw3 = yaw2 - 360
              Else
                yaw3 = yaw2
              EndIf
              If i = 10
              SetGadgetText(1, StrF(yaw2, 1))
              SetGadgetText(2, StrF(pitch3, 1))
              SetGadgetText(3, (StrF(roll3, 1)))
              EndIf      
            Next
          EndIf
        Else; связь с датчиком пропала
          MessageRequester("Ошибка", "Проверьте подключение датчика")
          daterr = 1
          threadCOMopen = CreateThread(@COMopen(), com)
        EndIf
      EndIf
    EndIf
  Wend
EndProcedure 

For k = 4 To 255
  If daterr = 0
    If OpenSerialPort(1, "COM" + k, 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
      comport = k
      Break  
    Else 
      x = x + 1
      If x = 255
      daterr = 1
      threadCOMopen1 = CreateThread(@COMopen1(), com1)
      MessageRequester("Ошибка", "Проверьте подключение датчика", #PB_MessageRequester_Ok)
      Break
      EndIf
    EndIf
  EndIf
Next
Debug comport
CreateThread(@anglesget(), get)

Repeat
  event = WaitWindowEvent()
  
Until event = #PB_Event_CloseWindow
End

; IDE Options = PureBasic 5.11 (Windows - x86)
; CursorPosition = 48
; Folding = 5
; EnableThread
; EnableXP
; IDE Options = PureBasic 5.11 (Windows - x64)
; CursorPosition = 144
; FirstLine = 21
; Folding = 5
; EnableThread
; EnableXP