#ifndef __POINT_H__
#define __POINT_H__

#pragma once

UISOFT_BEGIN_NAMESPACE(backend)

template <typename T = value_t>
class point
{
    typedef T value_t;

    typedef vector<value_t> vector_t;
    typedef point<value_t> point_t;

    private:
        value_t my_data[4];

    public:
        point(value_t xi_x = T(), value_t xi_y = T(), value_t xi_z = T(), value_t xi_w = 1)
        {
            my_data[0] = xi_x;
            my_data[1] = xi_y;
            my_data[2] = xi_z;
            my_data[3] = xi_w;
        }

        point(const point_t& xi_other)
        {
            if(this != &xi_other)
            {
                my_data[0] = xi_other.my_data[0];
                my_data[1] = xi_other.my_data[1];
                my_data[2] = xi_other.my_data[2];
                my_data[3] = xi_other.my_data[3];
            }
        }

        const point_t& operator = (const point_t& xi_other)
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

        bool operator == (const point_t& xi_other)
        {
            return my_data[0] == xi_other.my_data[0] && my_data[1] == xi_other.my_data[1] && my_data[2] == xi_other.my_data[2] && my_data[3] == xi_other.my_data[3];
        }

        bool operator != (const point_t& xi_other)
        {
            return !(operator == (xi_other));
        }

        vector_t operator - (const point_t& xi_point) // difference of points is vector
        {
            vector_t result;

            result.get()[0] = my_data[0]-xi_point.my_data[0];
            result.get()[1] = my_data[1]-xi_point.my_data[1];
            result.get()[2] = my_data[2]-xi_point.my_data[2];

            return result;
        }

        point_t operator + (const vector_t& xi_vector) // +translate
        {
            point_t result;

            result.my_data[0] = my_data[0]+xi_vector.get()[0];
            result.my_data[1] = my_data[1]+xi_vector.get()[1];
            result.my_data[2] = my_data[2]+xi_vector.get()[2];

            return result;
        }

        point_t operator - (const vector_t& xi_vector) // -translate
        {
            point_t result;

            result.my_data[0] = my_data[0]-xi_vector.get()[0];
            result.my_data[1] = my_data[1]-xi_vector.get()[1];
            result.my_data[2] = my_data[2]-xi_vector.get()[2];

            return result;
        }

        point_t& operator += (const vector_t& xi_vector) // incremental translate
        {
            my_data[0] += xi_vector.get()[0];
            my_data[1] += xi_vector.get()[1];
            my_data[2] += xi_vector.get()[2];

            return *this;
        }

        point_t& operator -= (const vector_t& xi_vector) // decremental translate
        {
            my_data[0] -= xi_vector.get()[0];
            my_data[1] -= xi_vector.get()[1];
            my_data[2] -= xi_vector.get()[2];

            return *this;
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

        value_t w() const
        {
            return my_data[3];
        }

        value_t& w()
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

        void homogeneous()
        {
            if(my_data[3] != T())
            {
                my_data[0] /= my_data[3];
                my_data[1] /= my_data[3];
                my_data[3] /= my_data[3];
            }
        }

        template <typename T>
        friend point_t operator * (value_t xi_scalar, const point_t& xi_point);

        template <typename T>
        friend point_t operator * (const point_t& xi_point, value_t xi_scalar);

        template <typename T>
        friend point_t operator / (const point_t& xi_point, value_t xi_scalar);
};

template <typename T>
inline point<T> operator * (T xi_scalar, const point<T>& xi_point)
{
    point<T> result;

    result.my_data[0] = xi_point.my_data[0]*xi_scalar;
    result.my_data[1] = xi_point.my_data[1]*xi_scalar;
    result.my_data[2] = xi_point.my_data[2]*xi_scalar;

    return result;
}

template <typename T>
inline point<T> operator * (const point<T>& xi_point, T xi_scalar)
{
    point<T> result;

    result.my_data[0] = xi_point.my_data[0]*xi_scalar;
    result.my_data[1] = xi_point.my_data[1]*xi_scalar;
    result.my_data[2] = xi_point.my_data[2]*xi_scalar;

    return result;
}

template <typename T>
inline point<T> operator / (const point<T>& xi_point, T xi_scalar)
{
    point<T> result;

    result.my_data[0] = xi_point.my_data[0]/xi_scalar;
    result.my_data[1] = xi_point.my_data[1]/xi_scalar;
    result.my_data[2] = xi_point.my_data[2]/xi_scalar;

    return result;
}

UISOFT_END_NAMESPACE

#endif // __POINT_H__
