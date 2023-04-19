# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("fishAI")


set(CMAKE_AUTOMOC ON)	# 针对信号和槽 帮助我们处理，含有信号和槽的源文件
set(CMAKE_AUTOUIC ON)	# 针对UI文件
set(CMAKE_AUTORCC ON)	# 针对图片的打包
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# 将源代码添加到此项目的可执行文件。
add_executable (${PROJECT_NAME} WIN32
    "main.cpp"
    "MainWindow.h" 
    "MainWindow.cpp"
    "MainWindow.ui"
    "image.qrc"
    "http.h"
    "http.cpp"
	"database.h"
    "database.cpp"
)


find_package(Qt5 COMPONENTS Core Widgets Network Sql)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
        Qt::Sql
)



if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
endif()