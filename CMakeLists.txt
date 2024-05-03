cmake_minimum_required(VERSION 3.10)

project(rt_chat)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/main.c
    src/input/chat_input.c
    src/input/login_input.c
    src/ui/chat_ui.c
    src/ui/login_ui.c
    src/utils/cursor.c
    src/utils/network_manager.c
    src/utils/thread_helper.c
)

find_package(Curses REQUIRED)

add_executable(rt_chat ${SOURCES})

include_directories(include)
include_directories(${CURSES_INCLUDE_DIR})

target_link_libraries(rt_chat ${CURSES_LIBRARIES})
