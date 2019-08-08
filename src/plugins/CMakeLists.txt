if (ANDROID)
    set(fileName ${CMAKE_BINARY_DIR}/kirogicore-android-dependencies.xml)
    file(WRITE "${fileName}" "<rules><dependencies><lib name='kirogicore'><depends>\n"
                                "<bundled file='${KDE_INSTALL_PLUGINDIR}/kirogi/vehiclesupport' />\n"
                                "<bundled file='${KDE_INSTALL_LIBDIR}/libQtZeroConf.so' />\n"
                             "</depends></lib></dependencies></rules>\n")
    install(FILES ${fileName} DESTINATION ${KDE_INSTALL_LIBDIR})
endif()

add_subdirectory(parrot)
add_subdirectory(ryzetello)
