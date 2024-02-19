#ifndef __PCH_H__
#define __PCH_H__

#pragma once

#if WIN32 || _WIN32 || WIN64 || _WIN64 || _WINDOWS
#   define WINDOWS
#endif

// crt
#ifdef WINDOWS
#   include <stdint.h>
#   include <fcntl.h>
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <io.h>
#   include <stdlib.h>
#   include <stdio.h>
#   include <tchar.h>
#   include <time.h>
#   include <iostream>
#   include <fstream>
#   include <sstream>
#   include <assert.h>
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   include <ole2.h>
#   include <limits.h>
#   include <excpt.h>
#   include <stdint.h>
#else
#endif

// stl
#include <algorithm>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <hash_set>
#include <string>

#ifdef WINDOWS
#   pragma warning(disable: 4996) // _CRT_SECURE_NO_WARNINGS
#   pragma warning(disable: 4181) // qualifier applied to reference type; ignored
#   pragma warning(disable: 4786) // for debug > 255
#   pragma warning(disable: 4355) // 'this' : used in base member initializer list
#   pragma warning(disable: 4930) // prototyped function not called (was a variable definition intended?)
#   pragma warning(disable: 4345) // behavior change: an object of POD type constructed with an initializer of the form () will be default-initialized
#   pragma warning(disable: 4311) // pointer truncation from 'byte *' to 'address_t'
#   pragma warning(disable: 4312) // conversion from 'address_t' to 'byte *' of greater size
#   pragma warning(disable: 4996) // The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _tell. See online help for details.
#   pragma warning(disable: 4503) // decorated name length exceeded, name was truncated
#   pragma warning(disable: 4275) // non dll-interface class '...' used as base for dll-interface class
#endif

// boost
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_LIST_SIZE 40
#define BOOST_MPL_LIMIT_VECTOR_SIZE 40

#include <boost/version.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/ref.hpp>
#include <boost/swap.hpp>
#include <boost/flyweight.hpp>
#include <boost/tokenizer.hpp>
#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_comparison.hpp"
#include "boost/tuple/tuple_io.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/variant.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/math/special_functions/fpclassify.hpp>
#include <boost/ptr_container/ptr_container.hpp>

#define MEMORY_PROFILING    0
#define STANDARD_ALLOCATOR  1 // using standard crt new operator or memory manager

#if MEMORY_PROFILING == 1
#   define _CRTDBG_MAP_ALLOC
#   include <stdlib.h>
#   include <crtdbg.h>
#   undef STANDARD_ALLOCATOR
#   define STANDARD_ALLOCATOR 1 // using standard crt new operator or memory manager
#endif

#if MEMORY_PROFILING == 1
#   define DUMP_MEMORY_LEAK _CrtDumpMemoryLeaks();
#   define SET_REPORT_MODE  _CrtSetReportMode(_CRT_WARN|_CRT_ASSERT|_CRT_ERROR, _CRTDBG_MODE_DEBUG|_CRTDBG_MODE_WNDW);
#else
#   define DUMP_MEMORY_LEAK
#   define SET_REPORT_MODE
#endif

#undef interface
#define interface struct

#ifndef UNICODE
#   define UNICODE
#endif

#define LITTLE_ENDIAN 0
#define BIG_ENDIAN    1
#define PDP_ENDIAN    2

#define ENDIAN LITTLE_ENDIAN

#if defined(_WIN64)
#   define ALIGN(x, b) (((x)+(b)-1)/(b)*(b))
#   define DEFAULT_ALIGNMENT (64)
#else
#   define ALIGN(x, b) (((x)+(b)-1)&~((b)-1))
#   define DEFAULT_ALIGNMENT (32)
#endif

#define ABSOLUTEVAL(xiWait)     (xiWait)
#define RELATIVEVAL(xiWait)     (-(xiWait))

#define NANOSECONDS(xiNanos)    (((signed __int64)(xiNanos))/100L)
#define MICROSECONDS(xiMicros)  (((signed __int64)(xiMicros))*NANOSECONDS(1000L))
#define MILLISECONDS(xiMilli)   (((signed __int64)(xiMilli))*MICROSECONDS(1000L))
#define SECONDS(xiSeconds)      (((signed __int64)(xiSeconds))*MILLISECONDS(1000L))

#define BYTE_SIZE       (8)
#define ARRAY_SIZE(x)   (sizeof(x)/sizeof(x[0]))

#ifdef WINDOWS
#   if defined(WIN32DLL) || defined(WIN64DLL) || defined(_WINDLL) || defined(WINDLL) || defined(_USRDLL)
#       define DECLSPEC __declspec(dllexport)
#   else
#       define DECLSPEC __declspec(dllimport)
#   endif

#    define STDCALL __cdecl
#else
#    define DECLSPEC
#    define STDCALL
#endif

