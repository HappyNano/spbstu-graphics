apt update
apt install libglu1-mesa-dev freeglut3-dev mesa-common-dev libglfw3-dev libglm-dev libglew-dev clang-format-15 -y
mkdir installing_deps && cd installing_deps

# Install stb
git clone https://github.com/natanaeljr/stb-cmake-package.git
cd stb-cmake-package
mkdir build && cd build
cmake ..
cmake --build .
cmake --install .
cd ../..

cd ..
rm -rf installing_deps