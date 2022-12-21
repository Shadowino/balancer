; RunProgram(#PB_Compiler_Home+"/Compilers/pbcompiler", "/?", "", #PB_Program_Open | #PB_Program_Read)
progf.s = "C:\PORTABLE\gcUploader\avrdude"
progp.s = "-F -v -p m328p -c arduino -P COM6 -b 115200 -D -U flash:w:"
progp = progp + Chr(34) + "D:\2\2.ino.standard.hex"+ Chr(34) + ":i"
; Debug progf + " " + progp
RunProgram(progf, progp, "", #PB_Program_Open | #PB_Program_Read | #PB_Program_Write)

; IDE Options = PureBasic 5.11 (Windows - x86)
; CursorPosition = 5
; EnableXP