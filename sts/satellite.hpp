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
#ifndef SATELLITE_HPP
#define SATELLITE_HPP

#include <vector>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <iomanip>
#include <locale>
#include <cstring>
#include <math.h>
#include <direct.h>
#include <algorithm>
#include <ctype.h>
#include <cstdlib>
#include <xstring>
#include <time.h>
#include <climits>
#include <iomanip>
#include <limits>
#include "orbit.hpp"
#include <vector>
#include <iterator>
#include <string>
#include "xmlreader_light.hpp"
#include "pugixml.hpp"

static int cumdays[] = {0, 0,31,59,90,120,151,181,212,243,273,304,334};
#define MAX_STRING_SIZE							32767
#define MAX_SIZE								627670
#define MAX_STRING_SIZE_256						256
#define MAX_STRING_SIZE_128						128
#define MAX_STRING_SIZE_16						16
#define MAX_ARRAY_SIZE							10000
#define GetCurrentDir							_getcwd
#define  BUFSIZE								256
#define MALLOC_MINIMUM_DIMENSION				5
#define MAX_STRING_SIZE_256						256
#define MAX_STRING_SIZE_128						128
#define MAX_STRING_SIZE_16						16
#define MAX_ARRAY_SIZE							10000
#define MAX_STACK								100
#define ARRAY_INDEX_START						0
#define NF_INFINITY								(DBL_MAX * 2)
#define NF_NAN									(NF_INFINITY * 0)
#define _isinf(x)								(!_finite(x))
#define _isfinite(x)							_finite(x)
#define FREE_DOUBLE_MATRIX(M) if (M) free_DoubleMatrix(&M)
#define MACHINE_EPSILON_SINGLE              	1.1920929e-007
#define MACHINE_EPSILON_DOUBLE              	2.220446049250313e-016
#define MACHINE_EPSILON                     	MACHINE_EPSILON_DOUBLE
#define REALMAX									1.797693134862316e+307
#define REALMIN									-1.797693134862316e+307
#define INTMIN									-2147483648
#define INTMAX									-2147483648
#define INF										NF_INFINITY
#define TRUE									1
#define FALSE									0
#define NF_PI									3.1415926535897

/* Mersenne Twister macros*/
#define NF_NP 									624
#define NF_MP 									397
#define MATRIX_A 								0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 								0x80000000UL   /* most significant w-r bits */
#define LOWER_MASK 								0x7fffffffUL   /* least significant r bits */

#define SQR(var) ((var)*(var))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define DEG2RAD(x) (3.1415926535897*(x)/180.0)
#define RAD2DEG(x) ((x)*180.0/3.1415926535897)

typedef unsigned char byte;
typedef unsigned int NF_BOOL;
typedef NF_BOOL* BOOLVector;
typedef NF_BOOL** BOOLMatrix;
typedef int* IntVector;
typedef int** IntMatrix;
typedef int*** IntTensor;
typedef double* DoubleVector;
typedef double** DoubleMatrix;
typedef double*** DoubleTensor;
typedef FILE*  filep;
typedef FILE** FpVector;
typedef unsigned int UINT;
typedef char* String;
typedef char** StringVector;
typedef int ReturnCode;
typedef void* void_ptr;
typedef void* void_ptr2;

typedef struct  SatsInfo_Type
{
    String  name_of_satellite;
    String type_of_sat;
    String  source_name;
    String  constel_name;
    String  type_constel;
    String  tle_line_1;
    String  tle_line_2;
    double epoch_year;
    double day_of_year;
    double ndt;
    double nd2t;
    double bstar;
    double inclination;
    double raan;
    double eccentricity;
    double argument_of_perigee;
    double mean_anomaly;
    double mean_motion;
    double revolution_number;
    double year;
    double month;
    double day;
    double hour;
    double min;
    double sec;
} SatsInfo_Type;
typedef  SatsInfo_Type** SatsInfo_TypeDblPtr;
typedef  SatsInfo_Type* SatsInfo_TypePtr;
typedef  SatsInfo_TypeDblPtr SatsInfo_Type_Matrix;
typedef  SatsInfo_TypePtr SatsInfo_Type_Vector;

typedef struct _SatsInfo_TypeMatrix
{
    int m;
    int n;
    SatsInfo_TypeDblPtr data;
} _SatsInfo_TypeMatrix;
typedef struct  _SatsInfo_TypeMatrix* SatsInfo_TypeMatrix;

class doubleMatrix {
public:
    DoubleMatrix data;
    int m;
    int n;
    int mmax;
    int nmax;
    
    //constructrs
    doubleMatrix(void);
    doubleMatrix(int m, int n);
    doubleMatrix(int m, int n, double val);
    doubleMatrix(const doubleMatrix& x);
    ~doubleMatrix();
    
    //memory
    void malloc(int m, int n);
    void malloc(int m, int n, double val);
    void resize(int m, int n);
    void resize(int m, int n, double val);
    void copy(const doubleMatrix &B);
    void free(void);
    int isempty(void) const;
    
    //// operators ////
    //scalar double
    doubleMatrix operator | (double B);
    doubleMatrix operator & (double B);
    doubleMatrix operator < (double B);
    doubleMatrix operator > (double B);
    doubleMatrix operator <= (double B);
    doubleMatrix operator >= (double B);
    doubleMatrix operator != (double B);
    doubleMatrix operator == (double B);
    doubleMatrix operator + (double B);
    doubleMatrix operator - (double B);
    doubleMatrix operator / (double B);
    doubleMatrix operator * (double B);
        
