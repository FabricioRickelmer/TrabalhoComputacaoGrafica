cmake_minimum_required(VERSION 3.10)
project(AviaoNasNuvens)

set(CMAKE_CXX_STANDARD 17)

# Inclui os diretórios do MSYS2 (UCRT64)
set(MSYS2_ROOT "C:/msys64/ucrt64")

include_directories(${MSYS2_ROOT}/include)
link_directories(${MSYS2_ROOT}/lib)

# Cria o executável a partir do arquivo na pasta src
add_executable(AviaoNasNuvens src/main.cpp)

# Vincula as bibliotecas do OpenGL, FreeGLUT e do Windows necessárias para resolver o undefined reference
target_link_libraries(AviaoNasNuvens freeglut opengl32 glu32 winmm gdi32)