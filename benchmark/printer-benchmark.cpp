#define CATCH_CONFIG_ENABLE_BENCHMARKING
#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <tableprinter/tableprinter.hpp>
#include <random>
#include <algorithm>
#include <array>

TEST_CASE( "Single float benchmark" , "[printer]" )
{
    using namespace tableprinter;

    std::array<float , 2048> floats;

    std::random_device rd;
    std::mt19937 gen { rd() };
    std::uniform_real_distribution<float> dist { 1.0 , 99999999.999999999 };

    std::generate_n(
        std::begin( floats ) ,
        std::size( floats ) ,
        [ &gen , &dist ](){
            return dist( gen );
        }
    );

    std::stringstream printer_output;

    printer p
    {
        { {} } ,
        printer_output
    };

    int i = 0;

    BENCHMARK( "Printer output bench" )
    {
        p.print( floats[ i++ % 2048 ] );

        return printer_output.tellp();
    };

    std::stringstream raw_output;

    BENCHMARK( "Raw output bench" )
    {
        return (raw_output << floats[i++ % 2048]).tellp();
    };
}

TEST_CASE( "3 fields benchmark" , "[printer]" )
{
    using namespace tableprinter;

    struct point
    {
        float x;
        int   y;
        std::string str;
    };

    std::array<point , 2048> points;

    std::random_device rd;
    std::mt19937 gen { rd() };
    std::uniform_real_distribution<float> dist { 1.0 , 99999999.999999999 };

    std::generate_n(
        std::begin( points ) ,
        std::size( points ) ,
        [ &gen , &dist ](){
            static std::string characters = "0123456789abcdefghijklmnopqrstuvwxyz";

            std::random_device rd;
            unsigned seed;

            if ( rd.entropy() != 0 )
                seed = rd();
            else
                seed = static_cast<unsigned> ( 
                    std::chrono::steady_clock::now().time_since_epoch()
                                                    .count()
                );

            std::mt19937 generator { seed };

            std::shuffle( std::begin( characters ) ,
                          std::end( characters ) ,
                          generator );

            return point 
                   {
                       dist( gen ) ,
                       int ( dist( gen ) ) ,
                       characters.substr( 0 , 8 )
                   };
        }
    );

    std::stringstream printer_output;

    printer p
    {
        {
            { name { "x" } , width { 8 } , fixed {} , precision { 3 } } ,
            { name { "y" } , width { 6 } , fixed {} , precision { 2 } } ,
            { name { "x" } , width { 16 } }
        } ,
        printer_output
    };

    int i = 0;

    BENCHMARK( "Printer output bench" )
    {
        const point& po = points[ i++ % 2048 ];
        p.print( po.x , po.y , po.str );

        return printer_output.tellp();
    };

    std::stringstream raw_output;

    BENCHMARK( "Raw output bench" )
    {
        const point& po = points[ i++ % 2048 ];

        raw_output << std::setw( 8 )  << std::fixed << std::setprecision( 3 ) << po.x
                   << std::setw( 6 )  << std::setprecision( 3 ) << po.y
                   << std::setw( 16 ) << po.str;

        return raw_output.tellp();
    };
}