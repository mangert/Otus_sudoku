cmake_minimum_required(VERSION 3.15)
project(Sudoku)

# Стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Отключаем предупреждения MSVC о unsafe-функциях (опционально)
if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

# Создаём исполняемый файл со всеми исходниками
add_executable(Sudoku
    src/main.cpp
    src/dancing_links/dancing_links_matrix.cpp
    # позже раскомментировать:
    # src/sudoku/sudoku_solver.cpp
    # src/sudoku/exact_cover_builder.cpp
)

# Указываем где искать заголовочные файлы
target_include_directories(Sudoku PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Для Windows: явно указываем консольное приложение
if(WIN32)
    set_target_properties(Sudoku PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()

# Для Debug-сборки добавляем флаг DEBUG
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()