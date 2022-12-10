
########################################################
# Files

set (txtinput_SRCS
  txtinput.cpp
  txtinputDock.cpp
  txtexporttopostgis.cpp
  geopackage.cpp
  txtreader.cpp
  txtwriter.cpp
  odatadefine.cpp
  odata.cpp
  odatalayer.cpp
  odatafeature.cpp
  odataattributes.cpp
  odatageometry.cpp
  ellipsoid.cpp
  projection.cpp
  gauss.cpp
)

set (txtinput_UIS
    txtinputDock.ui
)

set (txtinput_RCCS  txtinput.qrc)

########################################################
# Build

if (BUILD_WITH_QT6)
  QT6_WRAP_UI (txtinput_UIS_H  ${txtinput_UIS})
else()
  # 我这边使用QT5，对应的宏定义源代码在ui/CMakeLists.txt里面
  QT5_WRAP_UI (txtinput_UIS_H  ${txtinput_UIS})
endif()

add_library (plugin_txtinput MODULE ${txtinput_SRCS} ${txtinput_RCCS} ${txtinput_UIS_H})

# require c++17
target_compile_features(plugin_txtinput PRIVATE cxx_std_17)

include_directories(SYSTEM
  ${GEOS_INCLUDE_DIR}
)

include_directories(
  ${CMAKE_SOURCE_DIR}/src/plugins

  ${CMAKE_BINARY_DIR}/src/ui
  ${CMAKE_BINARY_DIR}/src/providers/postgres
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(plugin_txtinput
  qgis_core
  qgis_gui
  provider_postgres_a
)


########################################################
# Install

install(TARGETS plugin_txtinput
  RUNTIME DESTINATION ${QGIS_PLUGIN_DIR}
  LIBRARY DESTINATION ${QGIS_PLUGIN_DIR})

