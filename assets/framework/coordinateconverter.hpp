#ifndef __COORDINATECONVERTER_H__
#define __COORDINATECONVERTER_H__

#pragma once

#include <framework/pch.hpp>
#include <framework/unitconverter.hpp>
#include <framework/matrix.hpp>
#include <framework/units.hpp>

#define EARTH_ECCENTRICITY	(0.0818191)
#define EARTH_RADIUS		(6378.137)
#define PSITOL				(1.0e-9)
#define FLT_EPS             (1.0e-16)

UISOFT_BEGIN_NAMESPACE(backend)

class coordinateconverter
{
    public:
        typedef std::vector<std::vector<value_t> > value_t_matrix;
    
    public:
        static void define_lrf_origin_and_rotation_matrix(edistanceunit xi_distance_unit,
                                                          eangleunit xi_angle_unit,
                                                          value_t xi_latitude, 
                                                          value_t xi_longitude,                                                          
                                                          std::vector<value_t>& xo_origin,
                                                          value_t_matrix& xo_rotation_matrix)
        {
            double Dvarphi;

            xi_latitude = unitconverter::convert_angle_unit(xi_latitude, xi_angle_unit, eangleunit_rad);
            xi_longitude = unitconverter::convert_angle_unit(xi_longitude, xi_angle_unit, eangleunit_rad);

	        Dvarphi = EARTH_RADIUS/sqrt(1.0 - (value_t)pow(EARTH_ECCENTRICITY * sin(xi_latitude),2));

            xo_origin.resize(3,0.0);
	        xo_origin[0] = 0.0;
	        xo_origin[1] = (value_t)(0.5 * Dvarphi * (value_t)pow(EARTH_ECCENTRICITY,2) * (value_t)sin(2 * xi_latitude));
	        xo_origin[2] = (value_t)(Dvarphi * ((value_t)pow(EARTH_ECCENTRICITY * (value_t)sin(xi_latitude),2) - 1.0));

	        xo_origin[1] = unitconverter::convert_distance_unit(xo_origin[1], edistanceunit_km, xi_distance_unit);
	        xo_origin[2] = unitconverter::convert_distance_unit(xo_origin[2], edistanceunit_km, xi_distance_unit);

            matrix::init_matrix(3,3,xo_rotation_matrix);
	        xo_rotation_matrix[0][0] = -sin(xi_longitude);
	        xo_rotation_matrix[0][1] = cos(xi_longitude);
	        xo_rotation_matrix[0][2] = 0.0;

	        xo_rotation_matrix[1][0] = -sin(xi_latitude) * cos(xi_longitude);
	        xo_rotation_matrix[1][1] = -sin(xi_latitude) * sin(xi_longitude);
	        xo_rotation_matrix[1][2] = cos(xi_latitude);

	        xo_rotation_matrix[2][0] = cos(xi_latitude) * cos(xi_longitude);
	        xo_rotation_matrix[2][1] = cos(xi_latitude) * sin(xi_longitude);
	        xo_rotation_matrix[2][2] = sin(xi_latitude);
        }

        static void transform_ecef2geo(edistanceunit xi_distance_unit,
                                       eangleunit xi_angle_unit,
                                       value_t xi_x_ecf,
                                       value_t xi_y_ecf,
                                       value_t xi_z_ecf,
                                       value_t& xo_latitude, 
                                       value_t& xo_longitude,
                                       value_t& xo_altitude)
        {
	        value_t ecf_x, ecf_y, ecf_z, psi, psi0, c1, Rpsi;

	        ecf_x = unitconverter::convert_distance_unit(xi_x_ecf, xi_distance_unit, edistanceunit_km);
	        ecf_y = unitconverter::convert_distance_unit(xi_y_ecf, xi_distance_unit, edistanceunit_km);
	        ecf_z = unitconverter::convert_distance_unit(xi_z_ecf, xi_distance_unit, edistanceunit_km);

	        c1 = (value_t)(1.0 - (value_t)pow(EARTH_ECCENTRICITY,2));

	        psi = 0.0;
	        do
	        {
		        psi0 = psi;
		        psi = sin(psi);
		        Rpsi = (value_t)(EARTH_RADIUS / sqrt(1.0 - (value_t)pow(EARTH_ECCENTRICITY * psi,2)));
		        psi = atan2((value_t)(ecf_z + (1.0 - c1) * Rpsi * psi), (value_t)sqrt((value_t)pow(ecf_x,2) + (value_t)pow(ecf_y,2)));
		        psi0 = fabs(psi - psi0);
	        }
	        while (psi0 > PSITOL);

	        if (fabs(psi) < PSITOL)
	        {
		        //error("Latitude Zero. error in ecef2geo_pos()");
	        }

	        xo_latitude = psi;
	        if ((ecf_x != 0.0) || (ecf_y != 0.0))
	        {
		        xo_longitude = atan2(ecf_y, ecf_x);
	        }
	        else
	        {
		        xo_longitude = 0.0;
	        }
	        xo_altitude = ecf_z / sin(psi) - c1 * Rpsi;

            xo_latitude = unitconverter::convert_angle_unit(xo_latitude, eangleunit_rad, xi_angle_unit);
            xo_longitude = unitconverter::convert_angle_unit(xo_longitude, eangleunit_rad, xi_angle_unit);
	        xo_altitude = unitconverter::convert_distance_unit(xo_altitude, edistanceunit_km, xi_distance_unit);
        }
                
