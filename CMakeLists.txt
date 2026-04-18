cmake_minimum_required(VERSION 3.15)
project(Sudoku)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

# ============================================================================
# Библиотека DLX (общая для тестов и основной программы)
# ============================================================================
add_library(dlx STATIC
    src/dancing_links/dancing_links_matrix.cpp
)

target_include_directories(dlx PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# ============================================================================
# Основная программа (пока закомментирована, раскомментируем когда сделаем судоку)
# ============================================================================
add_executable(Sudoku src/main.cpp)
target_link_libraries(Sudoku dlx)

# ============================================================================
# Тесты (включаются всегда, но можно управлять через флаг)
# ============================================================================
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()