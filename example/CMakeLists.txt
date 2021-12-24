add_executable( print-instructions-benchmark print-instructions-benchmark.cpp )
target_link_libraries( print-instructions-benchmark tableprinter::tableprinter )

add_executable( print-scores print-scores.cpp )
target_link_libraries( print-scores tableprinter::tableprinter )

add_executable( read-scores read-scores.cpp )
target_link_libraries( read-scores tableprinter::tableprinter )

add_executable( print-colors print-colors.cpp )
target_link_libraries( print-colors tableprinter::tableprinter )