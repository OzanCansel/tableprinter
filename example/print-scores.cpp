#include <iostream>
#include <fstream>
#include <tableprinter/tableprinter.hpp>

int main()
{
    using namespace tableprinter;

    std::ofstream scores_f { "scores.txt" };

    printer p
    {
        {
            { name { "id" }      , width { 4 }  , hex {} } ,
            { name { "name" }    , width { 8 } } ,
            { name { "surname" } , width { 10 } } ,
            { name { "rank" }    , width { 6 } , decimal {} } ,
            { name { "score" }   , width { 7 } , fixed { } , precision { 2 } }
        } ,
        { std::cout , scores_f }
    };

    p.sanity_check()
     .echo( "The scores are listed below with their ranks :" )
     .print_headers()
     .print( 5000 , "Lucy"   , "Ballmer"  , 2 , 94.13 )
     .print( 5100 , "Roger"  , "Bacon"    , 5 , 77.13 )
     .print( 5200 , "Anna"   , "Smith"    , 3 , 87.13 )
     .print( 5300 , "Robert" , "Schwartz" , 1 , 98.34 )
     .print( 5400 , "Robert" , "Brown"    , 4 , 84.34 )
     .print( std::make_tuple( 5500 , "David" , "Timothy" , 6 , 71.34 ) );

    /*
       # Run the command if you want to see maximum score
       ./print-scores | tail -n 5 | awk '{ printf "%d %d %s %s %f\n" , $4 , $1 , $2 , $3 , $5 }' | sort | head -n 1 | awk '{ print $5 }'
       # or lowest score
       ./print-scores | tail -n 5 | awk '{ printf "%d %d %s %s %f\n" , $4 , $1 , $2 , $3 , $5 }' | sort | tac | head -n 1 | awk '{ print $5 }'
    */
}