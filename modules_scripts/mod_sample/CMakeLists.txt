AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/mod_sample.cpp")
AC_ADD_SCRIPT_LOADER("mod_sample" "${CMAKE_CURRENT_LIST_DIR}/src/loader.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/mod_sample.conf.dist")