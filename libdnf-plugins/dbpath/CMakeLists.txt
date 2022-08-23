
# set gettext domain for translations
add_definitions(-DGETTEXT_DOMAIN=\"libdnf5\")

add_library(dbpath MODULE dbpath.cpp)
# disable the 'lib' prefix in order to create dbpath.so
set_target_properties(dbpath PROPERTIES PREFIX "")

install(TARGETS dbpath LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/libdnf5/plugins/)
