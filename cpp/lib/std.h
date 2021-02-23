// All C++ Standard Library headers - https://en.cppreference.com/w/cpp/header
#pragma once

#if __cplusplus >= 199711L // ============================================ C++98 
//*// Utilities
#include <bitset> // bitset class template
#include <csetjmp> // Macro (and function) that saves (and jumps) to an execution context
#include <csignal> // Functions and macro constants for signal management
#include <cstdarg> // Handling of variable length argument lists
#include <cstddef> // Standard macros and typedefs
#include <cstdlib> // General purpose utilities: program control, dynamic memory allocation, random numbers, sort and search
#include <ctime>   // C-style time/date utilites
#include <functional> // Function objects, Function invocations, Bind operations and Reference wrappers
#include <typeinfo>   // Runtime type information utilities
#include <utility>    // Various utility components */

//*// Dynamic memory management
#include <memory> // High-level memory management utilities
#include <new>    // Low-level memory management utilities */

//*// Numeric limits
#include <cfloat>  // Limits of floating-point types
#include <climits> // Limits of integral types
#include <limits>  // Uniform way to query properties of arithmetic types */

//*// Error handling
#include <cassert>   // Conditionally compiled macro that compares its argument to zero
#include <cerrno>    // Macro containing the last error number
#include <exception> // Exception handling utilities
#include <stdexcept> // Standard exception objects */

//*// Strings
#include <cctype>  // Functions to determine the category of narrow characters
#include <cstring> // Various narrow character string handling functions
#include <cwchar>  // Various wide and multibyte string handling functions
#include <cwctype> // Functions to determine the catagory of wide characters
#include <string>  // basic_string class template */

//*// Containers
#include <deque>  // deque container
#include <list>   // list container
#include <map>    // map and multimap associative containers
#include <queue>  // queue and priority_queue container adaptors
#include <set>    // set and multiset associative containers
#include <stack>  // stack container adaptor
#include <vector> // vector container */

//*// Iterators
#include <iterator> // Range iterators */

//*// Algorithms
#include <algorithm> // Algorithms that operate on ranges */

//*// Numerics
#include <cmath>    // Common mathematics functions
#include <complex>  // Complex number type
#include <numeric>  // Numeric operations on values in ranges
#include <valarray> // Class for representing and manipulating arrays of values */

//*// Localization
#include <clocale> // C localization utilities
#include <locale>  // Localization utilities */

//*// Input/output
#include <cstdio>    // C-style input-output functions */
#include <fstream>   // basic_fstream, basic_ifstream, basic_ofstream class templates and several typedefs
#include <iomanip>   // Helper functions to control the format of input and output
#include <ios>       // ios_base class, basic_ios class template and several typedefs
#include <iosfwd>    // Forward declarations of all classes in the input/output
#include <iostream>  // Several standard stream objects
#include <istream>   // basic_istream class template and several typedefs
#include <ostream>   // basic_ostream, basic_iostream class templates and several typedefs
#include <sstream>   // basic_stringstream, basic_istringstream, basic_ostringstream class templates and several typedefs
#include <streambuf> // basic_streambuf class template
// #include <strstream> // (deprecated in C++98) strstream, istrstream, ostrstream */
#endif

