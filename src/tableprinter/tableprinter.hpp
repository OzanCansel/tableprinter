#pragma once

#include <vector>
#include <string>
#include <variant>
#include <functional>
#include <iomanip>
#include <ostream>
#include <tuple>
#include <sstream>

namespace tableprinter
{

struct name
{
    explicit name( ::std::string_view val ) : value { val }
    {}

    ::std::string value;

    operator ::std::string() const noexcept { return value; }
};

struct width
{
    explicit width( int val ) : value { val }
    {}

    int value;

    operator int() const noexcept { return value; }
};

struct fill
{
    explicit fill( char val ) : value { val }
    {}

    char value;

    operator char() const noexcept { return value; }
};

struct precision
{
    explicit precision( int val ) : value { val } 
    {}

    int value;

    operator int() const noexcept { return value; }
};

struct default_precision
{};

struct fixed
{};

struct unfixed
{};

struct left
{};

struct right
{};

using option = ::std::variant<name ,
                              width ,
                              precision ,
                              fill ,
                              default_precision ,
                              fixed ,
                              unfixed ,
                              left ,
                              right>;

namespace detail
{

template<typename... Ts>
struct overloaded : Ts...
{
    using Ts::operator ()...;
};

template<typename... Ts> overloaded(Ts...) -> overloaded<Ts...>;

template<typename T>
struct is_option :  
       ::std::bool_constant<::std::is_same_v<T , name>              ||
                            ::std::is_same_v<T , width>             ||
                            ::std::is_same_v<T , precision>         ||
                            ::std::is_same_v<T , default_precision> ||
                            ::std::is_same_v<T , fill>              ||
                            ::std::is_same_v<T , fixed>             ||
                            ::std::is_same_v<T , unfixed>           ||
                            ::std::is_same_v<T , left>              ||
                            ::std::is_same_v<T , right>>
{};

template<typename T>
static constexpr bool is_option_v = is_option<T>::value;

template<typename F>
struct invocable_with_option :
       ::std::bool_constant<::std::is_invocable_v<F , name>              ||
                            ::std::is_invocable_v<F , width>             ||
                            ::std::is_invocable_v<F , precision>         ||
                            ::std::is_invocable_v<F , default_precision> ||
                            ::std::is_invocable_v<F , fill>              ||
                            ::std::is_invocable_v<F , fixed>             ||
                            ::std::is_invocable_v<F , unfixed>           ||
                            ::std::is_invocable_v<F , left>              ||
                            ::std::is_invocable_v<F , right>>
{};

template<typename F>
static constexpr bool invocable_with_option_v = invocable_with_option<F>::value;

template<typename T, typename = ::std::void_t<>>
struct has_value_field :
       ::std::false_type
{};

template<typename T>
struct has_value_field<T, ::std::void_t<decltype( ::std::declval<T>().value )>> :
       ::std::true_type
{};

template<typename T>
static constexpr bool has_value_field_v = has_value_field<T>::value;

template<typename F>
inline void run( F&& val , const option& opt )
{
    static_assert(
        detail::invocable_with_option_v<F> ,
        "F must be invocable with an option."
    );

    ::std::visit(
        detail::overloaded {
            []( auto&& ) {} ,
            ::std::forward<F>( val )
        } ,
        opt
    );
}

template<typename F>
inline void run( F&& val , const ::std::vector<option>& options )
{
    for ( const option& opt : options )
        run( ::std::forward<F>( val ) , opt );
}

template<typename Opt>
inline ::std::vector<option> filter_opts( const ::std::vector<option>& options )
{
    static_assert( is_option_v<Opt> , "T must be an option." );

    ::std::vector<option> filtered;

    for ( const option& opt : options )
    {
        ::std::visit(
            detail::overloaded {
                []( auto&& ) {} ,
                [ &filtered ] ( const Opt& opt )
                {
                    filtered.push_back( opt );
                }
            } ,
            opt
        );
    }

    return filtered;
}

inline ::std::string concat( const ::std::vector<option>& options )
{
    ::std::stringstream ss;

    ss << "[ ";

    for ( const option& opt : options )
    {
        ::std::visit(
            [ &ss ]( const auto& opt )
            {
                if constexpr ( has_value_field_v<decltype(opt)> )
                {
                    ss << '\'' << opt.value << "' ";
                }
            } ,
            opt
        );
    }

    ss << "]";

    return ss.str();
};

}

struct column
{
    column( ::std::initializer_list<option> opts )
    {
        ::std::move(
            ::std::begin ( opts ) ,
            ::std::end( opts ) ,
            ::std::back_inserter( options )
        );
    }

