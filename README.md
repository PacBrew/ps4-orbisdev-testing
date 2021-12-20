# orbisdev toolchain testing

- Install orbisdev toolchain and portlibs
  - `sudo pacbew-pacman -S ps4-orbisdev ps4-orbisdev-portlibs`
- Build
    - `mkdir build && cd build`
    - `source /opt/pacbrew/ps4/orbisdev/ps4vars.sh`
    - `orbis-cmake -G "Unix Makefiles" ../`
    - `make ps4_orbisdev_test_pkg`
