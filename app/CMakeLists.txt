add_executable(SudokuApp
    app.cpp
    console_utils.cpp
    console_utils.h
    sudoku_json_io.h
    sudoku_ui.cpp
    sudoku_ui.h
)

target_link_libraries(SudokuApp
    dlx                    
)

target_include_directories(SudokuApp PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(MSVC)
    target_compile_options(SudokuApp PRIVATE /utf-8)
endif()

if(WIN32)
    set_target_properties(SudokuApp PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()
