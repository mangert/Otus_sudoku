add_executable(SudokuApp
    app.cpp
    # sudokuConsole.cpp   # раскомментируй, когда добавишь
)

target_link_libraries(SudokuApp
    dlx                    
)

target_include_directories(SudokuApp PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(WIN32)
    set_target_properties(SudokuApp PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()