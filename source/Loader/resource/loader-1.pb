; исходный код для загрузчика прошивок
#disableDef = #False

; для purebasic 5.11!!!!!
#PB_MessageRequester_Error = #PB_MessageRequester_Ok

LoadFont(0,"Consolas", 14)
LoadFont(1,"Consolas", 18)

Global LDRF.s = "avr/avrdude"
Global LDRPU.s = "-F -v -p m328p -c arduino -P %1 -b 115200 -D -U flash:w:" + Chr(34)+ "%2" + Chr(34)+":i"
Global LDRPD.s = "-F -v -p m328p -c arduino -P %1 -b 57600 -D -U flash:w:" + Chr(34)+ "%2" + Chr(34)+":i"


; [Uno]
; ; строка параметров передаваемая AVRDUDE для прошивки МК
; avrdude_param=-F -v -p m328p -c arduino -P %1 -b 115200 -D -U flash:w:"%2":i
; ; %1 - подставится номер выбранного порта
; ; %2 - подставится путь к выбранному хексу
; ; порт
; port=COM1
; ; путь к файлу прошивки
; hex_file_path=drop file here


Procedure loadHex()
;   RunProgram("calc") ; delete this fragment
;   ProcedureReturn ; delete this fragment
  errorF = #False
  If GetGadgetText(2) = ""
    MessageRequester("ERROR", "Укажите правильный порт!", #PB_MessageRequester_Error)
    errorF = #True 
  EndIf
  If Not ReadFile(0, GetGadgetText(11))
    MessageRequester("ERROR", "Указанный HEX фаил не найден!", #PB_MessageRequester_Error)
    errorF = #True 
  EndIf
  
  
  If errorF : ProcedureReturn : EndIf ; прекратить загрузку если найдены ошибки
  
  PP.s = ""
  If GetGadgetText(3) = "Uno"
    PP.s = LDRPU
  ElseIf GetGadgetText(3) = "Duemilanove"
    PP.s = LDRPD
  EndIf
  
  PP = ReplaceString(PP, "%1", GetGadgetText(2)) ; number port
  PP = ReplaceString(PP, "%2", GetGadgetText(11)) ; path to hex
;   PP = ReplaceString(PP, "%3", "115200")
  MessageRequester("runProgram", LDRF + " " + PP)

  avrdudeStat = RunProgram(LDRF, PP, "", #PB_Program_Open | #PB_Program_Read)
  If avrdudeStat
;     Debug avrdudeStat
  Else
    MessageRequester("ERROR",
                     "Целостность программы нарушена!" + Chr(10) + " Невозможно открыть avrdude.",
                     #PB_MessageRequester_Error)
;     MessageRequester("ERROR", "Программа загрузки не может быть запущена!", #PB_MessageRequester_Error)
  EndIf
  
EndProcedure

Procedure checkPort(PORT.s)
  
  
  ProcedureReturn false
EndProcedure


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
  TextGadget(103, 8, 80, 80, 24, "Тип модуля")
  TextGadget(102, 8, 112, 80, 24, "HEX фаил")
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
  If #disableDef
    DisableGadget(3, #True)
    DisableGadget(4, #True)
    DisableGadget(1, #True)
  EndIf
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
          loadHex()
        Case 2
          DisableGadget(3, #False)
        Case 3
          DisableGadget(4, #False)
        Case 4
          file$ = OpenFileRequester("Выбрать HEX", GetCurrentDirectory(),
                                    "HEX прошивка (.hex)|*.hex|Все файлы|*.*", 0)
          If file$
            SetGadgetText(11,file$)
            DisableGadget(1, #False)
          EndIf
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


; IDE Options = PureBasic 5.11 (Windows - x86)
; CursorPosition = 92
; FirstLine = 71
; Folding = -
; EnableUnicode
; EnableXP
; UseIcon = icon-nobackground.ico
; Executable = loader.exe