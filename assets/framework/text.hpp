#ifndef __TEXT_H__
#define __TEXT_H__

#pragma once

//*******************************************************************************
// Unicode                                                                      *
//*******************************************************************************
/*
 * Copyright 2001-2004 Unicode, Inc.
 * 
 * Disclaimer
 * 
 * This source code is provided as is by Unicode, Inc. No claims are
 * made as to fitness for any particular purpose. No warranties of any
 * kind are expressed or implied. The recipient agrees to determine
 * applicability of information provided. If this file has been
 * purchased on magnetic or optical media from Unicode, Inc., the
 * sole remedy for any claim will be exchange of defective media
 * within 90 days of receipt.
 * 
 * Limitations on Rights to Redistribute This Code
 * 
 * Unicode, Inc. hereby grants the right to freely use the information
 * supplied in this file in the creation of products supporting the
 * Unicode Standard, and to make copies of this file in any form
 * for internal or external distribution as long as this notice
 * remains attached.

 * Conversions between UTF32, UTF-16, and UTF-8. Source code file.
 * Author: Mark E. Davis, 1994.
 * Rev History: Rick McGowan, fixes & updates May 2001.
 * Sept 2001: fixed const & error conditions per
 * mods suggested by S. Parent & A. Lillich.
 * June 2002: Tim Dodd added detection and handling of incomplete
 * source sequences, enhanced error detection, added casts
 * to eliminate compiler warnings.
 * July 2003: slight mods to back out aggressive FFFE detection.
 * Jan 2004: updated switches in from-UTF8 conversions.
 * Oct 2004: updated to use UNI_MAX_LEGAL_UTF32 in UTF-32 conversions.
 */
#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long    UTF32;    /* at least 32 bits */
typedef unsigned short    UTF16;    /* at least 16 bits */
typedef unsigned char    UTF8;    /* typically 8 bits */
typedef unsigned char    Boolean; /* 0 or 1 */

/* Some fundamental constants */
#define UNI_REPLACEMENT_CHAR (UTF32)0x0000FFFD
#define UNI_MAX_BMP (UTF32)0x0000FFFF
#define UNI_MAX_UTF16 (UTF32)0x0010FFFF
#define UNI_MAX_UTF32 (UTF32)0x7FFFFFFF
#define UNI_MAX_LEGAL_UTF32 (UTF32)0x0010FFFF

static const int halfShift  = 10; /* used for shifting by 10 bits */

static const UTF32 halfBase = 0x0010000UL;
static const UTF32 halfMask = 0x3FFUL;

#define UNI_SUR_HIGH_START  (UTF32)0xD800
#define UNI_SUR_HIGH_END    (UTF32)0xDBFF
#define UNI_SUR_LOW_START   (UTF32)0xDC00
#define UNI_SUR_LOW_END     (UTF32)0xDFFF

typedef enum {
    conversionOK,         /* conversion successful */
    sourceExhausted,    /* partial character in source, but hit end */
    targetExhausted,    /* insuff. room in target for conversion */
    sourceIllegal        /* source sequence is illegal/malformed */
} ConversionResult;

typedef enum {
    strictConversion = 0,
    lenientConversion
} ConversionFlags;

/*
 * Index into the table below with the first byte of a UTF-8 sequence to
 * get the number of trailing bytes that are supposed to follow it.
 * Note that *legal* UTF-8 values can't have 4 or 5-bytes. The table is
 * left as-is for anyone who may want to do such conversion, which was
 * allowed in earlier algorithms.
 */
static const char trailingBytesForUTF8[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

/*
 * Magic values subtracted from a buffer value during UTF8 conversion.
 * This table contains as many values as there might be trailing bytes
 * in a UTF-8 sequence.
 */
static const UTF32 offsetsFromUTF8[6] = { 0x00000000UL, 0x00003080UL, 0x000E2080UL, 
             0x03C82080UL, 0xFA082080UL, 0x82082080UL };

/*
 * Once the bits are split out into bytes of UTF-8, this is a mask OR-ed
 * into the first byte, depending on how many bytes follow.  There are
 * as many entries in this table as there are UTF-8 sequence types.
 * (I.e., one byte sequence, two byte... etc.). Remember that sequencs
 * for *legal* UTF-8 will be 4 or fewer bytes total.
 */
static const UTF8 firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

Boolean isLegalUTF8(const UTF8 *source, int length);
Boolean isLegalUTF8Sequence(const UTF8 *source, const UTF8 *sourceEnd);
ConversionResult ConvertUTF8toUTF16(const UTF8** sourceStart, const UTF8* sourceEnd, UTF16** targetStart, UTF16* targetEnd, ConversionFlags flags);

#ifdef __cplusplus
}
#endif

