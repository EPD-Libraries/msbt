add_executable(msbt_test
    ./src/include/io/file.h
    ./src/main.cpp
)

if(WIN32 AND NOT MSVC)
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static -static-libgcc -static-libstdc++")
endif()

target_include_directories(msbt_test PRIVATE ./src/include)
target_link_libraries(msbt_test PUBLIC msbt)
