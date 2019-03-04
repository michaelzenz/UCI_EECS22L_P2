echo "Start running server on port 11000"
rm -rf build
mkdir build
cd build
cmake ..
make 
cd ..
