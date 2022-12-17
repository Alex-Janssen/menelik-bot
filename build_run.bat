set name="Adventures of Kingdom Quest"
g++ x.cpp -w -Wl,-subsystem,windows -o %name% -DNAME=%name%


if "%1" == "ZIP" goto zip
%name%.exe
exit /b

:zip
"C:\Program Files\7-Zip\7z.exe" a -tzip %name% "media" %name%.exe "SDL2.dll" "SDL2_image.dll"
%name%.exe
exit /b