#if __cplusplus >= 201103L // ============================================ C++11 
#include <chrono>           // time utilites
#include <initializer_list> // initializer_list class template
#include <tuple>            // tuple class template
#include <type_traits>      // Compile-time type information
#include <typeindex>        // type_index */
//*// Dynamic memory management
#include <scoped_allocator> // Nested allocator class */
//*// Numeric limits
#include <cinttypes> // Formatting macros, intmax_t and uintmax_t math and conversions
#include <cstdint> // Fixed-width integer types and limits of other types */
//*// Error handling
#include <system_error> // Defines error_code, a platform-dependent error code */
//*// Strings
#include <cuchar> // C-style Unicode character conversion functions */
//*// Containers
#include <array>        // array container
#include <forward_list> // forward_list container
#include <unordered_map> // unordered_map and unordered_multimap unordered associative containers
#include <unordered_set> // unordered_set and unordered_multiset unordered associative containers */
//*// Numerics
#include <cfenv>  // Floating-point environment access functions
#include <random> // Random number generators and distributions
#include <ratio>  // Compile-time rational arithmetic */
//*// Localization
#include <codecvt> // Unicode conversion facilities (deprecated in C++17) */
//*// Regular Expressions
#include <regex> // Classes, algorithms and iterators to support regular expression processing */
//*// Atomic Operations
#include <atomic> // Atomic operations */
//*// Thread support
#include <condition_variable> // Thread waiting conditions
#include <future>             // Primitives for asynchronous computations
#include <mutex>              // Mutual exclusion primitives
#include <thread>             // thread class and supporting functions */
#endif

#if __cplusplus >= 201402L // ============================================ C++14 
//*// Thread support
#include <shared_mutex> // Shared mutual exclusion primitives */
#endif

#if __cplusplus >= 201703L // ============================================ C++17
#include <any>      // any class
#include <optional> // optional class template
#include <variant>  // variant class template */
//*// Dynamic memory management
#include <memory_resource> // Polymorphic allocators and memory resources */
//*// Strings
#include <charconv>    // to_chars and from_chars
#include <string_view> // basic_string_view class template */
//*// Filesystem
#include <filesystem> // path class and supporting functions */
/*// Algorithms
#include <execution> // Predefined execution policies for parallel versions of the algorithms */
#endif

#if __cplusplus >= 202002L // =========================================================== C++20
#include <compare>         // Three-way comparison operator support
#include <concepts>        // Fundamental library concepts
#include <coroutine>       // Coroutines library
#include <source_location> // Supplies means to obtain source code location
#include <version> // Supplies implementation-dependent library information */
//*// Strings
#include <format> // Formatting library including format */
//*// Containers
#include <span> // span view */
//*// Ranges
#include <ranges> // Range access, primitives, requirements, utilities and adaptors */
//*// Numerics
#include <bit>     // Bit manipulation functions
#include <numbers> // Math constants */
//*// Input/output
#include <syncstream> // basic_osyncstream, basic_syncbuf, and typedefs */
//*// Thread support
#include <barrier>    // Barriers */
#include <latch>      // Latches
#include <semaphore>  // Semaphores
#include <stop_token> // Stop tokens for jthread
#define C17
#endif

/*// =============================================================== deprecated
#include <assert.h>   // sames as <cassert>
#include <errno.h>    // sames as <cerrno>
#include <float.h>    // sames as <cfloat>
#include <limits.h>   // sames as <climits>
#include <ctype.h>    // sames as <cctype>  in global namespace
#include <fenv.h>     // sames as <cfenv>   in global namespace
#include <inttypes.h> // sames as <cinttypes> in global namespace
#include <locale.h>   // sames as <clocale> in global namespace
#include <math.h>     // sames as <cmath>   in global namespace
#include <setjmp.h>   // sames as <csetjmp> in global namespace
#include <signal.h>   // sames as <csignal> in global namespace
#include <stdarg.h>   // sames as <cstdarg> in global namespace
#include <stddef.h>   // sames as <cstddef> in global namespace
#include <stdint.h>   // sames as <cstdint> in global namespace
#include <stdio.h>    // sames as <cstdio>  in global namespace
#include <stdlib.h>   // sames as <cstdlib> in global namespace
#include <string.h>   // sames as <cstring> in global namespace
#include <time.h>     // sames as <ctime>   in global namespace
#include <uchar.h>    // sames as <cuchar>  in global namespace
#include <wchar.h>    // sames as <cwchar>  in global namespace 
#include <wctype.h>   // sames as <cwctype> in global namespace */