AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/mod_battlepass.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/mod_battlepass.h")
AC_ADD_SCRIPT_LOADER("mod_battlepass" "${CMAKE_CURRENT_LIST_DIR}/src/loader.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/mod_sample.conf.dist")