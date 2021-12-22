#define CATCH_CONFIG_MAIN
#include"catch.hpp"
#include <sstream>
#include <tableprinter/tableprinter.hpp>

TEST_CASE( "Hex" , "[printer]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { hex {} }
        } ,
        ss
    };

    p.print( 15 );
    REQUIRE( ss.str() == "f\n" );
}

TEST_CASE( "Decimal" , "[printer]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    std::cout << std::hex;

    printer p
    {
        {
            { decimal {} }
        } ,
        ss
    };

    p.print( 15 );
    REQUIRE( ss.str() == "15\n" );
}

TEST_CASE( "Octal" , "[printer]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    std::cout << std::hex;

    printer p
    {
        {
            { octal {} }
        } ,
        ss
    };

    p.print( 15 );
    REQUIRE( ss.str() == "17\n" );
}

TEST_CASE( "Retrieve streams" , "[streams]" )
{
    using namespace tableprinter;

    std::stringstream ss1 , ss2 , ss3;

    printer p
    {
        {
            { name { "col-1" } , default_precision {} } ,
            { name { "col-2" } , default_precision {} } ,
            { name { "col-3" } , default_precision {} }
        } ,
        { ss1 , ss2 , ss3 }
    };

    REQUIRE( std::size( p.streams() ) == 3 );
}

TEST_CASE( "Remove streams" , "[remove_streams]" )
{
    using namespace tableprinter;

    std::stringstream ss1 , ss2 , ss3;

    printer p
    {
        {
            { name { "col-1" } , default_precision {} } ,
            { name { "col-2" } , default_precision {} } ,
            { name { "col-3" } , default_precision {} }
        } ,
        { ss1 , ss2 , ss3 }
    };

    p.remove_streams( ss1 , ss2 , ss3 );

    p.print( 1 , 2 , 3  );

    REQUIRE( std::empty( ss1.str() ) );
    REQUIRE( std::empty( ss2.str() ) );
}

TEST_CASE( "Add streams" , "[add_streams]" )
{
    using namespace tableprinter;

    printer p
    {
        {
            { name { "col-1" } , default_precision {} } ,
            { name { "col-2" } , default_precision {} } ,
            { name { "col-3" } , default_precision {} }
        }
    };

    std::stringstream ss;

    p.print( 1 , 2 , 3 );
    p.add_streams( ss );
    p.print( 4 , 5 , 6 );

    REQUIRE( ss.str() == "456\n" );

    p.add_streams( ss , ss );
    p.print( 7 , 8 , 9 );

    REQUIRE( ss.str() == "456\n789\n789\n789\n" );
}

TEST_CASE( "Should be constructed without outputs" )
{
    using namespace tableprinter;

    printer p
    {
        {
            { name { "col-1" } , default_precision {} } ,
            { name { "col-2" } , default_precision {} } ,
            { name { "col-3" } , default_precision {} }
        }
    };

    p.print( 1 , 2 , 3 );
}

TEST_CASE( "Print tuple elements" , "[print]" )
{
    using namespace tableprinter;

    std::stringstream ss;
    ss << std::setprecision( 2 );

    printer p
    {
        {
            { name { "col-1" } , default_precision {} } ,
            { name { "col-2" } , default_precision {} } ,
            { name { "col-3" } , default_precision {} }
        } ,
        ss
    };

    p.print( std::make_tuple( 0.123456 , 0.654321 , 0.123654 ) );

    REQUIRE( ss.str() == "0.1234560.6543210.123654\n" );
}

TEST_CASE( "Take default_precision option into account while printing" , "[print]" )
{
    using namespace tableprinter;

    std::stringstream ss;
    ss << std::setprecision( 2 );

    printer p
    {
        {
            { name { "col-1" } , default_precision {} }
        } ,
        ss
    };

    p.print( 0.123456 );

    REQUIRE( ss.str() == "0.123456\n" );
}

TEST_CASE( "Take unfixed option into account while printing" , "[print]" )
{
    using namespace tableprinter;

    std::stringstream ss;
    ss << std::fixed;

    printer p
    {
        {
            { name { "col-1" } , unfixed {} }
        } ,
        ss
    };

    p.print( 0.0 );

    REQUIRE( ss.str() == "0\n" );
}

TEST_CASE( "Take fill option into account while printing" , "[print]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { name { "col-1" } , width { 5 } , fill { '_' } }
        } ,
        ss
    };

    p.print( "one" );

    REQUIRE( ss.str() == "__one\n" );
}

TEST_CASE( "Take width option into account while printing" , "[print]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { name { "col-1" } , width { 5 } }
        } ,
        ss
    };

    p.print( "4.5" );

    REQUIRE( ss.str() == "  4.5\n" );
}

