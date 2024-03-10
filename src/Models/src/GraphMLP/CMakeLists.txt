cmake_minimum_required(VERSION 3.10)
project(GraphNeuralNetwork)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -fopenmp")

# Добавьте все исходные файлы вашего проекта
set(SOURCE_FILES
        ../ActivateFunction/ActivateFunction.cc
        GraphMLP.cc
        ..//Parser/ParseFile.cc
        ../Matrix/VecMatrix.cc
)

# Добавьте флаги для отладки
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

add_executable(GraphNeuralNetwork ${SOURCE_FILES})
