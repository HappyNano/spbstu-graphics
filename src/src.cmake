# Рекурсивно добавляем исходные файлы в проект
function(add_sources_recursive dir)
    file(GLOB_RECURSE sources ${dir}/*.cpp ${dir}/*.c)
    list(APPEND all_sources ${sources})

    set(SOURCES ${SOURCES} ${sources} PARENT_SCOPE)

    foreach(subdir IN LISTS ARGV)
        if(IS_DIRECTORY ${dir}/${subdir})
            add_sources_recursive(${dir}/${subdir})
        endif()
    endforeach()
endfunction()

# Добавляем исходные файлы в проект
set(SOURCES "")
add_sources_recursive(${CMAKE_CURRENT_SOURCE_DIR}/src/lab)
