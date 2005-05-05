# Microsoft Developer Studio Project File - Name="guichan" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=guichan - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "guichan.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "guichan.mak" CFG="guichan - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "guichan - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "guichan - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "guichan - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "msvc_intermediate"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GUICHAN_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GUICHAN_EXPORTS" /D "GUICHAN_BUILD" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /I "..\include include ../include ../../include guichan/include ../include/guichan" /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "guichan - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib"
# PROP Intermediate_Dir "msvc_intermediate_dbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GUICHAN_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GUICHAN_EXPORTS" /D "GUICHAN_BUILD" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"lib/guichand.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "guichan - Win32 Release"
# Name "guichan - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "widgets"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\widgets\button.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widgets\checkbox.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widgets\container.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widgets\dropdown.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widgets\icon.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widgets\label.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widgets\listbox.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widgets\radiobutton.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widgets\scrollarea.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widgets\slider.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widgets\textbox.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widgets\textfield.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widgets\window.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\cliprectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\src\color.cpp
# End Source File
# Begin Source File

SOURCE=.\src\defaultfont.cpp
# End Source File
# Begin Source File

SOURCE=.\src\exception.cpp
# End Source File
# Begin Source File

SOURCE=.\src\focushandler.cpp
# End Source File
# Begin Source File

SOURCE=.\src\font.cpp
# End Source File
# Begin Source File

SOURCE=.\src\graphics.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui.cpp
# End Source File
# Begin Source File

SOURCE=.\src\guichan.cpp
# End Source File
# Begin Source File

SOURCE=.\src\image.cpp
# End Source File
# Begin Source File

SOURCE=.\src\imagefont.cpp
# End Source File
# Begin Source File

SOURCE=.\src\key.cpp
# End Source File
# Begin Source File

SOURCE=.\src\keyinput.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mouseinput.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\src\widget.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "widgetsh"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\guichan\widgets\button.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\widgets\checkbox.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\widgets\dropdown.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\widgets\icon.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\widgets\label.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\widgets\listbox.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\widgets\radiobutton.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\widgets\scrollarea.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\widgets\slider.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\widgets\textbox.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\widgets\textfield.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\widgets\window.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\include\guichan\actionlistener.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\basiccontainer.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\cliprectangle.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\color.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\defaultfont.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\exception.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\focushandler.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\font.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\fontwidget.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\graphics.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\gui.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\image.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\imagefont.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\imageloader.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\input.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\key.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\keyinput.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\keylistener.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\listmodel.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\mouseinput.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\mouselistener.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\platform.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\rectangle.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\widget.hpp
# End Source File
# Begin Source File

SOURCE=.\include\guichan\x.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
