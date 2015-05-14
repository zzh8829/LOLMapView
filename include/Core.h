#ifndef Z_CORE_H_
#define Z_CORE_H_

// C++ 11 check
#if __cplusplus <= 199711L
	#error C++11 compliant compiler is required
#endif

// Windows
#if defined(_WIN32) || defined(_WIN64)
	
	#if defined(_MSC_VER)
		#error Vistual C++ is not supported
	#endif

// Apple OS X
#elif defined(__APPLE__)
	
	// Nothing to do

// Linux
#elif defined(__linux__)

	// Nothing to do

#endif

// C headers
#include <cassert>
#include <cctype>
#include <cfenv>
#include <cfloat>
#include <cinttypes>
#include <climits>
#include <clocale>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cwctype>

// C++ headers

// Containers
#include <array>
#include <bitset>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Input/Output
#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>

// Miscellaneous
#include <algorithm>
#include <chrono>
#include <complex>
#include <exception>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <locale>
#include <memory>
#include <new>
#include <numeric>
#include <random>
#include <ratio>
#include <regex>
#include <stdexcept>
#include <string>
#include <system_error>
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <utility>
#include <valarray>

#endif
