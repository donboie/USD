add_executable (subd
        main.cpp
        SceneDelegate.cpp SceneDelegate.h)

find_package(PythonLibs)
find_package(OpenGL)

target_include_directories(subd
        PUBLIC
            ${Boost_INCLUDE_DIR}
            ${TBB_ROOT}/include
            ${USD_INC_DIR}
            ${PYTHON_INCLUDE_PATH}
        )

target_link_libraries(
        subd
                ${USD_LIB_DIR}/libhd.so
                ${USD_LIB_DIR}/libhdStream.so
                ${USD_LIB_DIR}/libhdSt.so
                ${USD_LIB_DIR}/libhdx.so
                ${USD_LIB_DIR}/libtf.so
                ${USD_LIB_DIR}/libusd.so
                ${USD_LIB_DIR}/libsdf.so
                ${USD_LIB_DIR}/libgarch.so
                ${USD_LIB_DIR}/libtracelite.so
                ${USD_LIB_DIR}/libcameraUtil.so
                ${USD_LIB_DIR}/libgf.so
                ${USD_LIB_DIR}/libglf.so
                ${USD_LIB_DIR}/libvt.so
                ${USD_LIB_DIR}/libpxOsd.so
                ${OPENSUBDIV_ROOT_DIR}/lib/libosdCPU.so
                ${Boost_LIBRARIES}
                ${PYTHON_LIBRARY}
                ${OPENGL_LIBRARIES}
        )
