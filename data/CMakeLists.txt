add_subdirectory(icons)

install(PROGRAMS org.kde.kirogi.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/org.kde.kirogi.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