        static void transform_geo2lrf(edistanceunit xi_distance_unit,
                                      eangleunit xi_angle_unit,
                                      value_t xi_lrf_latitude,
                                      value_t xi_lrf_longitude,
                                      value_t xi_lrf_altitude,
                                      value_t xi_latitude, 
                                      value_t xi_longitude,
                                      value_t xi_altitude,                                      
                                      value_t& xo_x,
                                      value_t& xo_y,
                                      value_t& xo_z)
        {
	        if ((xi_latitude == xi_lrf_latitude) && (xi_longitude == xi_lrf_longitude) && (xi_altitude == xi_lrf_altitude))
	        {
		        xo_x = 0.0;
                xo_y = 0.0;
                xo_z = 0.0;
	        }
	        else
	        {		        
                std::vector<value_t> x(3);                

                std::vector<value_t> origin_lrf(3);
                value_t_matrix rotation_matrix_lrf;           
                matrix::init_matrix<value_t>(3,3,rotation_matrix_lrf);

                define_lrf_origin_and_rotation_matrix(xi_distance_unit,xi_angle_unit,xi_lrf_latitude,xi_lrf_longitude, origin_lrf, rotation_matrix_lrf);

                std::vector<value_t> origin(3);
                value_t_matrix rotation_matrix;           
                matrix::init_matrix<value_t>(3,3,rotation_matrix);

                define_lrf_origin_and_rotation_matrix(xi_distance_unit,xi_angle_unit,xi_latitude,xi_longitude, origin, rotation_matrix);
                
		        transform_lrf1_to_lrf2(origin, rotation_matrix, origin_lrf, rotation_matrix_lrf, x[0], x[1], x[2], xo_x, xo_y, xo_z);
                		        
	        }
        }

        static void transform_lrf2geo(edistanceunit xi_distance_unit,
                                      eangleunit xi_angle_unit,
                                      value_t xi_lrf_latitude,
                                      value_t xi_lrf_longitude,
                                      value_t xi_lrf_altitude,                                      
                                      value_t xi_x,
                                      value_t xi_y,
                                      value_t xi_z,
                                      value_t& xo_latitude, 
                                      value_t& xo_longitude,
                                      value_t& xo_altitude)
        {
            std::vector<value_t> origin_lrf(3);
            value_t_matrix rotation_matrix_lrf;           
            matrix::init_matrix<value_t>(3,3,rotation_matrix_lrf);

            define_lrf_origin_and_rotation_matrix(xi_distance_unit,xi_angle_unit,xi_lrf_latitude,xi_lrf_longitude, origin_lrf, rotation_matrix_lrf);

            transform_lrf2geo(xi_distance_unit, xi_angle_unit, origin_lrf, rotation_matrix_lrf, xi_x, xi_y, xi_z, xo_latitude, xo_longitude, xo_altitude);
        }

