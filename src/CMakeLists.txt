add_library(miemsense_logic STATIC
    math.cpp
    math.h
    SDK.cpp
    SDK.h
    structs.h
)

target_include_directories(miemsense_logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(MIEMsense main.cpp)

target_link_libraries(MIEMsense PRIVATE miemsense_logic)
