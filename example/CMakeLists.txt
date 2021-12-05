add_executable( print-instructions-benchmark print-instructions-benchmark.cpp )
add_executable( print-scores print-scores.cpp )

target_link_libraries( print-instructions-benchmark tableprinter::tableprinter )
target_link_libraries( print-scores tableprinter::tableprinter )