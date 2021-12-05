#define CATCH_CONFIG_MAIN
#include"catch.hpp"
#include <sstream>
#include <tableprinter/tableprinter.hpp>

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

    REQUIRE_THROWS_AS( p.sanity_check() , std::logic_error );
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

    REQUIRE_THROWS_AS( p.sanity_check() , std::logic_error );
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

    REQUIRE_THROWS_AS( p.sanity_check() , std::logic_error );
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

    REQUIRE_THROWS_AS( p.sanity_check() , std::logic_error );
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

    REQUIRE_THROWS_AS( p.sanity_check() , std::logic_error );
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

    REQUIRE_THROWS_AS( p.sanity_check() , std::logic_error );
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

    REQUIRE_THROWS_AS( p.sanity_check() , std::logic_error );
}