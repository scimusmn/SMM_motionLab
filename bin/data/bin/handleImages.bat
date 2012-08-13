set BAT_DIR=%~dp0

%BAT_DIR%PhotoResize141x169O.exe "%BAT_DIR%..\temp\300.jpg"

move "%BAT_DIR%..\temp\300-141x169.jpg" "%BAT_DIR%..\temp\thumb.jpg"

echo "%2%3%1"

xcopy /I /Y "%BAT_DIR%..\temp" "%2%3%1"
