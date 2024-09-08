cmake_minimum_required(VERSION 3.12)
project(lab1 VERSION 0 LANGUAGES CXX)

# Включаем генерацию compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Задаем переменные для директорий
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)

# Добавляем подмодули Git из .gitmodules
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.gitmodules")
    message(STATUS "Submodule update")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMOD_RESULT
    )
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
endif()

# Тут добавлять библиотеки find_package...

find_package(OpenGL REQUIRED)
add_subdirectory(${EXTERNAL_DIR}/glfw)

# Добавляем пути к заголовочным файлам

include(${SRC_DIR}/src.cmake)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${INCLUDE_DIR}
    ${EXTERNAL_DIR}/glfw/include
    ${EXTERNAL_DIR}/glm
)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY})

# ====================

# Проверка форматирования с помощью clang-format
set(CLANG_FORMAT_COMMAND /usr/bin/clang-format-15)
set(CLANG_FORMAT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/.clang-format)

# Добавляем пользовательскую цель для автоматической правки кода
add_custom_target(
    clang-format-fix
    COMMAND ${CLANG_FORMAT_COMMAND} -style=file -i ${SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running clang-format to fix code style..."
    VERBATIM
)

# Добавляем правку кода к цели построения
add_dependencies(${PROJECT_NAME} clang-format-fix)
