set BAT_DIR=%~dp0

%BAT_DIR%PhotoResize392x448IO.exe "%BAT_DIR%..\temp"

%BAT_DIR%PhotoResize141x161O.exe "%BAT_DIR%..\temp\300.jpg"

move "%BAT_DIR%..\temp\300-141x161.jpg" "%BAT_DIR%..\temp\thumb.jpg"

xcopy /I /Y "%BAT_DIR%..\temp" "C:\Users\Public\Pictures\highSpeed\set\%1"
