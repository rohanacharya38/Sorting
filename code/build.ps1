if(!(Test-Path("..\bin")))
{
        New-Item -Itemtype Directory "..\bin"
}
if(!(Test-Path("..\msvc")))
{
        New-Item -Itemtype Directory "..\msvc"
        $SDL2 = "https://www.libsdl.org/release/SDL2-devel-2.0.12-VC.zip"
        Push-Location ..\msvc
        New-Item -Itemtype Directory "lib"
        New-Item -Itemtype Directory "include"
        New-Item -Itemtype Directory "include/SDL2"
        #curl SDL2 download link
        Invoke-WebRequest -Uri $SDL2 -OutFile "SDL2.zip"
        #extract SDL2
        Expand-Archive SDL2.zip
        #copy SDL2.dll to bin
        Push-Location ".\SDL2"
        Copy-Item "SDL2-2.0.12\lib\x64\SDL2.dll" ..\..\bin
        #copy lib files to lib folder
        Copy-Item "SDL2-2.0.12\lib\x64\*.lib" ..\lib
        Copy-Item "SDL2-2.0.12\lib\x64\*.dll" ..\lib
        #copy include files to include folder
        Copy-Item "SDL2-2.0.12\include\*.h" "..\include\SDL2"
        Pop-Location
}
$source_name = "..\code\main.c","..\code\sorts.c","..\code\font.c"
$executable_name = "sort_viz.exe"
$lib_path ="..\msvc\lib\"
$include_path ="..\msvc\include"
$compiler_flags = "/nologo","/EHsc","/Zi" ,"/FC"
$libraries = "SDL2main.lib","SDL2.lib", "user32.lib", "shell32.lib", "gdi32.lib"
Push-Location ..\bin
if(!(Test-Path("SDL2.dll")))
{
        Copy-Item "../msvc/lib/SDL2.dll"
}
cl  $source_name /Fe$executable_name $compiler_flags /I$include_path /link /LIBPATH:$lib_path $libraries /NODEFAULTLIB:msvcrt.lib /SUBSYSTEM:CONSOLE
./sort_viz.exe