#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32   //under windows 2 compilers MSVC and GNU GCC
#ifdef _MSC_VER 
const char *pwsh_string = "if(!(Test-Path(\"..\\bin\")))\n\
{\n\
        New-Item -Itemtype Directory \"..\\bin\"\n\
}\n\
if(!(Test-Path(\"..\\msvc\")))\n\
{\n\
        New-Item -Itemtype Directory \"..\\msvc\"\n\
        $SDL2 = \"https://www.libsdl.org/release/SDL2-devel-2.0.12-VC.zip\"\n\
        Push-Location ..\\msvc\n\
        New-Item -Itemtype Directory \"lib\"\n\
        New-Item -Itemtype Directory \"include\"\n\
        New-Item -Itemtype Directory \"include/SDL2\"\n\
        #curl SDL2 download link\n\
        Invoke-WebRequest -Uri $SDL2 -OutFile \"SDL2.zip\"\n\
        #extract SDL2\n\
        Expand-Archive SDL2.zip\n\
        #copy SDL2.dll to bin\n\
        Push-Location \".\\SDL2\"\n\
        Copy-Item \"SDL2-2.0.12\\lib\\x64\\SDL2.dll\" ..\\..\\bin\n\
        #copy lib files to lib folder\n\
        Copy-Item \"SDL2-2.0.12\\lib\\x64\\*.lib\" ..\\lib\n\
        Copy-Item \"SDL2-2.0.12\\lib\\x64\\*.dll\" ..\\lib\n\
        #copy include files to include folder\n\
        Copy-Item \"SDL2-2.0.12\\include\\*.h\" \"..\\include\\SDL2\"\n\
        Pop-Location\n\
}\n\
$source_name = \"..\\code\\main.c\",\"..\\code\\sorts.c\",\"..\\code\\font.c\"\n\
$executable_name = \"sort_viz.exe\"\n\
$lib_path =\"..\\msvc\\lib\\\"\n\
$include_path =\"..\\msvc\\include\"\n\
$compiler_flags = \"/nologo\",\"/EHsc\",\"/Zi\" ,\"/FC\"\n\
$libraries = \"SDL2main.lib\",\"SDL2.lib\", \"user32.lib\", \"shell32.lib\", \"gdi32.lib\"\n\
Push-Location ..\\bin\n\
if(!(Test-Path(\"SDL2.dll\")))\n\
{\n\
        Copy-Item \"../msvc/lib/SDL2.dll\"\n\
}\n\
cl  $source_name /Fe$executable_name $compiler_flags /I$include_path /link /LIBPATH:$lib_path $libraries /NODEFAULTLIB:msvcrt.lib /SUBSYSTEM:CONSOLE\n\
./sort_viz.exe";

#elif __GNUC__
const char *gcc_win_str = "#download sdl2 for gcc\n\
if(!(Test-Path(\"..\\gcc\")))\n\
{\n\
New-Item -Itemtype Directory \"..\\gcc\"\n\
$SDL2 = \"https://www.libsdl.org/release/SDL2-devel-2.0.12-mingw.tar.gz\"\n\
Push-Location ..\\gcc\n\
New-Item -Itemtype Directory \"lib\"\n\
New-Item -Itemtype Directory \"include\"\n\
#curl SDL2 download link\n\
Invoke-WebRequest -Uri $SDL2 -OutFile \"SDL2.tar.gz\"\n\
#extract SDL2\n\
tar -xvf SDL2.tar.gz\n\
#copy SDL2.dll to bin\n\
Copy-Item \"SDL2-2.0.12\\x86_64-w64-mingw32\\bin\\SDL2.dll\" .\\lib\n\
#copy lib files to lib folder\n\
Copy-Item \"SDL2-2.0.12\\x86_64-w64-mingw32\\lib\\*.lib\" .\\lib\n\
Copy-Item \"SDL2-2.0.12\\x86_64-w64-mingw32\\lib\\*.la\" .\\lib\n\
Copy-Item \"SDL2-2.0.12\\x86_64-w64-mingw32\\lib\\*.a\" .\\lib\n\
#copy include files to include folder\n\
Copy-Item \"SDL2-2.0.12\\x86_64-w64-mingw32\\include\\SDL2\" .\\include -Recurse\n\
Pop-Location\n\
}\n\
if(!(Test-Path(\"..\\bin\")))\n\
{\n\
        New-Item -Itemtype Directory \"..\\bin\"\n\
}\n\
if((Test-Path(\"..\\include\\SDL2\")))\n\
{\n\
        Remove-Item -Recurse -Force \"..\\include\\SDL2\"\n\
}\n\
Push-Location \"..\\bin\"\n\
if(!(Test-Path(\"SDL2.dll\")))\n\
{\n\
        Copy-Item \"../gcc/lib/SDL2.dll\"\n\
}\n\
$source_name = \"..\\code\\main.c\",\"..\\code\\sorts.c\",\"..\\code\\font.c\"\n\
$executable_name = \"sort_viz.exe\"\n\
$lib_path =\"..\\gcc\\lib\\\"\n\
$include_path =\"..\\gcc\\include\"\n\
#gcc compiler flags\n\
gcc -o $executable_name $source_name -I$include_path -L$lib_path -lmingw32 -lSDL2main -lSDL2\n";
#endif //end of compilers for windows
#elif __linux__  //only gcc for linux
const char *bash_string = "#!/bin/bash\n\
# build script fors SDL in bash using gcc\n\
if [ ! -d \"../bin\" ]; then\n\
  mkdir \"../bin\"\n\
fi\n\
cd ../bin\n\
source_name=\"../code/main.c ../code/sorts.c\",\"..\\code\\font.c\"\n\
executable_name=\"sort_viz\"\n\
# Compile the program\n\
gcc  $source_name -w -lm -lSDL2 -o $executable_name\n\
\n\
# Run the program\n\
./sort_viz\n\
";
#elif __APPLE__
const char *bash_string = "# build script fors SDL in bash using gcc\n\
#!/bin/bash\n\
if [ ! -d \"../bin\" ]; then\n\
  mkdir \"../bin\"\n\
fi\n\
cd ../bin\n\
source_name=\"../code/main.c ../code/sorts.c\",\"..\\code\\font.c\"\n\
executable_name=\"sort_viz\"\n\
# Compile the program\n\
gcc  $source_name -w -lm -lSDL2 -o $executable_name\n\
\n\
# Run the program\n\
./sort_viz\n\
";
#endif
int main()
{

#ifdef _WIN32
// for msvc compiler
// ifdef for msvc
#ifdef _MSC_VER
    FILE *script = fopen("../code/build.ps1", "w");
    fwrite(pwsh_string, 1, strlen(pwsh_string), script);
    fclose(script);
    system("echo powershell -ExecutionPolicy Bypass -File ../code/build.ps1 &");
#elif __GNUC__
    FILE *script = fopen("../code/buildgcc.ps1", "w");
    fwrite(gcc_win_str, 1, strlen(gcc_win_str), script);
    fclose(script);
    system("powershell -ExecutionPolicy Bypass -File ../code/buildgcc.ps1 &");
#endif
#elif __linux__
    FILE *script = fopen("../code/build.sh", "w");
    fwrite(bash_string, 1, strlen(bash_string), script);
    fclose(script);
    system("bash ../code/build.sh &");
#elif __APPLE__
    FILE *script = fopen("../code/build.sh", "w");
    fwrite(bash_string, 1, strlen(bash_string), script);
    fclose(script);
    system("bash ../code/build.sh &");
#endif
    /*
    free(data);
    free(main_data);
    free(final_str);
    */
    // letting os free the memory is faster

    return 0;
}