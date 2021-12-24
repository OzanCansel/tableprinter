#include <iostream>
#include <random>
#include <tableprinter/tableprinter.hpp>

std::vector<std::string> shuffle( const std::vector<std::string>& colors )
{
    static std::default_random_engine rd;

    std::vector<std::string> shuffled { colors };

    std::shuffle(
        std::begin( shuffled ) ,
        std::end( shuffled ) ,
        rd
    );

    return shuffled;
}

int main()
{
    using namespace tableprinter;

    printer p
    {
        {
            { name { "iter" }    , width { 4 } } ,
            { name { "color-1" } , width { 8 } } ,
            { name { "color-2" } , width { 8 } } ,
            { name { "color-3" } , width { 8 } } ,
            { name { "color-4" } , width { 8 } } ,
            { name { "color-5" } , width { 8 } }
        } ,
        std::cout
    };

    std::string color_array[] = {
        "red" , "green" , "blue" , "yellow" , "purple"
    };

    std::vector<std::string> colors  {
        std::begin( color_array ) ,
        std::end( color_array )
    };

    int idx {};

    p.sanity_check()
     .print_headers()
     .print( ++idx , sequence( color_array ) )
     .print( ++idx , sequence( shuffle( colors = shuffle( colors ) ) ) )
     .print( ++idx , sequence( colors ) )
     .print( ++idx , sequence( shuffle( colors ) ) )
     .print( ++idx , sequence( shuffle( colors ) ) )
     .print( ++idx , "-_-_-" , sequence( std::begin( colors ) , std::begin( colors ) + 2 ) )
     .print( ++idx , sequence( shuffle( colors ) ) )
     .print( ++idx , sequence( shuffle( colors ) ) )
     .flush();
}