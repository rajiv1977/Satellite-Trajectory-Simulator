#ifndef __GUID_H__
#define __GUID_H__

#pragma once

UISOFT_BEGIN_NAMESPACE(backend)

#ifndef WINDOWS
typedef struct _GUID
{
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;
#endif

class guid
{
    typedef GUID guid_t;

    private:
        guid_t my_id;

    public:
        guid()
        {
#ifdef WINDOWS
            xassert(SUCCEEDED(::CoCreateGuid(&my_id)), x::eoperationfailed, L"Failed to create GUID.");
#else
            memset(&my_id, 0, sizeof(my_id));
#endif
        }

        guid(const guid& xi_other)
        {
            if(this != &xi_other)
            {
                my_id = xi_other.my_id;
            }
        }

        guid(const guid_t& xi_id) : my_id(xi_id)
        {
        }

        guid(const guid_t& xi_id, long xi_offset)  : my_id(xi_id)
        {
            my_id.Data1 += xi_offset;
        }

        guid_t& operator = (const guid& xi_other)
        {
            if(this != &xi_other)
            {
                my_id = xi_other.my_id;
            }

            return *this;
        }

        operator const guid_t& ()
        {
            return my_id;
        }

        operator guid_t& ()
        {
            return my_id;
        }

        const guid_t& id() const
        {
            return my_id;
        }

        guid_t& id()
        {
            return my_id;
        }

    public:
        string_t to_string() const
        {
            static const wchar guid_template[] = L"%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X";

            wchar buffer[48]; // 36 is enough, padding

            UISNWPRINTF(buffer,
                        ARRAY_SIZE(buffer),
                        CUT_SHORT,
                        guid_template,
                        my_id.Data1,
                        my_id.Data2,
                        my_id.Data3,
                        my_id.Data4[0],
                        my_id.Data4[1],
                        my_id.Data4[2],
                        my_id.Data4[3],
                        my_id.Data4[4],
                        my_id.Data4[5],
                        my_id.Data4[6],
                        my_id.Data4[7]);

            return string_t(buffer);
        }
};

UISOFT_END_NAMESPACE

#endif // __GUID_H__