    //matrix double
    doubleMatrix operator | (const doubleMatrix &B);
    doubleMatrix operator & (const doubleMatrix &B);
    doubleMatrix operator < (const doubleMatrix &B);
    doubleMatrix operator > (const doubleMatrix &B);
    doubleMatrix operator <= (const doubleMatrix &B);
    doubleMatrix operator >= (const doubleMatrix &B);
    doubleMatrix operator != (const doubleMatrix &B);
    doubleMatrix operator == (const doubleMatrix &B);
    doubleMatrix& operator = (const doubleMatrix &B);
    doubleMatrix operator + (const doubleMatrix &B);
    doubleMatrix operator - (const doubleMatrix &B);
    doubleMatrix operator / (const doubleMatrix &B);
    doubleMatrix operator * (const doubleMatrix &B);
    
};

typedef struct  States_Type
{
    doubleMatrix			position;
    doubleMatrix			velocity;
    doubleMatrix			ctime;
    String					SatName;
	double					lat;
	double					lon;
	double					radii;
} States_Type;
typedef  States_Type** States_TypeDblPtr;
typedef  States_Type* States_TypePtr;
typedef  States_TypeDblPtr States_Type_Matrix;
typedef  States_TypePtr States_Type_Vector;
typedef struct _States_TypeMatrix
{
    int						m;
    int						n;
    States_TypeDblPtr		data;
} _States_TypeMatrix;
typedef struct  _States_TypeMatrix* States_TypeMatrix;


States_TypeMatrix			malloc_States_TypeMatrix(int m, int n);
States_Type_Matrix			malloc_States_Type_Matrix(int m, int n);
DoubleMatrix				alloc_DoubleMatrix_val(int m, int n, double val);
DoubleMatrix				alloc_DoubleMatrix(int m, int n);

class sat_info
{   
public:
    std::vector<double>		tle;
    std::string				name;
    std::vector<double>		epochC;
    double					epochN;
};

class satellite
{  
public:
    SatsInfo_TypeMatrix		specified_sat_info;
	SatsInfo_TypeMatrix		specified_sat_info_direct;
    int						fC;

																	satellite();
    sat_info														norad2kep(char *file,char *name);
    double															NF_fix_s(double x);
    double															julian (double month, double day, double year);
    double															datenum(double year, double mon, double day, double hour, double imin, double sec, double mil);
    NF_BOOL															NF_isleapyear (int year);
    double															NF_eomday(double month, double year);
    std::vector< double >											NF_datevec(double serialdate);
    std::vector< std::pair<double, double> >						SatPOS(std::vector< double> ctime, sat_info sat);
    SatsInfo_Type_Matrix											malloc_SatsInfo_Type_Matrix(int m, int n);
    SatsInfo_TypeMatrix												malloc_SatsInfo_TypeMatrix(int m, int n);
    String															alloc_String(int n);
    void															sat_config(std::string config_filename);
    States_TypeMatrix												GetStates(std::vector<double> ctime);
    sat_info														tle2kep(char *cline2, char *cline3);
    void															SatPOS_Wrapper(int i, std::vector< double> ctime, sat_info satInfo, States_TypeMatrix states);
    sat_info														new2kep(int i, SatsInfo_TypeMatrix specified_sat_info);
    SatsInfo_TypeMatrix												resize_SatsInfoTypeMatrix(SatsInfo_TypeMatrix old, int m, int n);
    std::string														find_fullPath(std::string ginen_Name);
    void															printf_states(States_TypeMatrix states);  
    const std::string												currentDateTime(); 
    int																EconstlNum(std::string source_name, std::string name_of_satellite);
    int																norad2kep_constel(const char *file, const char *name);
	States_TypeMatrix												GetState(std::string name_of_satellite, std::vector< double> ctime);
	void															SatPOS_Wrapper_footPrint(int i, std::vector< double> ctime, sat_info satInfo, States_TypeMatrix states, double coverage);
	void															SatSingPOS_Wrapper(int i, std::vector< double> ctime, sat_info satInfo, States_TypeMatrix states);
	std::vector< std::pair<std::pair<double, double>, double> >		SatPOS_footPrint(std::vector< double> ctime, sat_info sat, double coverage);
	void															SatSingPOS_Wrapper_footPrint(int i, std::vector< double> ctime, sat_info satInfo, States_TypeMatrix states, double coverage);
	std::vector< std::pair<double, double> >						latlon(std::vector< std::pair<double, double> > pos, double ctime);
	double															actan2(double x, double y);
	double															thetag(double jd);
	double															mod_(double arg1, double arg2);
	States_TypeMatrix												footPrint(std::string name_of_satellite, std::vector< double> ctime, double coverage);
	States_TypeMatrix												footPrints(std::vector< double> ctime, double coverage);
	void															print_footPrint(States_TypeMatrix states);
	States_TypeMatrix												GetState_Fprint(std::string name_of_satellite, std::vector< double> ctime, double coverage);
	States_TypeMatrix												GetState_Fprints(std::vector<double> ctime, double coverage);
	double															footP_radius(std::vector< std::pair<double, double> >  states, double lat, double lon, double coverage);
	std::vector<double>												ecef2lla(double x, double y, double z);  
};

#endif