    ::std::vector<option> options;
};

class printer
{
public:

    using osref = ::std::reference_wrapper<::std::ostream>;

    printer(
        ::std::vector<column> columns ,
        ::std::vector<osref>  streams
    )
        :   m_columns { ::std::move( columns ) }
        ,   m_streams { ::std::move( streams ) }
    {   }

    printer(
        ::std::vector<column> columns ,
        ::std::ostream& stream
    )
        :   printer
            {
                ::std::move( columns ) , ::std::vector<osref>{ stream }
            }
    {   }


    template<typename... Ts>
    inline printer& print( const ::std::tuple<Ts...>& values );

    template<typename... Ts>
    inline printer& print( const Ts&... );
    inline printer& echo( ::std::string_view );
    inline printer& print_headers();
    inline const printer& sanity_check() const;
    inline printer& sanity_check();
    inline printer& flush();
    inline const ::std::vector<osref>& streams() const noexcept;

private:

    template<::std::size_t Idx , typename H , typename... T>
    inline void print_column( ::std::ostream& , const H& , const T&... );

    template<typename... Ts , ::std::size_t... Idx>
    inline void print_tuple( const ::std::tuple<Ts...>& , ::std::index_sequence<Idx...> );

    template<typename T>
    inline void throw_if_duplicate_opt( const column& , ::std::string_view error ) const;

    inline void throw_if_both_left_and_right_opt( const column& ) const;
    inline void throw_if_both_precision_and_default_precision_opt( const column& ) const;
    inline void throw_if_both_fixed_and_unfixed_opt( const column& ) const;

    ::std::vector<::std::reference_wrapper<::std::ostream>> m_streams;
    ::std::vector<column> m_columns;
};

}

template<typename... Ts>
tableprinter::printer& tableprinter::printer::print( const Ts&... params )
{
    static_assert( sizeof...( Ts ) , "There must be arguments to print." );

    if ( auto count = sizeof...( params ) != ::std::size( m_columns ) )
        throw ::std::invalid_argument {
            "There are " +
            ::std::to_string( ::std::size( m_columns ) ) +
            " columns but given " +
            ::std::to_string( count ) +
            " arguments."
        };

    for ( ::std::ostream& stream : m_streams )
    {
        print_column<0 , Ts...>( stream , params... );

        stream << "\n";
    }

    return *this;
}

template<typename... Ts>
tableprinter::printer& tableprinter::printer::print( const ::std::tuple<Ts...>& values )
{
    if ( auto count = sizeof...( Ts ) != ::std::size( m_columns ) )
        throw ::std::invalid_argument {
            "There are " +
            ::std::to_string( ::std::size( m_columns ) ) +
            " columns but given " +
            ::std::to_string( count ) +
            " arguments."
        };

    print_tuple( values , ::std::make_index_sequence<sizeof...( Ts )>() );

    return *this;
}

tableprinter::printer& tableprinter::printer::echo( ::std::string_view str )
{
    for ( ::std::ostream& stream : m_streams )
        stream << str << "\n";

    return *this;
}

tableprinter::printer& tableprinter::printer::print_headers()
{
    for ( ::std::ostream& stream : m_streams )
    {
        for ( const auto& column : m_columns )
        {
            detail::run(
                detail::overloaded {
                    [ &stream ]( const width& w )
                    {
                        stream << ::std::setw( w );
                    } ,
                    [ &stream]( const left& )
                    {
                        stream << ::std::left;
                    } ,
                    [ &stream ]( const right& )
                    {
                        stream << ::std::right;
                    }
                } ,
                column.options
            );

            bool printed {};

            detail::run(
                [ &stream , &printed ]( const name& n )
                {
                    printed = true;
                    stream << n.value;
                } ,
                column.options
            );

            if ( !printed )
                stream << "";
        }

        stream << "\n";
    }

    return *this;
}

