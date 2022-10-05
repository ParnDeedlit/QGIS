if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS_OLD "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
endif()
# override default path where built files are put to allow running qgis without installing
# set (DEFAULT_PLUGIN_SUBDIR  lib${LIB_SUFFIX}/qgis/plugins) in QGIS/CMakeList.txt
# set (QGIS_PLUGIN_SUBDIR  ${DEFAULT_PLUGIN_SUBDIR}  CACHE STRING "Subdirectory where plugins will be installed") in QGIS/CMakeList.txt
# 如果你不修改路径 默认在对应output/lib/qgis/plugins/路径下可以找到对应的libplugin_xxx.so库
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${QGIS_OUTPUT_DIRECTORY}/${QGIS_PLUGIN_SUBDIR})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${QGIS_OUTPUT_DIRECTORY}/${QGIS_PLUGIN_SUBDIR})

add_subdirectory(topology) # 添加子目录，告诉cmake这个文件夹需要参与编译路径中
add_subdirectory(geometry_checker)
add_subdirectory(txtinput)
# 这个地方我前文说了我的环境由于重复依赖导致这个库有点问题，因此编译的时候我就添加这个宏了
if (WITH_SPATIALITE)
  add_subdirectory(offline_editing)
endif()

if (GRASS_FOUND)
  add_subdirectory(grass)
endif()

# 如果有自定义文件夹宏直接添加近来，这里我感觉由于是静态语言提前编译，直接在上面add_subdirectory即可，感觉上更为直观一些，毕竟这个地方放的是内置的插件
if (CUSTOM_PLUGINS)
  add_subdirectory(${CUSTOM_PLUGINS})
endif()


# headers installed in qgis_core target
