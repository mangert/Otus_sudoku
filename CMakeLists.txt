# CMakeList.txt : CMake project for Sudoku, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Sudoku")

# Подключаем include директории
include_directories(${CMAKE_SOURCE_DIR}/include)

# Собираем библиотеку DLX
#add_library(dlx STATIC
    src/dancing_links/dancing_links_matrix.cpp    
#)

# Собираем библиотеку судоку
#add_library(sudoku_lib STATIC
    #src/sudoku/sudoku_solver.cpp
    #src/sudoku/exact_cover_builder.cpp
#)

# Add source to this project's executable.
add_executable (Sudoku  "src/main.cpp")

target_link_libraries(Sudoku
    #dlx
    #sudoku_lib
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Sudoku PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