        static void transform_lrf2geo(edistanceunit xi_distance_unit,
                                      eangleunit xi_angle_unit,
                                      const std::vector<value_t>& xi_origin, 
                                      const value_t_matrix& xi_rotation_matrix,                                      
                                      value_t xi_x,
                                      value_t xi_y,
                                      value_t xi_z,
                                      value_t& xo_latitude, 
                                      value_t& xo_longitude,
                                      value_t& xo_altitude)
        {
            value_t x_ecf, y_ecf, z_ecf;
            transform_lrf2ecf(xi_origin, xi_rotation_matrix, xi_x, xi_y, xi_z, x_ecf, y_ecf, z_ecf);

            transform_ecef2geo(xi_distance_unit,xi_angle_unit, x_ecf, y_ecf, z_ecf, xo_latitude, xo_longitude, xo_altitude);
        }

        static void transform_lrf2ecf(const std::vector<value_t>& origin, 
                                      const value_t_matrix& rotation_matrix,                                      
                                      value_t xi_x, 
                                      value_t xi_y, 
                                      value_t xi_z, 
                                      value_t& xo_x_ecf, 
                                      value_t& xo_y_ecf, 
                                      value_t& xo_z_ecf)
        {
            std::vector<value_t> x(3);            
            x[0] = xi_x;
            x[1] = xi_y;
            x[2] = xi_z;
		    matrix::subtract_from_vector<value_t>(origin, x);

            value_t_matrix rotation_matrix_transpose;
            matrix::init_matrix<value_t>(3,3,rotation_matrix_transpose);
		    matrix::matrix_transpose<value_t>(rotation_matrix, rotation_matrix_transpose);

            std::vector<value_t> x_new(3);
            
		    matrix::matrix_vector_product<value_t>(rotation_matrix_transpose, x, x_new);
            xo_x_ecf = x_new[0];
            xo_y_ecf = x_new[1];
            xo_z_ecf = x_new[2];
        }

        static void transform_lrf1_to_lrf2(edistanceunit xi_distance_unit,
                                           eangleunit xi_angle_unit,
                                           value_t xi_lrf1_latitude,
                                           value_t xi_lrf1_longitude,
                                           value_t xi_lrf1_altitude,
                                           value_t xi_lrf2_latitude,
                                           value_t xi_lrf2_longitude,
                                           value_t xi_lrf2_altitude,
                                           value_t xi_x1, 
                                           value_t xi_y1, 
                                           value_t xi_z1, 
                                           value_t& xo_x, 
                                           value_t& xo_y, 
                                           value_t& xo_z)
        {
            if ((xi_lrf1_latitude == xi_lrf2_latitude) && (xi_lrf1_longitude == xi_lrf2_longitude) && (xi_lrf1_altitude == xi_lrf2_altitude))
	        {
		        xo_x = xi_x1;
                xo_y = xi_y1;
                xo_z = xi_z1;
	        }
	        else
	        {		        
                std::vector<value_t> x(3);                

                std::vector<value_t> origin_lrf1(3);
                value_t_matrix rotation_matrix_lrf1;           
                matrix::init_matrix<value_t>(3,3,rotation_matrix_lrf1);

                define_lrf_origin_and_rotation_matrix(xi_distance_unit,xi_angle_unit,xi_lrf1_latitude,xi_lrf1_longitude, origin_lrf1, rotation_matrix_lrf1);

                std::vector<value_t> origin_lrf2(3);
                value_t_matrix rotation_matrix_lrf2;           
                matrix::init_matrix<value_t>(3,3,rotation_matrix_lrf2);

                define_lrf_origin_and_rotation_matrix(xi_distance_unit,xi_angle_unit,xi_lrf2_latitude,xi_lrf2_longitude, origin_lrf2, rotation_matrix_lrf2);
                
		        transform_lrf1_to_lrf2(origin_lrf1, rotation_matrix_lrf1, origin_lrf2, rotation_matrix_lrf2, xi_x1, xi_y1, xi_z1, xo_x, xo_y, xo_z);
                		        
	        }
        }

