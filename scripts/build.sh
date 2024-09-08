#!/bin/bash

# Находим полный путь до корня проекта
PROJECT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )"

# Создаем директорию для сборки
BUILD_DIR="$PROJECT_ROOT/build"
mkdir -p "$BUILD_DIR"

# Переходим в директорию сборки
cd "$BUILD_DIR"

if [[ "$1" == "--configure" ]]; then
  # Вызываем cmake для генерации файлов сборки
  cmake "$PROJECT_ROOT" -DCMAKE_BUILD_TYPE=Release
fi

# Выполняем make для компиляции проекта
cmake --build . -j 12
