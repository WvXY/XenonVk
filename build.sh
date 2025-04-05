# build.sh
# build with CMake and make
mkdir -p "build"
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=.. -DBUILD_SHARED_LIBS=ON
make