TEST_CASE( "Take left option into account while printing" , "[print]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { name { "col-1" } , left {} , width { 5 } }
        } ,
        ss
    };

    p.print( "4.5" );

    REQUIRE( ss.str() == "4.5  \n" );
}

TEST_CASE( "Take right option into account while printing" , "[print]" )
{
    using namespace tableprinter;

    std::stringstream ss;
    ss << std::left;

    printer p
    {
        {
            { name { "col-1" } , right {} , width { 5 } }
        } ,
        ss
    };

    p.print( "4.5" );

    REQUIRE( ss.str() == "  4.5\n" );
}

TEST_CASE( "Take precision option into account while printing" , "[print]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { name { "col-1" } , precision { 11 } }
        } ,
        ss
    };

    p.print( 5.3434532341 );

    REQUIRE( ss.str() == "5.3434532341\n" );
}

TEST_CASE( "If there are less elements in tuple than columns, should throw exception" , "[print]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { name { "col-1" } } ,
            { name { "col-2" } }
        } ,
        ss
    };

    REQUIRE_THROWS_AS( p.print( std::make_tuple( 1 ) ) , std::logic_error );
}

TEST_CASE( "If there are less arguments than columns, should throw exception" , "[print]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { name { "col-1" } } ,
            { name { "col-2" } }
        } ,
        ss
    };

    REQUIRE_THROWS_AS( p.print( 5 ) , std::logic_error );
}

TEST_CASE( "Print header names" , "[print_headers]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { name { "col-1" } } ,
            { name { "col-2" } }
        } ,
        ss
    };

    p.print_headers();

    REQUIRE( ss.str() == "col-1col-2\n" );
}

TEST_CASE( "echo" , "[echo]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {} ,
        ss
    };

    p.echo( "Hello" );

    REQUIRE( ss.str() == "Hello\n" );
}

TEST_CASE( "Ignore empty headers while printing headers" , "[print_headers]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { } ,
            { name { "col-2" } }
        } ,
        ss
    };

    p.print_headers();

    REQUIRE( ss.str() == "col-2\n" );
}

TEST_CASE( "Take into account width option while printing headers" , "[print_headers]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { width { 8 } } ,
            { name { "col-2" } , width { 8 } }
        } ,
        ss
    };

    p.print_headers();

    REQUIRE( ss.str() == "           col-2\n" );
}

TEST_CASE( "Take into account right option while printing headers" , "[print_headers]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    ss << std::left;

    printer p
    {
        {
            { name { "col-2" } , right { } , width { 8 } }
        } ,
        ss
    };

    p.print_headers();

    REQUIRE( ss.str() == "   col-2\n" );
}

TEST_CASE( "Take into account left option while printing headers" , "[print_headers]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { name { "col-2" } , left { } , width { 8 } }
        } ,
        ss
    };

    p.print_headers();

    REQUIRE( ss.str() == "col-2   \n" );
}

TEST_CASE( "If there are two 'name' options, should throw exception" , "[sanity_check]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { name { "should" } , name { "throw" } }
        } ,
        ss
    };

    REQUIRE_THROWS_AS( p.sanity_check() , tableprinter::duplicate_option );
}

TEST_CASE( "If there are two 'width' options, should throw exception" , "[sanity_check]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { width { 5 } , width { 3 } }
        } ,
        ss
    };

    REQUIRE_THROWS_AS( p.sanity_check() , tableprinter::duplicate_option );
}

TEST_CASE( "If there are two 'precision' options, should throw exception" , "[sanity_check]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { precision { 5 } , precision { 3 } }
        } ,
        ss
    };

    REQUIRE_THROWS_AS( p.sanity_check() , tableprinter::duplicate_option );
}

TEST_CASE( "If there are two 'fill' options, should throw exception" , "[sanity_check]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { fill { 'a' } , fill { 'b' } }
        } ,
        ss
    };

    REQUIRE_THROWS_AS( p.sanity_check() , tableprinter::duplicate_option );
}

TEST_CASE( "If both 'left' and 'right' options are specified, should throw exception" , "[sanity_check]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { left { } , right { } }
        } ,
        ss
    };

    REQUIRE_THROWS_AS( p.sanity_check() , tableprinter::both_left_and_right_option );
}

TEST_CASE( "If both 'fixed' and 'unfixed' options are specified, should throw exception" , "[sanity_check]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { fixed {} , unfixed { } }
        } ,
        ss
    };

    REQUIRE_THROWS_AS( p.sanity_check() , tableprinter::both_fixed_and_unfixed );
}

TEST_CASE( "If both 'precision' and 'default_precision' options are specified, should throw exception" , "[sanity_check]" )
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { precision { 5 } , default_precision { } }
        } ,
        ss
    };

    REQUIRE_THROWS_AS( p.sanity_check() , tableprinter::both_precision_and_default_precision );
}