setlocal
call boostvars64.bat

set OUTDIR=vs2013.x64

if exist "%OUTDIR%" (
  rmdir /S /Q "%OUTDIR%"
)
mkdir "%OUTDIR%"

pushd "%OUTDIR%"
cmake -G "Visual Studio 12 2013 Win64" -D BUILD_TEST=ON -D BUILD_EXAMPLES=ON ..\..
popd
endlocal