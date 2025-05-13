cmake_minimum_required(VERSION 3.15)

project("CS2 HealthShot Drop" VERSION 1.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

if (NOT DEFINED PLUGIN_NAME)
	set(PLUGIN_NAME "cs_healthshot_drop")
endif()

if (NOT DEFINED OUTPUT_NAME)
	set(OUTPUT_NAME "cs2_healthshot_drop")
endif()

if (NOT DEFINED LINKER_PARAM)
	set(LINKER_PARAM "/NOLOGO /LTCG /OPT:REF /OPT:ICF")
endif()

configure_file(	"${CMAKE_CURRENT_SOURCE_DIR}/metadata/version.in" 
				"${CMAKE_CURRENT_BINARY_DIR}/version.h"
				@ONLY)

set(METAMOD_CORE_PATH "${CMAKE_SOURCE_DIR}/../metamod/core")
set(SOURCES
			"${CMAKE_SOURCE_DIR}/utils/detour_manager.cpp"
			"${CMAKE_SOURCE_DIR}/${PLUGIN_NAME}.cpp"
			"${METAMOD_CORE_PATH}/metamod_oslink.cpp")
set(RESOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/metadata/version.rc")

add_library(${OUTPUT_NAME} SHARED ${SOURCES} ${RESOURCE_FILE})

target_include_directories(${OUTPUT_NAME} PRIVATE
	"${CMAKE_CURRENT_BINARY_DIR}"
	"${METAMOD_CORE_PATH}"
	"${METAMOD_CORE_PATH}/sourcehook"
	"${CMAKE_SOURCE_DIR}"
	"${CMAKE_SOURCE_DIR}/xbyak"
)

target_compile_definitions(${OUTPUT_NAME} PRIVATE 
	_CRT_SECURE_NO_WARNINGS
	META_IS_SOURCE2
	META_NO_HL2SDK
)

if (MSVC)
	target_compile_options(${OUTPUT_NAME} PRIVATE 
		"$<$<CONFIG:Release>:/O1>"
		"$<$<CONFIG:Release>:/GL>"
		/W4 
		/EHsc
	)
endif()

set_target_properties(${OUTPUT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

string(REPLACE " " ";" LINKER_OPTIONS "${LINKER_PARAM}")

if (MSVC)
	target_link_options(${OUTPUT_NAME} PRIVATE ${LINKER_OPTIONS})
endif()
