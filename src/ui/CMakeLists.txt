project(isibus_interface)

cmake_minimum_required(VERSION 2.4.0)
find_package(Qt4 REQUIRED)

SET( QT_USE_QT3SUPPORT TRUE )   
SET( QT_USE_QTXML TRUE )

include(${QT_USE_FILE})
add_definitions (${QT_DEFINITIONS})
add_definitions ( -g )

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/pics
)

set(isibus_sources
	main.cpp
	Isibus.cpp
)
set(isibus_ui
	Isibus.ui
)
set(isibus_moc_classes
	Isibus.h
	sprites.h
)

set(isibus)

QT4_WRAP_UI(isibus_ui_res
		${isibus_ui_headers}
		${isibus_ui_sources}
		${isibus_ui}
		)

QT4_WRAP_CPP(isibus_moc_res
		${isibus_moc_files}
		${isibus_moc_classes}
		)

qt4_automoc(${isibus_sources})

add_executable(isibus_exec ${isibus_ui_res} ${isibus_sources} ${isibus_moc_classes} ${isibus_moc_files})

target_link_libraries(isibus_exec ${QT_LIBRARIES})
