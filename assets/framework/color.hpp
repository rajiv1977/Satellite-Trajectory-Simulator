#ifndef __COLOR_H__
#define __COLOR_H__

#pragma once

UISOFT_BEGIN_NAMESPACE(backend)

template <typename T = value_t>
class color
{
    typedef color<value_t> color_t;

    private:
        value_t my_data[4]; // r, g, b, a

    public:
        color(value_t xi_r = T(), value_t xi_g = T(), value_t xi_b = T(), value_t xi_a = T())
        {
            my_data[0] = xi_r;
            my_data[1] = xi_g;
            my_data[2] = xi_b;
            my_data[3] = xi_a;
        }

        color(const color_t& xi_other)
        {
            if(this != &xi_other)
            {
                my_data[0] = xi_other.my_data[0];
                my_data[1] = xi_other.my_data[1];
                my_data[2] = xi_other.my_data[2];
                my_data[3] = xi_other.my_data[3];
            }
        }

        const color_t& operator = (const color_t& xi_other)
        {
            if(this != &xi_other)
            {
                my_data[0] = xi_other.my_data[0];
                my_data[1] = xi_other.my_data[1];
                my_data[2] = xi_other.my_data[2];
                my_data[3] = xi_other.my_data[3];
            }

            return *this;
        }

        bool operator == (const color_t& xi_other)
        {
            return my_data[0] == xi_other.my_data[0] && my_data[1] == xi_other.my_data[1] && my_data[2] == xi_other.my_data[2];
        }

        bool operator != (const color_t& xi_other)
        {
            return !(operator == (xi_other));
        }

        value_t red() const
        {
            return my_data[0];
        }

        value_t green() const
        {
            return my_data[1];
        }

        value_t blue() const
        {
            return my_data[2];
        }

        value_t alpha() const
        {
            return my_data[3];
        }

        value_t& red()
        {
            return my_data[0];
        }

        value_t& green()
        {
            return my_data[1];
        }

        value_t& blue()
        {
            return my_data[2];
        }

        value_t& alpha()
        {
            return my_data[3];
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

#endif // __COLOR_H__
