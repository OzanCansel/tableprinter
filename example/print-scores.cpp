#include <iostream>
#include <sstream>
#include <tableprinter/tableprinter.hpp>

int main()
{
    using namespace tableprinter;

    std::stringstream ss;

    printer p
    {
        {
            { name { "id" }      , width { 4 }  } ,
            { name { "name" }    , width { 10 } } ,
            { name { "surname" } , width { 10 } } ,
            { name { "rank" }    , width { 6 }  } ,
            { name { "score" }   , width { 7 } , fixed { } , precision { 2 } }
        } ,
        { std::cout , ss }
    };

    p.sanity_check()
     .echo( "The scores are listed below with their ranks :" )
     .print_headers()
     .print( 1 , "Lucy"   , "Ballmer"  , 2 , 94.13 )
     .print( 2 , "Roger"  , "Bacon"    , 5 , 77.13 )
     .print( 3 , "Anna"   , "Smith"    , 3 , 87.13 )
     .print( 4 , "Robert" , "Schwartz" , 1 , 98.34 )
     .print( 5 , "Robert" , "Brown"    , 4 , 84.34 )
     .print( std::make_tuple( 6 , "David" , "Timothy" , 6 , 71.34 ) );

    /*
       # Run the command if you want to see maximum score
       ./print-scores | tail -n 5 | awk '{ printf "%d %d %s %s %f\n" , $4 , $1 , $2 , $3 , $5 }' | sort | head -n 1 | awk '{ print $5 }'
       # or lowest score
       ./print-scores | tail -n 5 | awk '{ printf "%d %d %s %s %f\n" , $4 , $1 , $2 , $3 , $5 }' | sort | tac | head -n 1 | awk '{ print $5 }'
    */
}