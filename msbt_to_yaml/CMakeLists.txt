add_executable(msbt_to_yaml
    ./src/include/io/file.h
    ./src/main.cpp
)

if(WIN32 AND NOT MSVC)
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static -static-libgcc -static-libstdc++")
endif()

target_include_directories(msbt_to_yaml PRIVATE ./src/include)
target_link_libraries(msbt_to_yaml PUBLIC msbt)
