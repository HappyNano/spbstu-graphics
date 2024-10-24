# Создаем исполняемый файл для лабораторной №2
add_executable(lab3 ${SOURCES} ${SRC_DIR}/lab3_main.cpp ${EXTERNAL_DIR}/glad/src/glad.c)

target_include_directories(lab3 PRIVATE
    ${INCLUDE_DIR}
    ${EXTERNAL_DIR}/glad/include
    ${GLUT_INCLUDE_DIR}
)

target_link_libraries(lab3 glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} glm::glm stb::stb_image Freetype::Freetype)

set_target_properties(lab3 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY})

# Добавляем правку кода к цели построения
add_dependencies(lab3 clang-format-fix)
