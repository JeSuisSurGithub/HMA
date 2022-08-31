xmake f -m release --toolchain=gcc -p mingw --require=no
xmake -w -j $(nproc)