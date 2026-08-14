# Тесты для DancingLinksMatrix
add_executable(test_dancing_links
    dancing_links/test_dancing_links.cpp
 )

target_link_libraries(test_dancing_links
    dlx
)

target_include_directories(test_dancing_links PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Тесты для SudokuSolver
add_executable(test_sudoku
    sudoku/test_sudoku.cpp
 )

target_link_libraries(test_sudoku
    dlx
)

target_include_directories(test_sudoku PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Тесты JSON-ввода/вывода консольного приложения
add_executable(test_sudoku_json_io
    app/test_sudoku_json_io.cpp
)

target_include_directories(test_sudoku_json_io PRIVATE
    ${CMAKE_SOURCE_DIR}/app
)

# Добавляем тесты в CTest
add_test(NAME DancingLinksTests COMMAND test_dancing_links)
add_test(NAME SudokuSolverTests COMMAND test_sudoku)
add_test(NAME SudokuJsonIoTests COMMAND test_sudoku_json_io)

# Для Windows: консольные приложения
if(WIN32)
    set_target_properties(test_dancing_links test_sudoku test_sudoku_json_io PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()
