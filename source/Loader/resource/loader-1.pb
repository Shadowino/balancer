; исходный код для загрузчика прошивок

Procedure OpenWindow_0(x = 0, y = 0, width = 208, height = 168)
  OpenWindow(0, x, y, width, height, "Loader", #PB_Window_SystemMenu | #PB_Window_ScreenCentered | #PB_Window_MinimizeGadget)
  ButtonGadget(1, 8, 8, 192, 32, "Загрузить прошивку")
  TextGadget(#PB_Any, 8, 64, 80, 20, "Порт")
  ComboBoxGadget(2, 96, 64, 104, 20)
  AddGadgetItem(2, -1, "COM1")
  AddGadgetItem(2, -1, "COM2")
  AddGadgetItem(2, -1, "COM3")
  AddGadgetItem(2, -1, "COM4")
  TextGadget(#PB_Any, 8, 88, 80, 20, "Тип модуля")
  ComboBoxGadget(3, 96, 88, 104, 20)
  AddGadgetItem(3, -1, "Duemilanove")
  AddGadgetItem(3, 0, "Uno")
  SetGadgetState(3, 0)
  TextGadget(#PB_Any, 8, 112, 80, 20, "HEX фаил")
  ButtonGadget(4, 96, 112, 104, 20, "выбрать")
  StringGadget(11, 8, 136, 192, 20, "firmware/example.hex")
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
      EndSelect
  EndSelect
EndProcedure

OpenWindow_0()

Repeat
  eve = WaitWindowEvent()  
  Window_0_Events(eve)
  
Until eve = #PB_Event_CloseWindow


; IDE Options = PureBasic 5.73 LTS (Windows - x64)
; CursorPosition = 15
; FirstLine = 6
; Folding = -
; EnableXP
; UseIcon = icon-nobackground.ico
; Executable = loader.exe