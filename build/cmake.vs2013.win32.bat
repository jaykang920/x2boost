setlocal
call boostvars32.bat

set OUTDIR=vs2013.win32

if exist "%OUTDIR%" (
  rmdir /S /Q "%OUTDIR%"
)
mkdir "%OUTDIR%"

pushd "%OUTDIR%"
cmake -G "Visual Studio 12 2013" -D BUILD_TEST=ON -D BUILD_SAMPLES=ON ..\..
popd
endlocal