inline Boolean isLegalUTF8(const UTF8 *source, int length) {
    UTF8 a;
    const UTF8 *srcptr = source+length;
    switch (length) {
    default: return false;
    /* Everything else falls through when "true"... */
    case 4: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return false;
    case 3: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return false;
    case 2: if ((a = (*--srcptr)) > 0xBF) return false;

    switch (*source) {
        /* no fall-through in this inner switch */
        case 0xE0: if (a < 0xA0) return false; break;
        case 0xED: if (a > 0x9F) return false; break;
        case 0xF0: if (a < 0x90) return false; break;
        case 0xF4: if (a > 0x8F) return false; break;
        default:   if (a < 0x80) return false;
    }

    case 1: if (*source >= 0x80 && *source < 0xC2) return false;
    }
    if (*source > 0xF4) return false;
    return true;
}

/*
 * Exported function to return whether a UTF-8 sequence is legal or not.
 * This is not used here; it's just exported.
 */
inline Boolean isLegalUTF8Sequence(const UTF8 *source, const UTF8 *sourceEnd) {
    int length = trailingBytesForUTF8[*source]+1;
    if (source+length > sourceEnd) {
    return false;
    }
    return isLegalUTF8(source, length);
}

inline ConversionResult ConvertUTF8toUTF16 (
    const UTF8** sourceStart, const UTF8* sourceEnd, 
    UTF16** targetStart, UTF16* targetEnd, ConversionFlags flags) {
    ConversionResult result = conversionOK;
    const UTF8* source = *sourceStart;
    UTF16* target = *targetStart;
    while (source < sourceEnd) {
    UTF32 ch = 0;
    unsigned short extraBytesToRead = trailingBytesForUTF8[*source];
    if (source + extraBytesToRead >= sourceEnd) {
        result = sourceExhausted; break;
    }
    /* Do this check whether lenient or strict */
    if (! isLegalUTF8(source, extraBytesToRead+1)) {
        result = sourceIllegal;
        break;
    }
    /*
     * The cases all fall through. See "Note A" below.
     */
    switch (extraBytesToRead) {
        case 5: ch += *source++; ch <<= 6; /* remember, illegal UTF-8 */
        case 4: ch += *source++; ch <<= 6; /* remember, illegal UTF-8 */
        case 3: ch += *source++; ch <<= 6;
        case 2: ch += *source++; ch <<= 6;
        case 1: ch += *source++; ch <<= 6;
        case 0: ch += *source++;
    }
    ch -= offsetsFromUTF8[extraBytesToRead];

    if (target >= targetEnd) {
        source -= (extraBytesToRead+1); /* Back up source pointer! */
        result = targetExhausted; break;
    }
    if (ch <= UNI_MAX_BMP) { /* Target is a character <= 0xFFFF */
        /* UTF-16 surrogate values are illegal in UTF-32 */
        if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
        if (flags == strictConversion) {
            source -= (extraBytesToRead+1); /* return to the illegal value itself */
            result = sourceIllegal;
            break;
        } else {
            *target++ = UNI_REPLACEMENT_CHAR;
        }
        } else {
        *target++ = (UTF16)ch; /* normal case */
        }
    } else if (ch > UNI_MAX_UTF16) {
        if (flags == strictConversion) {
        result = sourceIllegal;
        source -= (extraBytesToRead+1); /* return to the start */
        break; /* Bail out; shouldn't continue */
        } else {
        *target++ = UNI_REPLACEMENT_CHAR;
        }
    } else {
        /* target is a character in range 0xFFFF - 0x10FFFF. */
        if (target + 1 >= targetEnd) {
        source -= (extraBytesToRead+1); /* Back up source pointer! */
        result = targetExhausted; break;
        }
        ch -= halfBase;
        *target++ = (UTF16)((ch >> halfShift) + UNI_SUR_HIGH_START);
        *target++ = (UTF16)((ch & halfMask) + UNI_SUR_LOW_START);
    }
    }
    *sourceStart = source;
    *targetStart = target;
    return result;
}
//*******************************************************************************

