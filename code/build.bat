@echo off
setlocal
set source_name="main.c"
set executable_name=sort_viz
set lib_path=lib\
set include_path=include\
set compiler_flags=/nologo /EHsc /Zi /FC
set libraries=SDL2.lib SDL2main.lib user32.lib shell32.lib gdi32.lib
cl -Zi  %source_name% /Fe%executable_name% %compiler_flags% /I%include_path% /link /LIBPATH:%lib_path% %libraries% /SUBSYSTEM:windows
echo --Compilation Success--