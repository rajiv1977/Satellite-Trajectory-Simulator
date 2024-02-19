#ifndef __X_H__
#define __X_H__

#pragma once

#if !defined(_NATIVE_WCHAR_T_DEFINED)
typedef unsigned short wchar;
#else
typedef wchar_t wchar;
#endif

//UISOFT_BEGIN_NAMESPACE(backend)

#include <stdarg.h>

#define EXCEPTION_BUFFER_SIZE (256)

class x : public std::exception
{
    public:

    enum eerror
    {
        einvalidarg = 0,
        eoutofmemory,
        eoutofrange,
        enullpointer,
        ebufferoverflow,
        ebufferunderflow,
        einvalidcontext,
        enotimplemented,
        einvalidoperation,
        eoperationfailed,
        ealreadyexist,
        efilenotfound
    };

    protected:
        int                     my_error;
        int                     my_sys_error;

        wchar                   my_buffer[EXCEPTION_BUFFER_SIZE];
        wchar                   my_text[EXCEPTION_BUFFER_SIZE];

        int                     my_line;
        const wchar*            my_file;

        char                    my_char_text[EXCEPTION_BUFFER_SIZE];

    public:
                                x(const wchar* xi_file = 0, int xi_line = 0, int xi_error = 0, int xi_sys_error = 0, const wchar* xi_text = 0, ...);
                                x(const x& xi_other);

        virtual                ~x();

        const x&                operator = (const x& xi_other);

        bool                    has_text()                      const;
        const wchar*            get_text()                      const;

        void                    wchar_to_char_text();

        virtual wchar*          why(bool xi_verbose = false);
        virtual const char*     what()                          const;
};

#define xassert(xi_expr, xi_err, xi_text, ...)                                                  \
{                                                                                               \
/*    if((xi_expr) == 0)                                                                          \
    {                                                                                           \
        throw x(TEXT(__FILE__), __LINE__, (xi_err), (sys_error()), (xi_text), ##__VA_ARGS__);   \
    } */                                                                                          \
}

unsigned int sys_error();

inline x::x(const wchar* xi_file, int xi_line, int xi_error, int xi_sys_error, const wchar* xi_text, ...)
        : my_error(xi_error), my_sys_error(xi_sys_error), my_line(xi_line), my_file(xi_file)
{
    my_buffer[0] = 0;
    my_text[0] = 0;
    my_char_text[0] = 0;

    if(xi_text != 0)
    {
        va_list arguments;

        va_start(arguments, xi_text);

        //UIVSNWPRINTF(my_text, ARRAY_SIZE(my_text), ((std::size_t)-1), xi_text, arguments);

        va_end(arguments);

        wchar_to_char_text();
    }
}

inline x::x(const x& xi_other)
{
    if(this != &xi_other)
    {
        my_error     = xi_other.my_error;
        my_sys_error = xi_other.my_sys_error;
        my_line      = xi_other.my_line;
        my_file      = xi_other.my_file;

        memcpy(my_buffer, xi_other.my_buffer, sizeof(my_buffer));
        memcpy(my_text, xi_other.my_text, sizeof(my_text));

        wchar_to_char_text();
    }
}

inline x::~x()
{
}

inline const x& x::operator = (const x& xi_other)
{
    if(this != &xi_other)
    {
        my_error     = xi_other.my_error;
        my_sys_error = xi_other.my_sys_error;
        my_line      = xi_other.my_line;
        my_file      = xi_other.my_file;

        memcpy(my_buffer, xi_other.my_buffer, sizeof(my_buffer));
        memcpy(my_text, xi_other.my_text, sizeof(my_text));

        wchar_to_char_text();
    }

    return *this;
}

inline wchar* x::why(bool xi_verbose)
{
    if(has_text())
    {
        if(xi_verbose)
        {
           // UISNWPRINTF(my_buffer, ARRAY_SIZE(my_buffer), CUT_SHORT, L"FILE: [%s], LINE: [%ld], ERROR: [%X], SYSERROR: [%X] => %s", my_file, my_line, my_error, my_sys_error, my_text);
        }
        else
        {
          //  UISNWPRINTF(my_buffer, ARRAY_SIZE(my_buffer), CUT_SHORT, L"%s", my_text);
        }
    }
    else
    {
        //UISNWPRINTF(my_buffer, ARRAY_SIZE(my_buffer), CUT_SHORT, L"FILE: [%s], LINE: [%ld], ERROR: [%X], SYSERROR: [%X]", my_file, my_line, my_error, my_sys_error);
    }

    return my_buffer;
}

inline const char* x::what() const
{
    return my_char_text;
}

inline bool x::has_text() const
{
    return my_text[0] != 0;
}

inline const wchar* x::get_text() const
{
    return my_text;
}

inline void x::wchar_to_char_text()
{
#ifdef WINDOWS
    ::WideCharToMultiByte(CP_ACP, 0, my_text, -1, my_char_text, EXCEPTION_BUFFER_SIZE, 0, 0);
#else
#endif
}

inline unsigned int sys_error()
{
#ifdef WINDOWS
    return ::GetLastError();
#else
    return 0;
#endif
}

#ifdef WINDOWS
inline int xfilter(unsigned int xi_code, struct _EXCEPTION_POINTERS* /*xi_ep*/)
{
    int result;

    if(xi_code == EXCEPTION_ACCESS_VIOLATION)
    {
        result = EXCEPTION_EXECUTE_HANDLER;
    }
    else
    {
        result = EXCEPTION_CONTINUE_SEARCH;
    }

   return result;
}
#endif

//UISOFT_END_NAMESPACE

#endif // __X_H__
