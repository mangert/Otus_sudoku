# Исполняемый файл для тестов
add_executable(run_tests
    test_dancing_links.cpp
)

target_link_libraries(run_tests
    dlx
)

# Добавляем тест в CTest (для автоматического запуска через `ctest`)
add_test(NAME DancingLinksTests COMMAND run_tests)

# Для Windows: консольное приложение
if(WIN32)
    set_target_properties(run_tests PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()