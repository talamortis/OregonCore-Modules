CU_SET_PATH("CMAKE_MOD_BOSS_ANNOUNCER_DIR" "${CMAKE_CURRENT_LIST_DIR}")

AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/mod_boss_announcer.cpp")

AC_ADD_SCRIPT_LOADER("Boss_Announcer" "${CMAKE_CURRENT_LIST_DIR}/src/loader_boss_announcer.h")

CU_ADD_HOOK(AFTER_WORLDSERVER_CMAKE "${CMAKE_CURRENT_LIST_DIR}/src/cmake/after_ws_install.cmake")