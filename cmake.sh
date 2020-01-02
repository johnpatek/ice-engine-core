if [ ! -d "build" ]; then
    mkdir build
fi

if [ ! -d "install" ]; then
    mkdir install
fi

cd build
cmake ..
cd ..
cmake --build build --target install