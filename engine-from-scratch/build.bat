set files=src\glad.c src\main.c src\engine\global.c src\engine\render\render.c src\engine\render\render_init.c
set libs=D:\C\engine-from-scratch\lib\SDL3.lib D:\C\engine-from-scratch\lib\freetype.lib

CL /Zi /I D:\C\engine-from-scratch\include %files% /link %libs% /OUT:mygame.exe