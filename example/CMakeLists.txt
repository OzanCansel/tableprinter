if ( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
	add_executable( print-instructions-benchmark print-instructions-benchmark.cpp )
	target_link_libraries( print-instructions-benchmark tableprinter::tableprinter )
endif()

add_executable( print-scores print-scores.cpp )
target_link_libraries( print-scores tableprinter::tableprinter )
