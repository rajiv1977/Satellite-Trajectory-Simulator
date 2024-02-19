/*
 * Satellite Trajectory Simulator
 *
 * Authors:  Rajiv Sithiravel and Thiagalingam Kirubarajan
 *
 * Contact:  	McMaster University
 * Hamilton, Ontario,
 * Canada.
 * sithirr@mcmaster.ca and kiruba@mcmaster.ca
 */
#ifndef ORBIT_HPP
#define ORBIT_HPP
#define _ISOC99_SOURCE

#include <fstream>
#include <iostream>
#include <ctype.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

/* constants used by SGP4/SDP4 code
 * /* direct imported from FORTRAN*/
#define deg2rad 	1.745329251994330E-2     	/* degrees to radians */
#define rad2deg	    57.32484076433121;			/* radian to degrees*/
#define pi      	3.14159265358979323846   	/* PI */
#define pio2		1.57079632679489656			/* PI/2 */
#define twopi		6.28318530717958623 		/* 2*PI */
#define x3pio2 		4.71238898038468967         /* 3*PI/2 */
#define e6a 		1.0E-6
#define tothrd 		6.6666666666666666E-1 		/* 2/3 */
#define xj2			1.0826158E-3				/* J2 harmonic */
#define xj3			-2.53881E-6					/* J3 harmonic */
#define xj4			-1.65597E-6					/* J4 harmonic */
#define xke 		7.43669161E-2
#define xkmper 		6.378137E3					/* Earth radius km */
#define xmnpda 		1.44E3						/* minutes per day */
#define ae          1.0
#define ck2			5.413079E-4
#define ck4			6.209887E-7
//#define f			3.35281066474748E-3			/* flattening factor */           may need this for SP4 model
#define ge   		3.986008E5					/* Earth grav. constant */
#define ss_ 		1.012229
#define qoms2t 		1.880279E-09
#define secday 		8.6400E4					/* seconds per day */
#define omega_E 	1.00273790934 				/* Earth rot./sid. day */
#define omega_ER 	6.3003879					/* Earth rot. rads/sid */
#define zns 		1.19459E-5
#define c1ss 		2.9864797E-6
#define zes 		1.675E-2
#define znl 		1.5835218E-4
#define c1l 		4.7968065E-7
#define zel 		5.490E-2
#define zcosis 		9.1744867E-1
#define zsinis 		3.9785416E-1
#define zsings 		-9.8088458E-1
#define zcosgs 		1.945905E-1
#define zcoshs 		1
#define zsinhs 		0
#define q22 		1.7891679E-6
#define q31 		2.1460748E-6
#define q33 		2.2123015E-7
#define g22 		5.7686396
#define g32 		9.5240898E-1
#define g44 		1.8014998
#define g52 		1.0508330
#define g54 		4.4108898
#define root22 		1.7891679E-6
#define root32 		3.7393792E-7
#define root44 		7.3636953E-9
#define root52 		1.1428639E-7
#define root54 		2.1765803E-9
#define thdt		4.3752691E-3
#define rho			1.5696615E-1
#define sr			6.96000E5				/* solar radius */

/* entry points of DEEP for SDP4*/
#define dpinit 1 	/* Deep-space initialization code */
#define dpsec 2 	/* Deep-space secular code */
#define dpper 3 	/* Deep-space periodic code*/

/* the flow is controlled by flags, to displace the GO TO in FORTRAN */
#define ALL_FLAGS               -1
#define SGP4_INITIALIZED_FLAG 	0x000002
#define SDP4_INITIALIZED_FLAG 	0x000004
#define SIMPLE_FLAG 			0x000040
#define DEEP_SPACE_EPHEM_FLAG 	0x000020
#define LUNAR_TERMS_DONE_FLAG 	0x000080
#define DO_LOOP_FLAG 			0x000200
#define RESONANCE_FLAG 			0x000400
#define SYNCHRONOUS_FLAG 		0x000800
#define EPOCH_RESTART_FLAG 		0x001000

// Two-line-element satellite orbital data structure //
typedef struct {
    double epoch, xndt2o, xndd6o, bstar, xincl,
            xnodeo, eo, omegao, xmo, xno;
    int catnr, elset, revnum;
    char sat_name[25], idesg[9];
} tle_t;

// geodetic position structure used by SGP4/SDP4 code. //
typedef struct {
    double lat, lon, alt, theta;
} geodetic_t;

// general three-dimensional vector structure used by SGP4/SDP4 code.//
typedef struct {
    double x, y, z, w;
} vector_t;

// common arguments between deep-space functions used by SGP4/SDP4 code.//
typedef struct {
    // Used by dpinit part of DEEP //
    double eosq, sinio, cosio, betao, aodp, theta2,
            sing, cosg, betao2, xmdot, omgdot, xnodot, xnodp;
    
    // Used by dpsec and dpper parts of DEEP //
    double xll, omgadf, xnode, em, xinc, xn, t;
    
    // Used by thetg and DEEP //
    double ds50;
} deep_arg_t;

/* orbit function */
std::vector< std::pair<double, double> > orbit(double epoch,double xndt2o,double xndd6o,double bstar,double xincl,double xnodeo,double eo,double omegao,double xmo,double xno,double revnum,double Tsince);

/* FLAG functions */
int isFlagSet(int flag);
int isFlagClear(int flag);
void SetFlag(int flag);
void ClearFlag(int flag);

/* MATHEMATICAL functions */
int Sign(double arg);
double Sqr(double arg);
double Cube(double arg);
double Radians(double arg);
double Degrees(double arg);
double ArcSin(double arg);
double ArcCos(double arg);
void Magnitude(vector_t *v);
void Vec_Add(vector_t *v1, vector_t *v2, vector_t *v3);
void Vec_Sub(vector_t *v1, vector_t *v2, vector_t *v3);
void Scalar_Multiply(double k, vector_t *v1, vector_t *v2);
void Scale_Vector(double k, vector_t *v);
double Dot(vector_t *v1, vector_t *v2);
double Angle(vector_t *v1, vector_t *v2);
void Cross(vector_t *v1, vector_t *v2 ,vector_t *v3);
void Normalize(vector_t *v);
double AcTan(double sinx, double cosx);
double FMod2p(double x);
double FixAngle(double x);
double Modulus(double arg1, double arg2);
double Frac(double arg);
int Round(double arg);
double Int(double arg);

int Sat_Eclipsed(vector_t *pos, vector_t *sol, double *depth);

/* time conversion */
void Date_Time(double julian_date, struct tm *cdate);
double Julian_Date(struct tm *cdate);
double Fraction_of_Day(int hr, int mi, double se);
int DOY (int yr, int mo, int dy);
double Julian_Date_of_Epoch(double epoch);
double Julian_Date_of_Year(double year);
double ThetaG_JD(double jd);
double ThetaG(double epoch, deep_arg_t *deep_arg);

/* SGP4/SDP4 functions */
void select_ephemeris(tle_t *tle);
std::vector< std::pair<double, double> > SGP4(double tsince, tle_t * tle);
void Deep(int ientry, tle_t * tle, deep_arg_t * deep_arg);
std::vector< std::pair<double, double> >  SDP4(double tsince, tle_t * tle);

#endif