        static void transform_lrf1_to_lrf2(const std::vector<value_t>& origin1, 
                                           const value_t_matrix& rotation_matrix1, 
                                           const std::vector<value_t>& origin2, 
                                           const value_t_matrix& rotation_matrix2, 
                                           value_t xi_x1, 
                                           value_t xi_y1, 
                                           value_t xi_z1, 
                                           value_t& xo_x, 
                                           value_t& xo_y, 
                                           value_t& xo_z)
        {
            value_t_matrix rotation_matrix_comb;
            matrix::init_matrix<value_t>(3,3,rotation_matrix_comb);

            matrix::matrix_matrix_transpose_product<value_t>(rotation_matrix2, rotation_matrix1, rotation_matrix_comb);
            std::vector<value_t> origin_comb(3);
	        matrix::matrix_vector_product<value_t>(rotation_matrix_comb, origin1, origin_comb);
	        matrix::subtract_vector<value_t>(origin2, origin_comb, origin_comb);

            std::vector<value_t> x1(3), x2(3);            
            x1[0] = xi_x1;
            x1[1] = xi_y1;
            x1[2] = xi_z1;
	        matrix::matrix_vector_product<value_t>(rotation_matrix_comb, x1, x2);
	        matrix::add_to_vector<value_t>(origin_comb, x2);
            xo_x = x2[0];
            xo_y = x2[1];
            xo_z = x2[2];
        }

        static void transform_covariance_lrf1_to_lrf2(edistanceunit xi_distance_unit,
                                           eangleunit xi_angle_unit,
                                           value_t xi_lrf1_latitude,
                                           value_t xi_lrf1_longitude,
                                           value_t xi_lrf1_altitude,
                                           value_t xi_lrf2_latitude,
                                           value_t xi_lrf2_longitude,
                                           value_t xi_lrf2_altitude,
                                           const value_t_matrix& xi_P,
                                           value_t_matrix& xo_P)
        {
            if ((xi_lrf1_latitude == xi_lrf2_latitude) && (xi_lrf1_longitude == xi_lrf2_longitude) && (xi_lrf1_altitude == xi_lrf2_altitude))
	        {
                matrix::init_matrix<value_t>(3,3,xo_P);
		        matrix::copy_matrix<value_t>(xi_P, xo_P);
	        }
	        else
	        {		        
                std::vector<value_t> x(3);                

                std::vector<value_t> origin_lrf1(3);
                value_t_matrix rotation_matrix_lrf1;           
                matrix::init_matrix<value_t>(3,3,rotation_matrix_lrf1);

                define_lrf_origin_and_rotation_matrix(xi_distance_unit,xi_angle_unit,xi_lrf1_latitude,xi_lrf1_longitude, origin_lrf1, rotation_matrix_lrf1);

                std::vector<value_t> origin_lrf2(3);
                value_t_matrix rotation_matrix_lrf2;           
                matrix::init_matrix<value_t>(3,3,rotation_matrix_lrf2);

                define_lrf_origin_and_rotation_matrix(xi_distance_unit,xi_angle_unit,xi_lrf2_latitude,xi_lrf2_longitude, origin_lrf2, rotation_matrix_lrf2);
                
		        transform_covariance_lrf1_to_lrf2(origin_lrf1, rotation_matrix_lrf1, origin_lrf2, rotation_matrix_lrf2, xi_P, xo_P);
                		        
	        }
        }

        static void transform_covariance_lrf1_to_lrf2(const std::vector<value_t>& origin1, 
                                           const value_t_matrix& rotation_matrix1, 
                                           const std::vector<value_t>& origin2, 
                                           const value_t_matrix& rotation_matrix2, 
                                           const value_t_matrix& xi_P,
                                           value_t_matrix& xo_P)
        {
            value_t_matrix rotation_matrix_comb;
            matrix::init_matrix<value_t>(3,3,rotation_matrix_comb);

            matrix::matrix_matrix_transpose_product<value_t>(rotation_matrix2, rotation_matrix1, rotation_matrix_comb);
                    
            matrix::init_matrix<value_t>(3,3,xo_P);
            matrix::weighted_matrix_outer_product<value_t>(rotation_matrix_comb, xi_P, xo_P);
        }

