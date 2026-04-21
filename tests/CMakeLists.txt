# Тесты для DancingLinksMatrix
add_executable(test_dancing_links
    dancing_links/test_dancing_links.cpp
 "sudoku/test_sudoku_utils.h")

target_link_libraries(test_dancing_links
    dlx
)

target_include_directories(test_dancing_links PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Тесты для SudokuSolver
add_executable(test_sudoku
    sudoku/test_sudoku.cpp
 "sudoku/test_sudoku_utils.h")

target_link_libraries(test_sudoku
    dlx
)

target_include_directories(test_sudoku PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Добавляем тесты в CTest
add_test(NAME DancingLinksTests COMMAND test_dancing_links)
add_test(NAME SudokuSolverTests COMMAND test_sudoku)

# Для Windows: консольные приложения
if(WIN32)
    set_target_properties(test_dancing_links test_sudoku PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()