#include <stdio.h>
#include <stdlib.h>

const char *bash_string= "# build script fors SDL in bash using gcc\n\
#!/bin/bash\n\
if [ ! -d \"../bin\" ]; then\n\
  mkdir \"../bin\"\n\
fi\n\
cd ../bin\
source_name=\"../code/main.c ../code/sorts.c\"\n\
executable_name=\"sort_viz\"\n\
# Compile the program\n\
gcc  $source_name -w -lm -lSDL2 -o $executable_name\n\
\n\
# Run the program\n\
./sort_viz\n\
";

const char *pwsh_string="if(!(Test-Path(\"..\\bin\")))\n\
{\n\
        New-Item -Itemtype Directory \"..\\bin\"\n\
}\n\
$source_name = \"..\\code\\main.c\",\"..\\code\\sorts.c\"\n\
$executable_name = \"sort_viz.exe\"\n\
$lib_path =\"..\\lib\\\"\n\
$include_path =\"..\\include\"\n\
$compiler_flags = \"/nologo\",\"/EHsc\",\"/Zi\" ,\"/FC\"\n\
$libraries = \"SDL2main.lib\",\"SDL2.lib\", \"user32.lib\", \"shell32.lib\", \"gdi32.lib\"\n\
Push-Location ..\\bin\n\
if(!(Test-Path(\"SDL2.dll\")))\n\
{\n\
        Copy-Item \"../lib/SDL2.dll\"\n\
}\n\
cl  $source_name /Fe$executable_name $compiler_flags /I$include_path /link /LIBPATH:$lib_path $libraries /NODEFAULTLIB:msvcrt.lib /SUBSYSTEM:CONSOLE\n\
./sort_viz.exe";
int main()
{
    // open sorts.h and list all function names present there
    char *data;
    FILE *sortsh;
    sortsh = fopen("../code/sorts.h", "r");
    // allocate array size equal to number of characters needed to store all function names
    long fsize;
    fseek(sortsh, 0L, SEEK_END);
    fsize = ftell(sortsh);
    fseek(sortsh, 0L, SEEK_SET);
    int i = 0;
    data = (char *)malloc(fsize + 100);
    memset(data, 0, fsize + 100);

    char c;
    int rows=0;
    while ((c = fgetc(sortsh)) != EOF)
    {
        if (c == 'v')
        {
            c = fgetc(sortsh);
            if (c == 'o')
            {
                c = fgetc(sortsh);
                if (c == 'i')
                {
                    c = fgetc(sortsh);
                    if (c == 'd')
                    {
                        c = fgetc(sortsh);
                        if (c == ' ')
                        {
                            i += sprintf(data + i, "%d.", rows);
                            rows++;
                            while ((c = fgetc(sortsh)) != '(')
                            {
                                data[i] = c;
                                i++;
                            }
                            data[i] = '\n';
                            i++;
                        }
                    }
                }
            }
        }
    }
    fclose(sortsh);

    char **functions= malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++)
    {
        functions[i] = malloc(100 * sizeof(char));
    }
    int j = 0;
    int k = 0;
    for (int i = 0; i < strlen(data); i++)
    {
        if (data[i] == '.')
        {
            for(k=0;data[i+1]!='\n';i++,k++)
            {
                functions[j][k]=data[i+1];
            }
            functions[j][k]='\0';
            j++;
        }
        
    }
    
    fsize=0;
    int final_index=0;
    long main_fsize=0;
    FILE *finalptr=fopen("../code/main.c","w");
    FILE *mainptr=fopen("../misc/main.c","r");
    char *main_data;
    fseek(mainptr, 0L, SEEK_END);
    main_fsize = ftell(mainptr);
    fsize+=main_fsize;
    fseek(mainptr, 0L, SEEK_SET);
    fsize+= strlen(data);
    for (int i = 0; i < rows; i++)
    {
        fsize+=strlen(functions[i]);
    }
    fsize+=rows*strlen("case SDL_SCANCODE_0:\ncase SDL_SCANCODE_KP_0:\nbreak;\n");
    fsize+=strlen("int32_t switch_function(void *keycode)\n{\nSDL_Scancode key=*((SDL_Scancode *)keycode);\nswitch (key)\n\n{\ndefault:\nbreak;\n}\nthread_created=false\nreturn 0;\n}\n");
    main_data=malloc(main_fsize+100);
    memset(main_data, 0, main_fsize+100);
    fread(main_data,1,main_fsize,mainptr);
    fclose(mainptr);
    main_data[main_fsize]='\0';
    char *final_str=malloc(fsize+100);
    memset(final_str, 0, fsize+100);
    final_index+=sprintf(final_str,"const char *display_str=\"");
    for (int i = 0; i < rows; i++)
    {
        final_index+=sprintf(final_str+final_index,"%d.%s\\n",i,functions[i]);
    }
    final_index+=sprintf(final_str+final_index,"\";\n%s\n",main_data);
    final_index+=sprintf(final_str+final_index,"int32_t switch_function(void *keycode)\n{\nSDL_Scancode key=*((SDL_Scancode *)keycode);\nswitch (key)\n\n{\n");
    for (int i = 0; i < rows; i++)
    {
        final_index+=sprintf(final_str+final_index,"case SDL_SCANCODE_%d:\ncase SDL_SCANCODE_KP_%d:\n%s(sRects, RECT_COUNT);\nbreak;\n",i,i,functions[i]);
    }
    final_index+=snprintf(final_str+final_index,strlen("default:\nbreak;\n}\nthread_created=false;\nreturn 0;\n}\n"),"default:\nbreak;\n}\nthread_created=false;\nreturn 0;\n}\n");
    final_str[final_index]='\0';
    fwrite(final_str,1,final_index-1,finalptr);
    fclose(finalptr);
    
    #ifdef _WIN32
    FILE *script=fopen("../code/build.ps1","w");
    fwrite(pwsh_string,1,strlen(pwsh_string),script);
    fclose(script);
    system("powershell -ExecutionPolicy Bypass -File ../code/build.ps1");
    #elif __linux__
    FILE *script=fopen("../code/build.sh","w");
    fwrite(bash_string,1,strlen(bash_string),script);
    fclose(script);
    system("bash ../code/build.sh");
    #endif
    /*
    free(data);
    free(main_data);
    free(final_str);
    */
   //letting os free the memory is faster
    
    return 0;
}