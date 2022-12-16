zephyr_include_directories(.)
target_sources(app PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/helper.c
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt_timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/logimouse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/radio_communication.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/radio_controls.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/radio_unifying.cpp)