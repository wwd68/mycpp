cmake_minimum_required(VERSION 3.15)

project(WxGui LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(. AllSRCS)
#message(STATUS ${AllSRCS})

add_library(WxGui SHARED ${AllSRCS})

target_compile_definitions(WxGui PRIVATE WXGUI_LIBRARY)

#搜索系统包管理器pkg-config
find_package(PkgConfig REQUIRED)
#搜索gtkmm开发库
pkg_search_module(PKG_GTKMM REQUIRED gtkmm-3.0)
#message(STATUS PKG_GTKMM_FOUND:${PKG_GTKMM_FOUND})
#message(STATUS ${PKG_GTKMM_INCLUDE_DIRS})
#message(STATUS ${PKG_GTKPKG_GTKMM_LIBRARY_DIRS})
#message(STATUS ${PKG_GTKMM_LIBRARIES})
include_directories(${PKG_GTKMM_INCLUDE_DIRS})
link_directories(${PKG_GTK_LIBRARIES})
target_link_libraries(WxGui ${PKG_GTKMM_LIBRARIES})

#搜索OpenCV库，必需是安装的，没有会错误停止
find_package(OpenCV REQUIRED)
#显示OpenCV是否找到
#message(STATUS OpenCV:${OpenCV_FOUND})
#显示OpenCV头文件目录
#message(STATUS OpenCV_INCLUDE_DIRS:${OpenCV_INCLUDE_DIRS})
#显示OpenCV连接库目录
#message(STATUS OpenCV_LIBRARY_DIRS:${OpenCV_LIBRARY_DIRS})
#显示OpenCV连接库文件
#message(STATUS OpenCV_LIBRARIES:${OpenCV_LIBRARIES})

#把OpenCV头文件目录列表加入搜索目录中
include_directories(${OpenCV_INCLUDE_DIRS})

#把OpenCV的库文件加入连接库文件表中
link_directories(${OpenCV_LIBRARIES})

#把OpenCV库文件连接到执行文件中mycpp
target_link_libraries(WxGui ${OpenCV_LIBRARIES})

# 1.在系统中寻找资源编译程序glib-compile-resources
find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)
#message(STATUS ${GLIB_COMPILE_RESOURCES})
set(myrc mygrec.rc)           #设置编译后的文件名为mygrec.rc 存入变量myrc
set(GREC_XML gladerec.xml)      #设置要编译的文件名gladerec.xml 存入变量

#message(STATUS ${GREC_XML})
# 2.添加定义命令编译资源
add_custom_command(
    OUTPUT ${myrc}      #定义的命令函数
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}   #设置工作目录为当前目录
    COMMAND ${GLIB_COMPILE_RESOURCES}   #使用之前找到的系统命令
    ARGS    #传递给命令的参数
    --target=${CMAKE_CURRENT_BINARY_DIR}/${myrc} ${GREC_XML}
    COMMENT     #执行时输出说明
        "compile ${GREC_XML}"
    VERBATIM     #起用不同平台特殊字符转译
    MAIN_DEPENDENCY ${GREC_XML}    #主依赖
    #DEPENDS        #依赖
    #{
        #dlg_login.glade
        #dlg_paramset.glade
        #MainWindow.glade
    #}
)
#message(STATUS ${CMAKE_CURRENT_BINARY_DIR}/${myrc})
#message(STATUS ${myrc})
# 3.添加自定义目标
add_custom_target(
    dummy-resource  #目标名,相当于make dummy-resource
    ALL     #包含入make all,就是构建项目时总会构建dummy-resource
    DEPENDS ${myrc}  #执行自定义的命令${myrc}
    )
#message(STATUS ${dummy-resource})
# 4.连接到库文件中,此为单独资源,不用了:
#target_link_libraries(WxGui ${CMAKE_CURRENT_BINARY_DIR}/${myrc})
#set_source_files_properties(
#    ${CMAKE_CURRENT_BINARY_DIR}/${myrc}
#    PROPERTIES GENERATED TRUE
#    )

# 5.加依赖关系,此为单独资源,不用了:
#sadd_dependencies(WxGui dummy-resource)
