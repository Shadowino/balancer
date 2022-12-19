; исходный код для загрузчика прошивок

LoadFont(0,"Consolas", 14)
LoadFont(1,"Consolas", 18)

Procedure ScanPort(GID = 2)
  If IsSerialPort(0)
    CloseSerialPort(0)  
  EndIf
  ClearGadgetItems(GID)
  For i = 0 To 64  
    If OpenSerialPort(0, "COM" + Str(i), 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
      AddGadgetItem(GID, -1, "COM" + Str(i))
      CloseSerialPort(0)
    EndIf
  Next
EndProcedure

  

Procedure OpenWindow_0(x = 0, y = 0, width = 440, height = 184)
  OpenWindow(0, x, y, width, height, "Загрузчик программ © SelSoft 2022", #PB_Window_SystemMenu | #PB_Window_ScreenCentered | #PB_Window_MinimizeGadget)
  TextGadget(100, 8, 8, 400, 32, "Подключите ARDUINO", #PB_Text_Border)
  TextGadget(101, 8, 48, 80, 24, "Порт")
  TextGadget(102, 8, 80, 80, 24, "HEX фаил")
  TextGadget(103, 8, 112, 80, 24, "Тип модуля")
  ComboBoxGadget(2, 96, 48, 200, 24)
  ComboBoxGadget(3, 96, 80, 200, 24)
  StringGadget(11, 96, 112, 200, 24, "firmware/example.hex", #PB_String_ReadOnly)
  ButtonGadget(5, 304, 48, 104, 24, "Поиск")
  ButtonGadget(4, 304, 112, 104, 24, "выбрать")
  ButtonGadget(1, 8, 144, 200, 32, "Загрузить программу")
  
;   AddGadgetItem(2, -1, "COM1")
;   AddGadgetItem(2, -1, "COM2")
;   AddGadgetItem(2, -1, "COM3")
;   AddGadgetItem(2, -1, "COM4")
  AddGadgetItem(3, -1, "Duemilanove")
  AddGadgetItem(3, 0, "Uno")
  SetGadgetState(3, 0)
  SetGadgetFont(100, FontID(0))
  SetGadgetFont(101, FontID(1))
  SetGadgetFont(102, FontID(1))
  SetGadgetFont(103, FontID(1))
  ScanPort(2)
EndProcedure

Procedure Window_0_Events(event)
  Select event
    Case #PB_Event_Menu
      Select EventMenu()
      EndSelect

    Case #PB_Event_Gadget
      Select EventGadget()
        Case 1
          MessageRequester("INFO", "LOAD")
          RunProgram("calc")
        Case 2
          MessageRequester("INFO", "PORT SELECT")
        Case 3
          MessageRequester("INFO", "warning")
        Case 4
          SetGadgetText(11,OpenFileRequester("Выбрать HEX", GetCurrentDirectory(), "HEX прошивка|*.hex|Все файлы|*.*", 0))
        Case 5
          ScanPort(2)
      EndSelect
  EndSelect
EndProcedure

OpenWindow_0()

Repeat
  eve = WaitWindowEvent()  
  Window_0_Events(eve)
  
Until eve = #PB_Event_CloseWindow


; IDE Options = PureBasic 5.11 (Windows - x64)
; CursorPosition = 64
; Folding = -
; EnableUnicode
; EnableXP
; UseIcon = icon-nobackground.ico
; Executable = loader.exe