        static void transform_range_azimuth_altitude_measurement_to_slcf(edistanceunit xi_distance_unit,
                                                                     eangleunit xi_angle_unit,
                                                                     value_t xi_sensor_latitude,
                                                                     value_t xi_sensor_longitude,
                                                                     value_t xi_sensor_altitude,
                                                                     value_t xi_range,
                                                                     value_t xi_azimuth,
                                                                     value_t xi_altitude,
                                                                     value_t xi_var_range,
                                                                     value_t xi_var_azimuth,
                                                                     value_t xi_var_altitude,
                                                                     value_t& xo_x,
                                                                     value_t& xo_y,
                                                                     value_t& xo_z,
                                                                     value_t& xo_var_x,
                                                                     value_t& xo_var_y,
                                                                     value_t& xo_var_z,
                                                                     value_t& xo_cov_xy,
                                                                     value_t& xo_cov_xz,
                                                                     value_t& xo_cov_yz)                                                                     
        {
	        value_t v, r, rs, hs, z_rho, z_theta, z_h, x, y, z, dist_unit_conv_factor;
	        value_t_matrix R, H, con_R;
            matrix::init_matrix<value_t>(3,3,R);
            matrix::init_matrix<value_t>(3,3,H);
            matrix::init_matrix<value_t>(3,3,con_R);
            	        
	        hs = unitconverter::convert_distance_unit(xi_sensor_altitude, xi_distance_unit, edistanceunit_km);
	        z_rho = unitconverter::convert_distance_unit(xi_range, xi_distance_unit, edistanceunit_km);
	        z_theta = unitconverter::convert_angle_unit(xi_azimuth, xi_angle_unit, eangleunit_rad);
	        z_h = unitconverter::convert_distance_unit(xi_altitude, xi_distance_unit, edistanceunit_km);

	        rs = (value_t)(EARTH_RADIUS * sqrt(1.0 - pow(EARTH_ECCENTRICITY,2)) / ((value_t)1.0 - pow(EARTH_ECCENTRICITY * sin(unitconverter::convert_angle_unit(xi_sensor_latitude, xi_angle_unit, eangleunit_rad)),2)));

	        v = (value_t)((1.0 - hs / (rs + hs)) * z_h - (1.0 - 0.5 * hs / (rs + hs)) * hs - 0.5 * (pow(z_rho,2) - pow(z_h,2)) / (rs + hs));
	        //v = (z_h + z_rho + hs)*(z_h - z_rho - hs)/(2*(rs+hs)); %NEED TO CHECK

	        r = pow(z_rho,2) - pow(v,2);
	        r = (std::max)(r, (value_t)FLT_EPS);
	        r = sqrt(r);

	        x = r * cos(z_theta);		//sin(z_theta);
	        y = r * sin(z_theta);		//cos(z_theta);
	        z = v + hs;

	        xo_x = unitconverter::convert_distance_unit(x, edistanceunit_km, xi_distance_unit);
	        xo_y = unitconverter::convert_distance_unit(y, edistanceunit_km, xi_distance_unit);
	        xo_z = unitconverter::convert_distance_unit(z, edistanceunit_km, xi_distance_unit);
            	       
	        dist_unit_conv_factor = unitconverter::convert_distance_unit(1.0, xi_distance_unit, edistanceunit_km);
	        R[0][0] = pow(dist_unit_conv_factor,2) * xi_var_range;
	        R[1][1] = xi_var_azimuth;
	        R[2][2] = pow(dist_unit_conv_factor,2) * xi_var_altitude;

	        H[0][0] = (1 + v / (rs + hs)) * (z_rho * x) / (pow(z_rho,2) - pow(v,2));
	        H[0][1] = -y;				//y;
	        H[0][2] = (value_t)(-(1.0 + (z_h - hs) / (rs + hs)) * v * x / pow(r,2));	//-((rs+v)/(rs+hs))*((z_rho*x)/(SQR(z_rho) - SQR(v)));
	        H[1][0] = (1 + v / (rs + hs)) * (z_rho * y) / (pow(z_rho,2) - pow(v,2));
	        H[1][1] = x;				//-x;
	        H[1][2] = (value_t)(-(1.0 + (z_h - hs) / (rs + hs)) * v * y / pow(r,2));	//-((rs+v)/(rs+hs))*((z_rho*y)/(SQR(z_rho) - SQR(v)));
	        H[2][0] = -(z_rho / (rs + hs));
	        H[2][1] = 0;
	        H[2][2] = (value_t)(1.0 + (z_h - hs) / (rs + hs));	//(rs + v) / (rs + hs);

            matrix::weighted_matrix_outer_product(H, R, con_R);	        
	        matrix::scale_matrix(pow(1 / dist_unit_conv_factor,2), con_R);

	        xo_var_x = con_R[0][0];
	        xo_var_y = con_R[1][1];
	        xo_var_z = con_R[2][2];
	        xo_cov_xy = con_R[0][1];
	        xo_cov_xz = con_R[0][2];
	        xo_cov_yz = con_R[1][2];
        }

};

UISOFT_END_NAMESPACE

#endif // __COORDINATECONVERTER_H__
