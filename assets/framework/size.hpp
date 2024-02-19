#ifndef __SIZE_H__
#define __SIZE_H__

#pragma once

UISOFT_BEGIN_NAMESPACE(backend)

template <typename T = value_t>
class size
{
    typedef T value_t;
    typedef size<value_t> size_t;

    private:
        value_t my_data[3];

    public:
        size(value_t xi_x = T(), value_t xi_y = T(), value_t xi_z = T())
        {
            my_data[0] = xi_x;
            my_data[1] = xi_y;
            my_data[2] = xi_z;
        }

        size(const size_t& xi_other)
        {
            if(this != &xi_other)
            {
                my_data[0] = xi_other.my_data[0];
                my_data[1] = xi_other.my_data[1];
                my_data[2] = xi_other.my_data[2];
            }
        }

        const size_t& operator = (const size_t& xi_other)
        {
            if(this != &xi_other)
            {
                my_data[0] = xi_other.my_data[0];
                my_data[1] = xi_other.my_data[1];
                my_data[2] = xi_other.my_data[2];
            }

            return *this;
        }

        bool operator == (const size_t& xi_other)
        {
            return my_data[0] == xi_other.my_data[0] && my_data[1] == xi_other.my_data[1] && my_data[2] == xi_other.my_data[2];
        }

        bool operator != (const size_t& xi_other)
        {
            return !(operator == (xi_other));
        }

        value_t x() const
        {
            return my_data[0];
        }

        value_t& x()
        {
            return my_data[0];
        }

        value_t y() const
        {
            return my_data[1];
        }

        value_t& y()
        {
            return my_data[1];
        }

        value_t z() const
        {
            return my_data[2];
        }

        value_t& z()
        {
            return my_data[2];
        }

        const value_t* get() const
        {
            return my_data;
        }

        value_t* get()
        {
            return my_data;
        }
};

UISOFT_END_NAMESPACE

#endif // __SIZE_H__