#include "unicode/utypes.h"
#include "unicode/uchar.h"
#include "unicode/locid.h"
#include "unicode/ustring.h"
#include "unicode/ucnv.h"
#include "unicode/unistr.h"

UISOFT_BEGIN_NAMESPACE(backend)

typedef UChar char_t;
typedef UnicodeString text_t;

enum ehorizontaltextalignment
{
    Left = -1,
    Center = 0,
    Right = 1
};

enum everticaltextalignment
{
    etop = -1,
    emiddle = 0,
    ebottom = 1
};

class text
{
    public:
        static inline string_t& trim(string_t& xi_s)
        {
            return text::ltrim(text::rtrim(xi_s));
        }

        static inline string_t& ltrim(string_t& xi_s)
        {
            xi_s.erase(xi_s.begin(), std::find_if(xi_s.begin(), xi_s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
            return xi_s;
        }

        static inline string_t& rtrim(string_t& xi_s)
        {
            xi_s.erase(std::find_if(xi_s.rbegin(), xi_s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), xi_s.end());
            return xi_s;
        }

        static void replace(const string_t& xi_pattern, const string_t& xi_replace, string_t& xio_src)
        {
            for(size_t i = 0; (i = xio_src.find(xi_pattern, i)) != string_t::npos; i++)
            {
                xio_src.replace(i++, xi_pattern.length(), xi_replace);
            }
        }

        template <typename T>
        static void split(const T& xi_string,
                          const T& xi_delimiters,
                          std::vector<T>& xo_result,
                          bool xi_include_empty_result = true,
                          int xi_atleast = -1,
                          bool xi_one_char_column = false)
        {
            T::size_type last = 0;
            T::size_type pos = 0;

            T buff;

            while((pos = xi_string.find_first_of(xi_delimiters, last)) != T::npos)
            {
                buff.assign(xi_string, last, pos-last);

                if(xi_include_empty_result || !buff.empty())
                {
                    xo_result.push_back(buff);
                }

                if(xo_result.size() == xi_atleast)
                {
                    return;
                }

                last = pos+1;

                if(!xi_one_char_column) // skip 'delims'
                {
                    while(last < xi_string.length() && xi_delimiters.find(xi_string[last]) != T::npos)
                    {
                        ++last;
                    }
                }
            }

            // we always have column after 'delims', like "a|b|c|" has 4 columns: 'a', 'b', 'c', ''
            buff.assign(xi_string, last, xi_string.length()-last);

            if(xi_include_empty_result || !buff.empty())
            {
                xo_result.push_back(buff);
            }
        }

        static void to_lower(const string_t& xi_string, string_t& xo_result)
        {
            /*
            xo_result.clear();

            xo_result.reserve(xi_string.length()+1);

            for(size_t i = 0; i < xi_string.length(); i++)
            {
                xo_result += towlower(xi_string[i]);
            }
            */
            xo_result = xi_string;

            boost::algorithm::to_lower(xo_result);
        }

        static string_t& to_lower(string_t& xi_string)
        {
            // std::transform(xi_string.begin(), xi_string.end(), xi_string.begin(), std::tolower);
            boost::algorithm::to_lower(xi_string);
            return xi_string;
        }

        static void to_upper(const string_t& xi_string, string_t& xo_result)
        {
            xo_result.clear();

            xo_result.reserve(xi_string.length()+1);

            for(size_t i = 0; i < xi_string.length(); i++)
            {
                xo_result += towupper(xi_string[i]);
            }
        }

        static string_t& to_upper(string_t& xi_string)
        {
            std::transform(xi_string.begin(), xi_string.end(), xi_string.begin(), std::toupper);
            return xi_string;
        }

        #define STRING_CONVERSION_BUFFER_SIZE 512

        static std::wstring string_to_wstring(const char* xi_s)
        {
            return text::string_to_wstring(std::string(xi_s));
        }

        static std::wstring string_to_wstring(const std::string& xi_s)
        {
#ifdef WINDOWS
            std::wstring result;

            int count = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, xi_s.c_str(), -1, 0, 0);

            if(count > 0)
            {
                if(count < STRING_CONVERSION_BUFFER_SIZE/8)
                {
                    wchar buffer[STRING_CONVERSION_BUFFER_SIZE];

                    count = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, xi_s.c_str(), -1, buffer, count);

                    if(count > 0)
                    {
                        result.assign(buffer);
                    }
                }
                else
                {
                    boost::scoped_ptr<wchar> p(new wchar[count]);

                    count = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, xi_s.c_str(), -1, p.get(), count);

                    if(count > 0)
                    {
                        result.assign(p.get());
                    }
                }
            }

