#CMake最小版本3.15
cmake_minimum_required(VERSION 3.15)

#定义主项目中为mycpp 程序语言c++
project(mycpp LANGUAGES CXX)

#设置c++编译器版本为11
set(CMAKE_CXX_STANDARD 11)

#设置c++编译器gcc,g++在系统中必需是安装的
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#自动搜索当前目录的下的所有.c,.h,.cc,.cpp,.hpp文件名并存入ccfiles
aux_source_directory(. ccfiles)
aux_source_directory(./dnn dnncs)
#生名生成的执行文件为mycpp,并编译当前目录下的所有程序源码文件
#引用之前生成的文件列表，引用格式${ccfiles}
add_executable(mycpp ${ccfiles} ${dnncs})

#增加一个WxGui的子目录，子目录为Gtkmm,glade可视化界面生成的动态库
add_subdirectory(WxGui)

#把WxGui目录添加到头文件搜索目录表中
include_directories("WxGui" "dnn")

#定义生成的执行文件mycpp连接到界面动态库WxGui
target_link_libraries(mycpp WxGui)

#搜索系统中的包管理器pkg-config必需是安装的，
#没有安装命令sudo apt install pkg-config
find_package(PkgConfig REQUIRED)

#如果系统存在pkg-config包管理器，搜索gtkmm-3.0,
#没有安装命令sudo apt install libgtkmm-3.0-dev
pkg_search_module(PKG_GTKMM REQUIRED gtkmm-3.0)

#显示是否找到gtkmm开发库
#message(STATUS PKG_GTKMM_FOUND:${PKG_GTKMM_FOUND})

#显示gtkmm头文件列表
#message(STATUS ${PKG_GTKMM_INCLUDE_DIRS})

#显示gtkmm库目录
#message(STATUS ${PKG_GTKPKG_GTKMM_LIBRARY_DIRS})

#显示gtkmm库文件
#message(STATUS ${PKG_GTKMM_LIBRARIES})

#把gtkmm头文件目录列表加入搜索目录中
include_directories(${PKG_GTKMM_INCLUDE_DIRS})

#把gtkmm的库文件加入连接库文件表中
link_directories(${PKG_GTK_LIBRARIES})

#把gtkmm库文件连接到执行文件中mycpp
target_link_libraries(mycpp ${PKG_GTKMM_LIBRARIES})

#拷贝glade可视化设计的界面文件.glade从源码文件目录到生成项目目录中
#file(COPY ./WxGui/MainWindow.glade DESTINATION ./WxGui)
#file(COPY ./WxGui/dlg_paramset.glade DESTINATION ./WxGui)
#拷贝分类器配置文件从源码文件目录到生成项目目录中
file(COPY ./config/haarcascade_eye.xml DESTINATION ./config)
file(COPY ./config/haarcascade_frontalface_alt.xml DESTINATION ./config)
file(COPY ./config/haarcascade_mcs_mouth.xml DESTINATION ./config)

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
target_link_libraries(mycpp ${OpenCV_LIBRARIES})
