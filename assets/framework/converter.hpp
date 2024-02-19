#ifndef __CONVERTER_H__
#define __CONVERTER_H__

#pragma once

#include <tinythread.h>
#include <framework/singleton.hpp>

UISOFT_BEGIN_NAMESPACE(backend)

class converter : public singleton_shared_memory<converter, 'cvrt'>
{
    public:
        template <typename T> std::string convet_to_string(const T& xi_value)
        {
            std::ostringstream result;

            xassert(result << xi_value, x::einvalidarg, L"Failed to convert to string.");

            return result.str();
        }

        template <typename T> string_t convet_to_wstring(const T& xi_value)
        {
            std::wostringstream result;

            xassert(result << xi_value, x::einvalidarg, L"Failed to convert to string.");

            return result.str();
        }

        template <typename T> T parse_from_string(const std::string& xi_value)
        {
            typedef T value_t;

            value_t result = value_t();

            char leftovers;

            std::istringstream input(xi_value);

            xassert(input >> result && !input.get(leftovers).good(), x::einvalidarg, L"Failed to parse from string.");

            return (value_t)result;
        }

        template <typename T> T parse_from_wstring(const string_t& xi_value)
        {
            typedef T value_t;

            value_t result = value_t();

            wchar leftovers;

            std::wistringstream input(xi_value);

            xassert(input >> result && !input.get(leftovers).good(), x::einvalidarg, L"Failed to parse from string.");

            return (value_t)result;
        }

        template <typename T> bool try_parse_from_string(const std::string& xi_value, T& xo_result)
        {
            typedef T value_t;

            value_t result;

            char leftovers;

            std::istringstream input(xi_value);

            xassert(input >> result && !input.get(leftovers).good(), x::einvalidarg, L"Failed to parse from string.");

            xo_result = (value_t)result;

            return true;
        }

        template <typename T> bool try_parse_from_wstring(const string_t& xi_value, T& xo_result)
        {
            typedef T value_t;

            value_t result;

            wchar leftovers;

            std::wistringstream input(xi_value);

            xassert(input >> result && !input.get(leftovers).good(), x::einvalidarg, L"Failed to parse from string.");

            xo_result = (value_t)result;

            return true;
        }
};

UISOFT_END_NAMESPACE

#endif // __CONVERTER_H__
