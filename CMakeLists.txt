cmake_minimum_required(VERSION 3.17)
project(src C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_executable(src src/main.c src/db/db_interface.c src/db/db_interface.h src/db/sqlite3.h src/db/sqlite3.c src/models/db_models.h src/ui/ui.c src/ui/ui.h res/strings.h)
