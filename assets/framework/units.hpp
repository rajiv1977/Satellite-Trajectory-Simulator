#ifndef __UNITS_H__
#define __UNITS_H__

#pragma once

UISOFT_BEGIN_NAMESPACE(backend)

enum edistanceunit
{
    eunknowndistanceunit,
    edistanceunit_m, 
    edistanceunit_km,
    edistanceunit_mi,
    edistanceunit_nmi,
    edistanceunit_ft
};

enum etimeunit
{
    eunknowntimeunit,
    etimeunit_s,
    etimeunit_hr,
    etimeunit_min
};

enum espeedunit
{
    eunknownspeedunit,
    espeedunit_m_s,
    espeedunit_km_h,
    espeedunit_knots,
    espeedunit_ft_min    
};

enum eangleunit
{
    eunknownangleunit,
    eangleunit_rad,
    eangleunit_deg      
};

UISOFT_END_NAMESPACE

#endif // __UNITS_H__
