set(JSONIMPORTER_FILES
    main.cpp
    jsonimporter.cpp
    jsonimporter.hpp
    label.cpp
    label.hpp
)

openmw_add_executable(omwsave-jsonimporter
    ${JSONIMPORTER_FILES}
)

target_link_libraries(omwsave-jsonimporter
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    components
)

if (BUILD_WITH_CODE_COVERAGE)
  add_definitions (--coverage)
  target_link_libraries(omwsave-jsonimporter gcov)
endif()

if (WIN32)
  INSTALL(TARGETS omwsave-jsonimporter RUNTIME DESTINATION ".")
endif(WIN32)

if (DESIRED_QT_VERSION MATCHES 4)
#    target_link_libraries(omwsave-jsonimporter ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY})
#    if(WIN32)
#        target_link_libraries(omwsave-jsonimporter ${QT_QTMAIN_LIBRARY})
#    endif(WIN32)
else()
    qt5_use_modules(omwsave-jsonimporter Widgets Core)
endif()
