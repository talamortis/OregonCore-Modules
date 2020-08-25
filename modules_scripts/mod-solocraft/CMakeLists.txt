
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/Solocraft.cpp")

AC_ADD_SCRIPT_LOADER("Solocraft" "${CMAKE_CURRENT_LIST_DIR}/src/loader.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/Solocraft.conf.dist")
