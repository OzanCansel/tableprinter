add_executable( printer-test printer-test.cpp catch.hpp )

target_link_libraries( printer-test PRIVATE tableprinter )

target_include_directories( printer-test PRIVATE tableprinter )

add_test( NAME printer-test COMMAND printer-test -s )