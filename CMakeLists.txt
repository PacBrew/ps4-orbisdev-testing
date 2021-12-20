cmake_minimum_required(VERSION 3.21)
project(ps4_orbisdev_test)

file(GLOB SOURCES source/*.cpp)

add_executable(ps4_orbisdev_test ${SOURCES})
add_self(ps4_orbisdev_test)
add_pkg(ps4_orbisdev_test ${CMAKE_SOURCE_DIR}/pkg "PACB00001" "pacbrew orbisdev test" "01.00")

target_link_libraries(ps4_orbisdev_test
        SceSysmodule_stub
        SceSystemService_stub
        SceNet_stub
        SceUserService_stub
        ScePigletv2VSH_stub
        SceVideoOut_stub
        SceGnmDriver_stub
        orbis
        ScePad_stub
        SceAudioOut_stub
        SceIme_stub
        z
        )
