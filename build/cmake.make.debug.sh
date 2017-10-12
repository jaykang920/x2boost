#/bin/sh
OUTDIR=make.debug

if [ -d "$OUTDIR" ]; then
  rm -rf "$OUTDIR"
fi
mkdir "$OUTDIR"

pushd "$OUTDIR"
cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug -D BUILD_TEST=ON -D BUILD_SAMPLES=ON ../..
popd