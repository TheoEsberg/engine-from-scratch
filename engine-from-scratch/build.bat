set render=src\engine\render\render.c src\engine\render\render_init.c src\engine\render\render_util.c
set io=src\engine\io\io.c
set files=src\glad.c src\main.c src\engine\global.c %render% %io%
set libs=D:\C\engine-from-scratch\lib\SDL3.lib D:\C\engine-from-scratch\lib\freetype.lib

CL /Zi /I D:\C\engine-from-scratch\include %files% /link %libs% /OUT:mygame.exe