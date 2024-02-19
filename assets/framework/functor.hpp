#ifndef __FUNCTOR_H__
#define __FUNCTOR_H__

#pragma once

#undef min
#undef max

UISOFT_BEGIN_NAMESPACE(backend)

template <typename T> inline const T& minimum(const T& xi_t1, const T& xi_t2)
{
    return xi_t1 < xi_t2 ? xi_t1 : xi_t2;
}

template <typename T> inline const T& minimum(const T& xi_t1, const T& xi_t2, const T& xi_t3)
{
    return xi_t1 > xi_t2 ? (xi_t2 > xi_t3 ? xi_t3 : xi_t2) : (xi_t1 > xi_t3 ? xi_t3 : xi_t1);
}

template <typename T> inline const T& maximum(const T& xi_t1, const T& xi_t2)
{
    return xi_t1 > xi_t2 ? xi_t1 : xi_t2;
}

template <typename T> inline const T& maximum(const T& xi_t1, const T& xi_t2, const T& xi_t3)
{
    return xi_t1 > xi_t2 ? (xi_t1 > xi_t3 ? xi_t1 : xi_t3) : (xi_t2 > xi_t3 ? xi_t2 : xi_t3);
}

template <typename T> inline const T absolute(const T xi_t)
{
    return xi_t >= 0 ? xi_t : -xi_t;
}

