project(isibus)

find_package(Qt4 REQUIRED)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include/ ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/base/ ${CMAKE_CURRENT_SOURCE_DIR}/ui/)

set(easybus_SRCS
	main.cpp
)

set(controlcenter_SRCS
	ctrl_center/cc_main.cpp
	ctrl_center/roadmap.cpp
	ctrl_center/xmlParser.cpp
)

set(uis_SRCS
	ui/main.ui
)

QT4_WRAP_UI(
	easybus_UIS_H ${uis_SRCS}
)

qt4_automoc(${controlcenter_SRCS} ${easybus_SRCS} )

add_executable(easybus ${controlcenter_SRCS} ${uis_SRCS} ${easybus_SRCS})
target_link_libraries(easybus ${QT_LIBRARIES})
