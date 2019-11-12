echo "remove build directory's contents"
rm -rf ./build/*
cd ./build
cmake ./..
make