template <typename T> inline int compare(const T& xi_key1, const T& xi_key2)
{
    if(xi_key1 < xi_key2)
    {
        return -1;
    }
    else if(xi_key1 == xi_key2)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

inline int get_next_size(int xi_size)
{
    static const int fibsequence[] =
    {
              1,         2,         3,          5,          8,         13,         21,         34,
             55,        89,       144,        233,        377,        610,        987,       1597,
           2584,      4181,      6765,      10946,      17711,      28657,      46368,      75025,
         121393,    196418,    317811,     514229,     832040,    1346269,    2178309,    3524578,
        5702887,   9227465,  14930352,   24157817,   39088169,   63245986,  102334155,  165580141 
    };

    static const int n = sizeof(fibsequence)/sizeof(unsigned int);

    int i = 0;

    int size = xi_size;
            
    while(!(size < fibsequence[i]))
    {
        i++;
    }

    return ALIGN(fibsequence[i], DEFAULT_ALIGNMENT);
}

template <typename T> inline T round(T xi_x)
{
    if(xi_x >= 0.5)
    {
        return std::ceil(xi_x);
    }
    else
    {
        return std::floor(xi_x);
    }
}

template <typename T> inline T opengl_round(T xi_x)
{
    return xi_x;//??((int)(round<T>(((T)xi_x)*2)/2))+(T)0.5;
}

template <class T>//??
void shift_data(std::vector<T> &data, int ele_start_index, int ele_size, int num_elements, int num_ele_shift, bool backward =true)
{
    if (backward)
    {
        memmove(&(data[ele_size*ele_start_index]), &(data[ele_size*(ele_start_index + num_ele_shift)]),(ele_size*(num_elements-1))*sizeof(T));
    }
    else
    {
        memmove(&(data[ele_size*(ele_start_index+num_ele_shift)]), &(data[ele_size*ele_start_index]),(ele_size*(num_elements))*sizeof(T));
    }
}

template <class T>
void copy_data(std::vector<T> &source_data, std::vector<T> &dest_data, int source_ele_start_index, int dest_ele_start_index, int ele_size, int num_elements)
{
    memcpy(&(dest_data[ele_size*dest_ele_start_index]), &(source_data[ele_size*source_ele_start_index]),(ele_size*num_elements)*sizeof(T));
}

inline value_t pow10(int xi_i)
{
    value_t result = 1;

    for(int j = 0; j < xi_i; j++)
    {
        result *= 10;
    }

    return result;
}

inline value_t nice_number(value_t xi_x, bool xi_round)
{
    int exp = (int)std::log10(xi_x);

    value_t f = (value_t)(xi_x/pow10(exp));

    value_t nf = 1;

    if(xi_round)
    {
        if(f < 1.5)
        {
            nf = 1;
        }
        else if(f < 3)
        {
            nf = 2;
        }
        else if(f < 7)
        {
            nf = 5;
        }
        else
        {
            nf = 10;
        }
    }
    else
    {
        if(f <= 1)
        {
            nf = 1;
        }
        else if(f <= 2)
        {
            nf = 2;
        }
        else if(f <= 5)
        {
            nf = 5;
        }
        else
        {
            nf = 10;
        }
    }

    return (value_t)(nf*pow10(exp));
}

inline value_t calculate_interval(value_t xi_available_size, value_t xi_max_interval_size, value_t xi_range)
{
    if(xi_available_size <= 0 || !(xi_range > std::numeric_limits<value_t>::epsilon()))
    {
        return xi_range/10; // s potolka
    }

    #define exponent(x) ((value_t)(std::ceil(std::log((x)))))
    #define mantissa(x) ((value_t)((x)/std::pow(10, exponent((x))-1)))

    // reduce intervals for horizontal axis.
    // real maximum interval count
    value_t maxIntervalCount = xi_available_size / xi_max_interval_size;

    xi_range = std::abs(xi_range);

    value_t interval = std::pow(10, exponent(xi_range));
    value_t tempInterval = interval;

    // decrease interval until interval count becomes less than maxIntervalCount
    for(;;)
    {
        int m = (int)mantissa(tempInterval);
        if (m == 5)
        {
            // reduce 5 to 2
            tempInterval = tempInterval / 2.5f;
        }
        else if (m == 2 || m == 1 || m == 10)
        {
            // reduce 2 to 1, 10 to 5, 1 to 0.5
            tempInterval = tempInterval / 2.0;
        }
        else
        {
            tempInterval = tempInterval / 2.0;
        }

        if(xi_range / tempInterval > maxIntervalCount)
        {
            break;
        }

        if(boost::math::isnan<value_t>(tempInterval) || boost::math::isinf<value_t>(tempInterval))
        {
            break;
        }

        interval = tempInterval;
    }

    interval = nice_number(interval, true);

    return interval;
}

struct cdeleter
{
    void operator () (void * const xi_p) const
    {
        if(xi_p != 0)
        {
            ::free(xi_p);
        }
    }
};

template <typename T>
struct cppdeleter_inplace
{
    void operator () (void * const xi_p) const
    {
        if(xi_p != 0)
        {
            ((T*)xi_p)->~T();
            ::free(xi_p);
        }
    }
};

template <typename T>
struct cppdeleter
{
    void operator () (void * const xi_p) const
    {
        if(xi_p != 0)
        {
            delete xi_p;
        }
    }
};

inline int insure_range(int xi_value, int xi_minimum, int xi_maximum)
{
    return std::min(std::max(xi_value, xi_minimum), xi_maximum);
}

inline double insure_range(double xi_value, double xi_minimum, double xi_maximum)
{
    return std::min(std::max(xi_value, xi_minimum), xi_maximum);
}

inline void create_directory(const std::string& xi_directory)
{
#ifdef WINDOWS
    static const std::string separators("\\/");

    DWORD attributes = ::GetFileAttributesA(xi_directory.c_str());

    if(attributes == INVALID_FILE_ATTRIBUTES)
    {
        std::size_t slash_index = xi_directory.find_last_of(separators);

        if(slash_index != std::wstring::npos)
        {
            create_directory(xi_directory.substr(0, slash_index));
        }

        BOOL result = ::CreateDirectoryA(xi_directory.c_str(), 0);

        if(result == FALSE && GetLastError() != ERROR_ALREADY_EXISTS)
        {
            throw std::runtime_error("Could not create directory");
        }
    }
    else
    {
        bool directory_or_junction = ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0) || ((attributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);

        if(!directory_or_junction)
        {
            throw std::runtime_error("Could not create directory because a file with the same name exists");
        }
  }
#endif
}

UISOFT_END_NAMESPACE

#endif // __FUNCTOR_H__
