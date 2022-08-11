#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <string>

struct entry
{
    int         id;
    std::string name;
    std::string surname;
    int         rank;
    float       score;
};

std::istream& operator>>( std::istream& is , entry& s );
void skip_echo_and_header( std::istream& is );
void print_max_score( const std::vector<entry>& );

int main()
{
    std::ifstream scores_f { "scores.txt" };

    if ( !scores_f )
    {
        std::cerr << "'scores.txt' could not be read.\n"
                     "Run 'print-scores' first."
                  << std::endl;

        return 1;
    }

    std::cout << "A demonstration how "
                 "table-like output is "
                 "convenient to be read"
              << std::endl;

    skip_echo_and_header( scores_f );

    std::istream_iterator<entry> scores_i_itr { scores_f };
    std::istream_iterator<entry> eof;

    std::vector<entry> scores { scores_i_itr , eof };

    print_max_score( scores );
}

std::istream& operator>>( std::istream& is , entry& s )
{
    return is >> s.id
              >> s.name
              >> s.surname
              >> s.rank
              >> s.score;
}

void skip_echo_and_header( std::istream& is )
{
    std::string skipped_line;
    std::getline( is , skipped_line );
    std::getline( is , skipped_line );
}

void print_max_score( const std::vector<entry>& scores )
{
    auto by_score = []( const entry& lhs , const entry& rhs ){
        return lhs.score < rhs.score;
    };

    auto winner = max_element(
        begin( scores ) ,
        end( scores ) ,
        by_score
    );

    std::cout << "Winner is "
              << winner->name
              << " "
              << winner->surname
              << " who has score "
              << winner->score
              << std::endl;
}