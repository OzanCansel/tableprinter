add_executable( printer-benchmark printer-benchmark.cpp catch.hpp )

target_link_libraries( printer-benchmark PRIVATE tableprinter::tableprinter )

target_include_directories( printer-benchmark PRIVATE tableprinter::tableprinter )