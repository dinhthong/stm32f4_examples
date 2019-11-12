echo "remove build directory's contents"
rm -rf ./build/*
cd ./build
cmake -DCMAKE_TOOLCHAIN_FILE=Toolchain-arm-none-eabi.cmake ./..
make