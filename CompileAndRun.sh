rm -rf build
mkdir build
cd build
cmake ..
make 
cd ..
./bin/kchat-server 11000