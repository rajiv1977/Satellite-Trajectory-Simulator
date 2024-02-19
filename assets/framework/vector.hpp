#ifndef __VECTOR_H__
#define __VECTOR_H__

#pragma once

UISOFT_BEGIN_NAMESPACE(backend)

template <typename T = value_t>
class vector
{
    typedef T value_t;
    typedef vector<value_t> vector_t;

    private:
        value_t my_data[4];

    public:
        vector(value_t xi_x = T(), value_t xi_y = T(), value_t xi_z = T(), value_t xi_w = T())
        {
            my_data[0] = xi_x;
            my_data[1] = xi_y;
            my_data[2] = xi_z;
            my_data[3] = xi_w;
        }

        vector(const vector_t& xi_other)
        {
            if(this != &xi_other)
            {
                my_data[0] = xi_other.my_data[0];
                my_data[1] = xi_other.my_data[1];
                my_data[2] = xi_other.my_data[2];
                my_data[3] = xi_other.my_data[3];
            }
        }

        const vector_t& operator = (const vector_t& xi_other)
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

        bool operator == (const vector_t& xi_other)
        {
            return my_data[0] == xi_other.my_data[0] && my_data[1] == xi_other.my_data[1] && my_data[2] == xi_other.my_data[2];
        }

        bool operator != (const vector_t& xi_other)
        {
            return !(operator == (xi_other));
        }

        vector_t operator - ()
        {
            vector_t result(*this);
            result.negate();
            return result;
        }

        vector_t operator + (vector_t xi_vector)
        {
            vector_t result;

            result.my_data[0] = my_data[0]+xi_vector.my_data[0];
            result.my_data[1] = my_data[1]+xi_vector.my_data[1];
            result.my_data[2] = my_data[2]+xi_vector.my_data[2];

            return result;
        }

        vector_t operator - (vector_t xi_vector)
        {
            vector_t result;

            result.my_data[0] = my_data[0]-xi_vector.my_data[0];
            result.my_data[1] = my_data[1]-xi_vector.my_data[1];
            result.my_data[2] = my_data[2]-xi_vector.my_data[2];

            return result;
        }

        value_t x() const
        {
            return my_data[0];
        }

        value_t y() const
        {
            return my_data[1];
        }

        value_t z() const
        {
            return my_data[2];
        }

        value_t w() const
        {
            return my_data[3];
        }

        value_t& x()
        {
            return my_data[0];
        }

        value_t& y()
        {
            return my_data[1];
        }

        value_t& z()
        {
            return my_data[2];
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

        void negate()
        {
            my_data[0] = -my_data[0];
            my_data[1] = -my_data[1];
            my_data[2] = -my_data[2];
        }

        value_t module() const
        {
            return std::sqrt((my_data[0]*my_data[0])+(my_data[1]*my_data[1])+(my_data[2]*my_data[2]));
        }

        void normalize()
        {
            value_t m = module();

            if(m != T())
            {
                my_data[0] /= m;
                my_data[1] /= m;
                my_data[3] /= m;
            }
        }

        vector_t normalized()
        {
            vector_t result(*this);
            result.normalize();
            return result;
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
        friend vector_t operator * (value_t xi_scalar, const vector_t& xi_vector);

        template <typename T>
        friend vector_t operator * (const vector_t& xi_vector, value_t xi_scalar);

        template <typename T>
        friend vector_t operator / (const vector_t& xi_vector, value_t xi_scalar);
};

template <typename T>
inline vector<T> operator * (T xi_scalar, const vector<T>& xi_vector)
{
    vector<T> result;

    result.my_data[0] = xi_vector.my_data[0]*xi_scalar;
    result.my_data[1] = xi_vector.my_data[1]*xi_scalar;
    result.my_data[2] = xi_vector.my_data[2]*xi_scalar;

    return result;
}

template <typename T>
inline vector<T> operator * (const vector<T>& xi_vector, T xi_scalar)
{
    vector<T> result;

    result.my_data[0] = xi_vector.my_data[0]*xi_scalar;
    result.my_data[1] = xi_vector.my_data[1]*xi_scalar;
    result.my_data[2] = xi_vector.my_data[2]*xi_scalar;

    return result;
}

template <typename T>
inline vector<T> operator / (const vector<T>& xi_vector, T xi_scalar)
{
    vector<T> result;

    result.my_data[0] = xi_vector.my_data[0]/xi_scalar;
    result.my_data[1] = xi_vector.my_data[1]/xi_scalar;
    result.my_data[2] = xi_vector.my_data[2]/xi_scalar;

    return result;
}

UISOFT_END_NAMESPACE

#endif // __VECTOR_H__
