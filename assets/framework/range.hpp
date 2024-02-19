#ifndef __RANGE_H__
#define __RANGE_H__

#pragma once

UISOFT_BEGIN_NAMESPACE(backend)

// () => false,false
// [) => true,false
// (] => false,true
// [] => true,true
template <bool L = true, bool U = true, typename T = value_t>
class range
{
    typedef T value_t;
    typedef range<L, U, value_t> range_t;

    typedef std::numeric_limits<value_t> numericlimits_t;

    private:
        value_t my_min_value;
        value_t my_max_value;

    public:
        range() : my_min_value(numericlimits_t::quiet_NaN()), my_max_value(numericlimits_t::quiet_NaN())
        {
        }

        range(value_t xi_min_value, value_t xi_max_value) : my_min_value(xi_min_value), my_max_value(xi_max_value)
        {
        }

        range(const range_t& xi_other)
        {
            if(this != &xi_other)
            {
                my_min_value = xi_other.my_min_value;
                my_max_value = xi_other.my_max_value;
            }
        }

        const range_t& operator = (const range_t& xi_other)
        {
            if(this != &xi_other)
            {
                my_min_value = xi_other.my_min_value;
                my_max_value = xi_other.my_max_value;
            }

            return *this;
        }

        bool operator == (const range_t& xi_other)
        {
            return my_min_value == xi_other.my_min_value && my_max_value == xi_other.my_max_value;
        }

        bool operator != (const range_t& xi_other)
        {
            return !(operator == (xi_other));
        }

        value_t min_value() const
        {
            return my_min_value;
        }

        value_t& min_value()
        {
            return my_min_value;
        }

        value_t max_value() const
        {
            return my_max_value;
        }

        value_t& max_value()
        {
            return my_max_value;
        }

        value_t delta() const
        {
            return std::abs(my_max_value-my_min_value);
        }

        value_t median() const
        {
            return ((my_max_value+my_min_value)/2.0);
        }

        bool inside(value_t xi_value) const
        {
            bool lower_bound = L ? xi_value >= my_min_value : xi_value > my_min_value;
            bool upper_bound = U ? xi_value <= my_max_value : xi_value < my_max_value;

            return lower_bound && upper_bound;
        }

        bool inside(const range_t& xi_range) const
        {
            bool lower_bound = L ? xi_value >= xi_range.my_min_value : xi_value > xi_range.my_min_value;
            bool upper_bound = U ? xi_value <= xi_range.my_max_value : xi_value < xi_range.my_max_value;

            return lower_bound && upper_bound;
        }

        bool above_lower_bound(value_t xi_value) const
        {
            return (L ? xi_value >= my_min_value : xi_value > my_min_value);
        }
        
        bool below_lower_bound(value_t xi_value) const
        {
            return (L ? xi_value < my_min_value : xi_value <= my_min_value);
        }

        bool below_upper_bound(value_t xi_value) const
        {
            return (U ? xi_value <= my_max_value : xi_value < my_max_value);
        }

        bool above_upper_bound(value_t xi_value) const
        {
            return (U ? xi_value > my_max_value : xi_value >= my_max_value);
        }
};

typedef range<false, true, value_t> timerange_t;

UISOFT_END_NAMESPACE

#endif // __RANGE_H__
