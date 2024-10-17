# Создаем исполняемый файл для лабораторной №2
add_executable(lab2 ${SOURCES} ${SRC_DIR}/lab2_main.cpp ${EXTERNAL_DIR}/glad/src/glad.c)

target_include_directories(lab2 PRIVATE
    ${INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${EXTERNAL_DIR}/glad/include
    ${GLUT_INCLUDE_DIR}
)

target_link_libraries(lab2 glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLUT_LIBRARIES} glm::glm stb::stb_image)

set_target_properties(lab2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY})

# Добавляем правку кода к цели построения
add_dependencies(lab2 clang-format-fix)
