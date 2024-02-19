#ifndef __VARIANT_H__
#define __VARIANT_H__

#pragma once

UISOFT_BEGIN_NAMESPACE(backend)

typedef void* object_t;

typedef boost::variant<byte,
                       double,
                       int8_t,
                       int16_t,
                       int32_t,
                       int64_t,
                       uint64_t,
                       int,
                       object_t,
                       std::string,
                       std::wstring,
                       std::vector<byte>,
                       std::vector<double>,
                       std::vector<int8_t>,
                       std::vector<int16_t>,
                       std::vector<int32_t>,
                       std::vector<object_t>,
                       std::vector<std::string>,
                       std::vector<std::wstring>,
                       tm> variant_t;

UISOFT_END_NAMESPACE

#endif // __VARIANT_H__