const tableprinter::printer& tableprinter::printer::sanity_check() const
{
    for ( const column& column : m_columns )
    {
        throw_if_duplicate_opt<name>( column , "Multiple 'name' options are not sane." );
        throw_if_duplicate_opt<width>( column , "Multiple 'width' options are not sane.");
        throw_if_duplicate_opt<precision>( column , "Multiple 'precision' options are not sane." );
        throw_if_duplicate_opt<fill>( column , "Multiple 'fill' options are not sane." );
        throw_if_both_left_and_right_opt( column );
        throw_if_both_precision_and_default_precision_opt( column );
        throw_if_both_fixed_and_unfixed_opt( column );
    }

    return *this;
}

tableprinter::printer& tableprinter::printer::sanity_check()
{
    const auto& self { *this };

    self.sanity_check();

    return *this;
}

tableprinter::printer& tableprinter::printer::flush()
{
    for ( ::std::ostream& os : m_streams )
        os.flush();

    return *this;
}

template<::std::size_t Idx , typename H , typename... T>
void tableprinter::printer::print_column( ::std::ostream& os , const H& val , const T&... rest )
{
    const auto& options { m_columns[ Idx ].options };

    detail::run(
        detail::overloaded {
            [ &os ]( const width& w )
            {
                os << ::std::setw( w );
            } ,
            [ &os ]( const fill& f )
            {
                os << ::std::setfill( f.value );
            } ,
            [ &os ]( const precision& p )
            {
                os << ::std::setprecision( p );
            } ,
            [ &os ]( const default_precision& )
            {
                os << ::std::setprecision( 6 );
            } ,
            [ &os ]( const fixed& f )
            {
                os << ::std::fixed;
            } ,
            [ &os ]( const unfixed& )
            {
                os.unsetf( ::std::ios_base::fixed );
            } ,
            [ &os ]( const left& )
            {
                os << ::std::left;
            } ,
            [ &os ]( const right& )
            {
                os << ::std::right;
            }
        } ,
        options
    );

    os << val;

    if constexpr( sizeof...( T ) != 0 )
        print_column<Idx + 1 , T...>( os , rest... );
}

template<typename... Ts , ::std::size_t... Idx>
void tableprinter::printer::print_tuple( const ::std::tuple<Ts...>& values , ::std::index_sequence<Idx...> )
{
    print( ::std::get<Idx>( values )... );
}

template<typename T>
void tableprinter::printer::throw_if_duplicate_opt( const column& col , ::std::string_view error ) const
{
    auto opts = detail::filter_opts<T>( col.options );

    if (  ::std::size( opts ) > 1 )
    {
        ::std::string concatenated;

        throw ::std::logic_error {
            ::std::string { error } +
            " " +
            detail::concat( opts )
        };
    }
}

void tableprinter::printer::throw_if_both_left_and_right_opt( const column& col ) const
{
    auto lefts  = detail::filter_opts<left>( col.options );
    auto rights = detail::filter_opts<right>( col.options );

    if ( !::std::empty( lefts ) && !::std::empty( rights ) )
    {
        throw ::std::logic_error {
            "Specifying both 'left' and 'right' options are not sane."
        };
    }
}

void tableprinter::printer::throw_if_both_precision_and_default_precision_opt( const column& col ) const
{
    auto precisions    = detail::filter_opts<precision>( col.options );
    auto default_precs = detail::filter_opts<default_precision>( col.options );

    if ( !::std::empty( precisions ) && !::std::empty( default_precs ) )
    {
        throw std::logic_error {
            "Specifying both 'precision' and 'default_precision' options are not sane."
        };
    }
}

void tableprinter::printer::throw_if_both_fixed_and_unfixed_opt( const column& col ) const
{
    auto fixeds   = detail::filter_opts<fixed>( col.options );
    auto unfixeds = detail::filter_opts<unfixed>( col.options );

    if ( !::std::empty( fixeds ) && !::std::empty( unfixeds ) )
    {
        throw ::std::logic_error {
            "Specifying both 'fixed' and 'unfixed' options are not sane."
        };
    }
}
