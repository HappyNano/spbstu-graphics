# Создаем исполняемый файл для лабораторной №2
add_executable(course ${SOURCES} ${SRC_DIR}/course_main.cpp ${EXTERNAL_DIR}/glad/src/glad.c)

target_include_directories(course PRIVATE
    ${INCLUDE_DIR}
    ${EXTERNAL_DIR}/glad/include
    ${GLUT_INCLUDE_DIR}
)

target_link_libraries(course glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} glm::glm stb::stb_image Freetype::Freetype)

set_target_properties(course PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY})

# Добавляем правку кода к цели построения
add_dependencies(course clang-format-fix)