            return result;
#else
            std::wstring aux(xi_s.length(), L' ');

            aux.assign(xi_s.begin(), xi_s.end());

            return aux;
#endif
        }

        static std::string wstring_to_string(const wchar* xi_s)
        {
            return text::wstring_to_string(std::wstring(xi_s));
        }

        static std::string wstring_to_string(const std::wstring& xi_s)
        {
#ifdef WINDOWS
            std::string result;

            int count = ::WideCharToMultiByte(CP_ACP, 0, xi_s.c_str(), -1, 0, 0, 0, 0);

            if(count > 0)
            {
                if(count < STRING_CONVERSION_BUFFER_SIZE/8)
                {
                    char buffer[STRING_CONVERSION_BUFFER_SIZE];

                    count = ::WideCharToMultiByte(CP_ACP, 0, xi_s.c_str(), -1, buffer, count, 0, 0);

                    if(count > 0)
                    {
                        result.assign(buffer);
                    }
                }
                else
                {
                    boost::scoped_ptr<char> p(new char[count]);

                    count = ::WideCharToMultiByte(CP_ACP, 0, xi_s.c_str(), -1, p.get(), count, 0, 0);

                    if(count > 0)
                    {
                        result.assign(p.get());
                    }
                }
            }

            return result;
#else
            std::string aux(xi_s.length(), L' ');

            aux.assign(xi_s.begin(), xi_s.end());

            return aux;
#endif
        }

        static text_t wstring_to_text(const wchar_t* xi_s)
        {
            return text::wstring_to_text(string_t(xi_s));
        }

        static text_t wstring_to_text(const string_t& xi_s)
        {
            return text_t(xi_s.data());
        }

        static string_t text_to_wstring(const char_t* xi_t)
        {
            return text::text_to_wstring(text_t(xi_t));
        }

        static string_t text_to_wstring(const text_t& xi_t)
        {
            return string_t(xi_t.getBuffer(), xi_t.length());
        }

        template <typename T>
        static const T* str_last_str(const T* xi_string, const T* xi_pattern)
        {
            const T* result(0);

            if(xi_string != 0 && xi_pattern != 0)
            {
                const T* found(0);

                for(size_t pos = 0; (found = ::wcsstr(xi_string+pos, xi_pattern)) != 0;)
                {
                    result = found;
                    pos = (found-xi_string)+1;
                }
            }

            return result;
        }

        template <>
        static const char* str_last_str<char>(const char* xi_string, const char* xi_pattern)
        {
            const char* result(0);

            if(xi_string != 0 && xi_pattern != 0)
            {
                const char* found(0);

                for(size_t pos = 0; (found = ::strstr(xi_string+pos, xi_pattern)) != 0;)
                {
                    result = found;
                    pos = (found-xi_string)+1;
                }
            }

            return result;
        }
};

inline void utf8_to_utf16(const unsigned char* xi_utf8_buffer, wchar* xo_utf16_buffer, int xi_count)
{
    xassert(xi_utf8_buffer != 0 && xo_utf16_buffer != 0, x::einvalidarg, 0);
    ConversionResult result = ConvertUTF8toUTF16(&xi_utf8_buffer, xi_utf8_buffer+xi_count, (UTF16**)(&xo_utf16_buffer), (UTF16*)(xo_utf16_buffer+xi_count), strictConversion);
    xassert(result == conversionOK, x::einvalidarg, 0);
}

UISOFT_END_NAMESPACE

#endif // __TEXT_H__
