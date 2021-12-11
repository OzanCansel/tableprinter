/*
This source file is based on https://github.com/psoberoi/stepanov-conversations-course/blob/master/benchmarks/transformbench.cpp repo.

I just modified the printing part.
*/

#include <tableprinter/tableprinter.hpp>
#include <stdint.h>
#include <cstddef>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <numeric>
#include <cmath>
#include <list>
#include <vector>
#include <deque>
#include <random>

#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
  #define NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
  #define NOINLINE __declspec(noinline)
#endif

#define TotallyOrdered typename
#define Sequence typename
#define Number typename
#define BinaryOperation typename
#define RandomAccessIterator typename

#if __cplusplus <= 199711L

#include <time.h>

class timer {
private:
    clock_t start_time;
public:
    typedef double result_type;

    void start() {
        start_time = clock();
    }

    result_type stop() {
        return 1000000000. * ((clock() - start_time) / double(CLOCKS_PER_SEC));
    }
};

template<class ForwardIterator, class T>
void iota(ForwardIterator first, ForwardIterator last, T value)
{
    while(first != last) {
        *first++ = value;
        ++value;
    }
}

#else

#include <chrono>

class timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
public:
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double stop() {
        auto stop_time = std::chrono::high_resolution_clock::now();
        return double(std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count());
    }
};

using std::iota;

#endif

template <RandomAccessIterator I>
void random_iota(I first, I last) {
  std::iota(first, last, 0);

  std::random_device rd;
  std::mt19937 g(rd());

  std::shuffle(first, last , g);
}

template <typename N>
struct plus
{
  N operator() (const N& x, const N& y) { return  x + y; }
};

template <typename N>
struct multiplies
{
  N operator() (const N& x, const N& y) { return x * y; }
};

template <typename N>
struct divides
{
  N operator() (const N& x, const N& y) {
    if (y == N(0)) return x;
    return x / y;
  }
};

template <typename N>
struct fplus
{
  NOINLINE N operator() (const N& x, const N& y);
};

template <typename N>
N fplus<N>::operator() (const N& x, const N& y) {
  return x + y;
}

template <TotallyOrdered T>
struct minimum
{
  T operator()(const T& x, const T& y) {
    if (y < x) return y;
    return x;
  }
};

template <TotallyOrdered T>
struct norm
{
  T operator()(const T& x, const T& y) {
    return x * x + y * y;
  }
};

template <Number T>
struct conversion 
{
  T operator()(const T& x, const T& y) { return T(double(x) + double(y)); }
};

template <Number N>
struct square_root
{
  N operator()(N x, N y) {
    return N(std::sqrt(std::abs(x + y))); 
  }
};

template <Number N>
struct cube_root_pow
{
  N operator()(N x, N y) {
    return N(std::pow(std::abs(x + y), 1.0/3.0)); 
  }
};

template <Sequence S1, Sequence S2, Sequence S3, BinaryOperation Op>
double time_test(const S1& s1, const S2& s2, S3& s3, Op op, size_t n, size_t m) {
  typedef typename S1::iterator I;
  timer t;
  t.start();
  for (int i = 0; i < m; ++i)
    std::transform(s1.begin(), s1.end(), s2.begin(), s3.begin(), op);
  double time = t.stop()/(double(n) * m);

  return time;
}

std::string type_description(double) { return std::string("double"); }
std::string type_description(long double) { return std::string("long double"); }
std::string type_description(float) { return std::string("float"); }
std::string type_description(uint8_t) { return std::string("uint8_t"); }
std::string type_description(uint16_t) { return std::string("uint16_t"); }
std::string type_description(uint32_t) { return std::string("uint32_t"); }
std::string type_description(uint64_t) { return std::string("uint64_t"); }
std::string type_description(int8_t) { return std::string("int8_t"); }
std::string type_description(int16_t) { return std::string("int16_t"); }
std::string type_description(int32_t) { return std::string("int32_t"); }
std::string type_description(int64_t) { return std::string("int64_t"); }

template <typename T, typename Alloc>
std::string type_description(const std::vector<T, Alloc>&) { return std::string("vector<") + type_description(T()) + std::string(">"); }

template <typename T, typename Alloc>
std::string type_description(const std::deque<T, Alloc>&) { return std::string("deque<") + type_description(T()) + std::string(">"); }

template <typename T, typename Alloc>
std::string type_description(const std::list<T, Alloc>&) { return std::string("list<") + type_description(T()) + std::string(">"); }

void print_info(size_t n, size_t m) {
  time_t now = time(0);

  std::cout << "Applying operation " 
	    << n * m << " times" 
	    << " (sequence length " << n << " by " << m << " iterations)"
	    << std::endl;
}

template <Number T, template <Number N, class Alloc = std::allocator<N> > class Seq>
struct test_instructions 
{
  tableprinter::printer& p;
  void operator()(size_t n, size_t m) {
    std::vector<T> data(n);
    random_iota(data.begin(), data.end());
    Seq<T> v1(data.begin(), data.end());
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(data.begin(), data.end(), g);
    Seq<T> v2(data.begin(), data.end());
    Seq<T> v3(n);
    
    p.print(
        type_description(v1) ,
        time_test(v1, v2, v3, plus<T>(), n, m) ,
        time_test(v1, v2, v3, fplus<T>(), n, m) ,
        time_test(v1, v2, v3, multiplies<T>(), n, m) ,
        time_test(v1, v2, v3, norm<T>(), n, m) ,
        time_test(v1, v2, v3, conversion<T>(), n, m) ,
        time_test(v1, v2, v3, divides<T>(), n, m) ,
        time_test(v1, v2, v3, square_root<T>(), n, m) ,
        time_test(v1, v2, v3, cube_root_pow<T>(), n, m)
    );
    
    p.flush();
  }

};

template <template <Number N, class Alloc = std::allocator<N> > class Seq>
struct test_all_types
{
  tableprinter::printer& p;

  void operator() (size_t n, size_t m) {
    test_instructions<int8_t, Seq>{ p }(n, m); 
    test_instructions<int16_t, Seq>{ p }(n, m); 
    test_instructions<int32_t, Seq>{ p }(n, m); 
    test_instructions<int64_t, Seq>{ p }(n, m); 
    test_instructions<float, Seq>{ p }(n, m); 
    test_instructions<double, Seq>{ p }(n, m);
  }
}; 

int main(int argc, char* argv[]) {
    /* Look at the line 208 to see where tableprinter is used*/
    using namespace tableprinter;

    printer p
    {
        { 
            { name { "type" }  , width { 16 } } ,
            { name { "plus" }  , width { 7 } , fixed { } , precision { 2 } } ,
            { name { "fplus" } , width { 7 } , fixed { } , precision { 2 } } ,
            { name { "times" } , width { 7 } , fixed { } , precision { 2 } } ,
            { name { "norm" }  , width { 7 } , fixed { } , precision { 2 } } ,
            { name { "cnvrt" } , width { 7 } , fixed { } , precision { 2 } } ,
            { name { "div" }   , width { 7 } , fixed { } , precision { 2 } } ,
            { name { "sqrt" }  , width { 7 } , fixed { } , precision { 2 } } ,
            { name { "pow" }   , width { 7 } , fixed { } , precision { 2 } }
        } ,
        std::cout
    };

    uint64_t n = 1000; 
    if (argc == 2) n = atoi(argv[1]);
    uint64_t m(100000000 / n);
    print_info( n , m );
    p.print_headers();
    test_all_types<std::vector>{ p }(n, m);
    test_all_types<std::deque>{ p }(n, m);
    test_all_types<std::list>{ p }(n, m);
}