typedef unsigned char byte;
typedef wchar_t       wchar;

typedef double value_t;

#define MV_OPENGL_ENUM_TYPE  GL_DOUBLE
#define MV_OPENGL_VALUE_TYPE gldouble

#if defined(_WIN64)
typedef unsigned int64_t address_t;
#else
typedef unsigned int address_t;
#endif
typedef unsigned int hashval_t;

typedef std::wstring string_t;

#ifdef WINDOWS
typedef HWND        window_t;
typedef HDC         device_t;
typedef HGLRC       ogl_context_t;
typedef HINSTANCE   module_t;
#else
typedef int         window_t;
typedef int         device_t;
typedef int         ogl_context_t;
typedef int         module_t;
#endif

#ifdef WINDOWS
#   define SHARED_LIBRARY_SUFFIX (L".dll")
#else
#   define SHARED_LIBRARY_SUFFIX (L".so")
#endif

static const char*  EMPTY_STR = "";
static const wchar* EMPTY_WSTR = L"";
static const char*  NEWLINE_STR = "\n";
static const wchar* NEWLINE_WSTR = L"\n";

#ifdef WINDOWS
static const char* INT64_FORMAT = "%I64d";
#else
static const char* INT64_FORMAT = "%lld";
#endif

#define UISOFT_BEGIN_NAMESPACE(x) namespace uisoft { namespace etflib { namespace x {
#define UISOFT_END_NAMESPACE }; }; };
#define UISOFT_USING_NAMESPACE(x) using namespace uisoft::etflib::x;

#define UISOFT_MULTIMINI_BEGIN_NAMESPACE(x) namespace uisoft { namespace multimini { namespace etflib { namespace x {
#define UISOFT_MULTIMINI_END_NAMESPACE }; }; }; };
#define UISOFT_MULTIMINI_USING_NAMESPACE(x) using namespace uisoft::multimini::etflib::x;

#ifdef DISALLOW_COPY_MOVE_ASSIGNMENT
#   undef DISALLOW_COPY_MOVE_ASSIGNMENT
#endif
#define DISALLOW_COPY_MOVE_ASSIGNMENT(__T)  \
    private:                                \
        __T(const __T&);                    \
        __T(const __T&&);                   \
        const __T& operator = (const __T&); \
        const __T& operator = (const __T&&);

#ifdef WINDOWS
#   define UIVSNWPRINTF _vsnwprintf_s
#   define UIVSNPRINTF  _vsnprintf_s
#   define UISNWPRINTF  _snwprintf_s
#   define UISNPRINTF   _snprintf_s
#   define CUT_SHORT    _TRUNCATE
#   define FILEOPEN     _wfopen
#else
#   define UIVSNWPRINTF vsnwprintf_s
#   define UIVSNWPRINTF vsnprintf_s
#   define UISNWPRINTF  snwprintf_s
#   define UISNPRINTF   snwprintf_s
#   define CUT_SHORT    ((std::size_t)-1)
#   define FILEOPEN     fopen
#endif

#define MAX_LINE_SIZE (2048)

#ifndef PI
#   define PI                           (3.141592653589793)
#endif
#define DVAPI                           (2*PI)
#ifndef Ek
#   define Ek                           (2.718281828459045)
#endif
#define TRIG_RESOLUTION                 (360)
#define RADIAN_TO_DEGREE(xi_angle)      (xi_angle*(180.0/PI))
#define DEGREE_TO_RADIAN(xi_angle)      (PI*xi_angle/180.0)

#define MAX_STRING_SIZE                 (32767)
#define COMMENT_STYLE_NONE              (0)
#define COMMENT_STYLE_MATLAB            (1)
#define COMMENT_STYLE_C                 (2)
#define COMMENT_STYLE_STRING_MATLAB     L"%"
#define COMMENT_STYLE_STRING_C          L"//"
#define END_LINE_STRING_MATLAB          L"]"
#define END_LINE_STRING_C               L"]"

#ifdef WINDOWS
#   define MAX_PATH_SIZE               _MAX_PATH
#else
#   define MAX_PATH_SIZE                (1024)
#endif

// framework
/*#include <framework/x.hpp>
#include <framework/functor.hpp>
#include <framework/factory.hpp>
#include <framework/variant.hpp>
#include <framework/guid.hpp>
#include <framework/vector.hpp>
#include <framework/point.hpp>
#include <framework/size.hpp>
#include <framework/rect.hpp>
#include <framework/color.hpp>
#include <framework/path.hpp>
#include <framework/serializationcontext.hpp>
#include <framework/binarycontext.hpp>*/

/*#include <integration/defs/defs.hpp>
#include <integration/framework/range.hpp>

USING_INTEGRATION_NAMESPACE */

#endif // __PCH_H__
