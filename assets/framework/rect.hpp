#ifndef __RECT_H__
#define __RECT_H__

#pragma once

UISOFT_BEGIN_NAMESPACE(backend)

template <typename T = value_t>
class rect
{
    typedef T value_t;

    typedef point<value_t> point_t;
    typedef rect<value_t> rect_t;

    private:
        value_t my_data[4];

    public:
        rect(value_t xi_left = T(), value_t xi_top = T(), value_t xi_right = T(), value_t xi_bottom = T())
        {
            my_data[0] = xi_left;
            my_data[1] = xi_top;
            my_data[2] = xi_right;
            my_data[3] = xi_bottom;
        }

        rect(const point_t& xi_left_top, const point_t& xi_right_bottom)
        {
            my_data[0] = xi_left_top.x();
            my_data[1] = xi_left_top.y();
            my_data[2] = xi_right_bottom.x();
            my_data[3] = xi_right_bottom.y();
        }

        rect(const rect_t& xi_other)
        {
            if(this != &xi_other)
            {
                my_data[0] = xi_other.my_data[0];
                my_data[1] = xi_other.my_data[1];
                my_data[2] = xi_other.my_data[2];
                my_data[3] = xi_other.my_data[3];
            }
        }

        const rect_t& operator = (const rect_t& xi_other)
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

        bool operator == (const rect_t& xi_other)
        {
            return my_data[0] == xi_other.my_data[0] && my_data[1] == xi_other.my_data[1] && my_data[2] == xi_other.my_data[2] && my_data[3] == xi_other.my_data[3];
        }

        bool operator != (const rect_t& xi_other)
        {
            return !(operator == (xi_other));
        }

        value_t left() const
        {
            return my_data[0];
        }

        value_t& left()
        {
            return my_data[0];
        }

        value_t top() const
        {
            return my_data[1];
        }

        value_t& top()
        {
            return my_data[1];
        }

        value_t right() const
        {
            return my_data[2];
        }

        value_t& right()
        {
            return my_data[2];
        }

        value_t bottom() const
        {
            return my_data[3];
        }

        value_t& bottom()
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

        void normalize()
        {
            if(my_data[0] > my_data[2])
            {
                std::swap(my_data[0], my_data[2]);
            }

            if(my_data[1] > my_data[3])
            {
                std::swap(my_data[1], my_data[3]);
            }
        }
};

UISOFT_END_NAMESPACE

#endif // __RECT_H__
