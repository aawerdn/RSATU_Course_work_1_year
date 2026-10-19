cmake_minimum_required(VERSION 3.16)
project(SFML_Project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----- Путь к SFML (локальная копия) -----
set(SFML_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/SFML-3.1.0")
set(SFML_DIR "${SFML_ROOT}/lib/cmake/SFML" CACHE PATH "Path to SFML CMake config")

find_package(SFML 3.1 REQUIRED COMPONENTS Graphics Window System)

# ----- Исполняемый файл -----
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Maze.cpp
    src/PathFinder.cpp
)

target_link_libraries(${PROJECT_NAME}
    SFML::Graphics
    SFML::Window
    SFML::System
)

if(WIN32)
    set(SFML_BIN_DIR "${SFML_ROOT}/bin")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")

        set(DLL_SUFFIX "-d-3")
    else()
        set(DLL_SUFFIX "-3")
    endif()

    # Список базовых имён DLL
    set(DLL_NAMES sfml-graphics sfml-window sfml-system)
    set(DLL_FILES)

    # Проверяем существование каждой DLL
    foreach(DLL ${DLL_NAMES})
        set(DLL_PATH "${SFML_BIN_DIR}/${DLL}${DLL_SUFFIX}.dll")
        if(EXISTS "${DLL_PATH}")
            list(APPEND DLL_FILES "${DLL_PATH}")
        else()
            message(WARNING "DLL not found: ${DLL_PATH}")
        endif()
    endforeach()

    # Копируем найденные DLL в папку с исполняемым файлом
    if(DLL_FILES)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${DLL_FILES} $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying SFML DLLs"
        )
    endif()

    # Копируем шрифт (если есть)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/LiberationSans-Regular.ttf")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_CURRENT_SOURCE_DIR}/LiberationSans-Regular.ttf" $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying font file"
        )
    endif()

    # Копируем минимальные DLL MinGW для автономного запуска (опционально)
    find_program(MINGW_MAKE mingw32-make)
    if(MINGW_MAKE)
        get_filename_component(MINGW_BIN_DIR ${MINGW_MAKE} DIRECTORY)
        foreach(MINGW_DLL libstdc++-6.dll libgcc_s_seh-1.dll libwinpthread-1.dll)
            if(EXISTS "${MINGW_BIN_DIR}/${MINGW_DLL}")
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${MINGW_BIN_DIR}/${MINGW_DLL}" $<TARGET_FILE_DIR:${PROJECT_NAME}>
                )
            endif()
        endforeach()
    endif()
endif()