# Microsoft Developer Studio Project File - Name="EngesnOSL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=EngesnOSL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EngesnOSL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EngesnOSL.mak" CFG="EngesnOSL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EngesnOSL - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "EngesnOSL - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EngesnOSL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "EngesnOSL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "EngesnOSL - Win32 Release"
# Name "EngesnOSL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "x86"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\x86\ACPI.cpp
# End Source File
# Begin Source File

SOURCE=..\x86\bios32.cpp
# End Source File
# Begin Source File

SOURCE=..\x86\cmos.cpp
# End Source File
# Begin Source File

SOURCE=..\x86\CPUDetect.cpp
# End Source File
# Begin Source File

SOURCE=..\x86\GDT.cpp
# End Source File
# Begin Source File

SOURCE=..\x86\Interrupt.cpp
# End Source File
# Begin Source File

SOURCE=..\x86\PIT.cpp
# End Source File
# Begin Source File

SOURCE=..\x86\speaker.cpp
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\lib\IStream.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\OStream.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\string.cpp
# End Source File
# End Group
# Begin Group "drv"

# PROP Default_Filter ""
# Begin Group "keyb"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\drv\keyb\kbInt.cpp
# End Source File
# Begin Source File

SOURCE=..\drv\keyb\keyboard.cpp
# End Source File
# End Group
# Begin Group "vid"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\drv\vid\Modes.cpp
# End Source File
# Begin Source File

SOURCE=..\drv\vid\Video.cpp
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=..\asmf.cpp
# End Source File
# Begin Source File

SOURCE=..\end.cpp
# End Source File
# Begin Source File

SOURCE=..\Kernel.cpp
# End Source File
# Begin Source File

SOURCE=..\semaphore.cpp
# End Source File
# Begin Source File

SOURCE=..\Support.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\ACPI.h
# End Source File
# Begin Source File

SOURCE=..\include\asmf.H
# End Source File
# Begin Source File

SOURCE=..\include\bios32.h
# End Source File
# Begin Source File

SOURCE=..\include\cmos.h
# End Source File
# Begin Source File

SOURCE=..\include\CPUDetect.h
# End Source File
# Begin Source File

SOURCE=..\include\end.h
# End Source File
# Begin Source File

SOURCE=..\include\GDT.h
# End Source File
# Begin Source File

SOURCE=..\include\Interrupt.h
# End Source File
# Begin Source File

SOURCE=..\include\IOStream.h
# End Source File
# Begin Source File

SOURCE=..\include\IStream.h
# End Source File
# Begin Source File

SOURCE=..\include\keyboard.h
# End Source File
# Begin Source File

SOURCE=..\include\limits.h
# End Source File
# Begin Source File

SOURCE=..\include\maths.h
# End Source File
# Begin Source File

SOURCE=..\include\Modes.h
# End Source File
# Begin Source File

SOURCE=..\include\OStream.h
# End Source File
# Begin Source File

SOURCE=..\include\PIT.h
# End Source File
# Begin Source File

SOURCE=..\include\semaphore.h
# End Source File
# Begin Source File

SOURCE=..\include\speaker.h
# End Source File
# Begin Source File

SOURCE=..\include\string.h
# End Source File
# Begin Source File

SOURCE=..\include\types.h
# End Source File
# Begin Source File

SOURCE=..\include\Video.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
