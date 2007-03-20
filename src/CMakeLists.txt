project(isibus)

add_subdirectory(ui)

cmake_minimum_required(VERSION 2.4.0)
find_package(Qt4 REQUIRED)

SET( QT_USE_QT3SUPPORT TRUE )   
SET( QT_USE_QTXML TRUE )

include(${QT_USE_FILE})

set(base_SRCS
	base/IvyApplication.cxx
	base/Ivy.cxx
	base/xmlParser.cpp
)

set(isibus_SRCS
	main.cpp
)

set(controlcenter_SRCS
	ctrl_center/cc_main.cpp
	ctrl_center/roadmap.cpp
)

set(uis_SRCS
)

set(rc_SRCS
)

set(moc_HDRS
)

add_definitions (${QT_DEFINITIONS})
add_definitions ( -g )

QT4_ADD_RESOURCES ( isibus_RC_SRCS ${rc_SRCS} )

QT4_WRAP_UI( isibus_UIS_H ${uis_SRCS} )

QT4_WRAP_CPP ( isibus_MOC_HDRS ${moc_HDRS})
 
include_directories (
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include/ 
	${CMAKE_CURRENT_SOURCE_DIR}/base/ 
	${CMAKE_CURRENT_SOURCE_DIR}/ui/
)

add_executable(isibus_cc ${base_SRCS} ${controlcenter_SRCS} ${isibus_RC_SRCS} ${isibus_SRCS} ${isibus_UIS_H} ${isibus_MOC_HDRS} )

target_link_libraries(isibus_cc ${QT_LIBRARIES} Ivy pcre)

install(TARGETS isibus_cc DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
install(FILES roadmap.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
