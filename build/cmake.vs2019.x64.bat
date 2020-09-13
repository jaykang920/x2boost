setlocal
call boostvars64.bat

set OUTDIR=vs2019.x64

if exist "%OUTDIR%" (
  rmdir /S /Q "%OUTDIR%"
)
mkdir "%OUTDIR%"

pushd "%OUTDIR%"
cmake -G "Visual Studio 16 2019" -A Win64 -D BUILD_TEST=ON -D BUILD_SAMPLES=ON ..\..
popd
endlocal