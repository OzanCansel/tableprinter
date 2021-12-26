#include <iostream>
#include <random>
#include <tableprinter/tableprinter.hpp>

std::vector<std::string> shuffle( const std::vector<std::string>& colors )
{
    static std::default_random_engine rd;

    std::vector<std::string> shuffled { colors };

    shuffle(
        begin( shuffled ) ,
        end( shuffled ) ,
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

    std::string colors_array[] = {
        "red" , "green" , "blue" , "yellow" , "purple"
    };

    std::vector<std::string> colors  {
        begin( colors_array ) ,
        end( colors_array )
    };

    int idx {};

    p.sanity_check()
     .print_headers()
     .print( ++idx , sequence( colors_array ) )
     .print( ++idx , sequence( shuffle( colors = shuffle( colors ) ) ) )
     .print( ++idx , sequence( colors ) )
     .print( ++idx , sequence( shuffle( colors ) ) )
     .print( ++idx , sequence( shuffle( colors ) ) )
     .print( ++idx , "-_-_-" , sequence( begin( colors ) , begin( colors ) + 2 ) )
     .print( ++idx , sequence( shuffle( colors ) ) )
     .print( ++idx , sequence( shuffle( colors ) ) )
     .flush();
}