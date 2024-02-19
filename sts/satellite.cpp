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
#include "satellite.hpp"

satellite::satellite()
{
    return;
}

// constructor
sat_info satellite::norad2kep(char* file, char* name)
{
    char  cline1[100];
    char  cline2[100];
    char  cline3[100];
    char  holder[100];
    char *str1, *str2;

    double      yr, dayofyear, epochN;
    std::string status;

    FILE* pFile;
    pFile = fopen(file, "r");
    if (pFile == NULL)
    {
        perror("Error opening file");
    }
    else
    {
        status = "false";
        while (!feof(pFile) && (status.compare("true") != 0))
        {
            fgets(cline1, MAX_STRING_SIZE, pFile);
            fgets(cline2, MAX_STRING_SIZE, pFile);
            fgets(cline3, MAX_STRING_SIZE, pFile);
            str1 = strlwr(cline1);
            strcpy(holder, name);
            str2 = strlwr(holder);

            if (strstr(str1, str2) != NULL)
            {
                status = "true";
            }
        }
    }

    if (status.compare("true") != 0)
    {
        perror("satellite with the name given is not in the list");
    }
    fclose(pFile);
    std::cout.precision(10);

    // epoch year (2 digits)
    // yr = str2num(cline2(19:20)); matlab
    char buffer1[3];
    buffer1[0] = cline2[18];
    buffer1[1] = cline2[19];
    buffer1[2] = '\0';
    yr         = atof(buffer1);

    // epoch day fraction
    // dayofyear = str2num(cline2(21:32));  matlab
    char buffer2[14];
    buffer2[0]  = cline2[20];
    buffer2[1]  = cline2[21];
    buffer2[2]  = cline2[22];
    buffer2[3]  = cline2[23];
    buffer2[4]  = cline2[24];
    buffer2[5]  = cline2[25];
    buffer2[6]  = cline2[26];
    buffer2[7]  = cline2[27];
    buffer2[8]  = cline2[28];
    buffer2[9]  = cline2[29];
    buffer2[10] = cline2[30];
    buffer2[11] = cline2[31];
    buffer2[12] = cline2[32];
    buffer2[13] = '\0';
    dayofyear   = atof(buffer2);

    // NORAD specification
    // epochN=(1e3.*yr)+ dayofyear;  matlab
    epochN = (1000 * yr) + dayofyear;

    // time conversion
    // if (yr < 57) yr = 2000 + yr; else yr = 1900 + yr; end matlab
    if (yr < 57)
    {
        yr = 2000 + yr;
    }
    else
    {
        yr = 1900 + yr;
    }

    double xjdtmp = julian(1, 0, yr);

    // Julian date of the epoch
    double xjdtle = xjdtmp + dayofyear;

    // d/dt of mean motin
    // ndt = str2num(cline2(34:43)); matlab
    char buffer3[14];
    buffer3[0]  = cline2[33];
    buffer3[1]  = cline2[34];
    buffer3[2]  = cline2[35];
    buffer3[3]  = cline2[36];
    buffer3[4]  = cline2[37];
    buffer3[5]  = cline2[38];
    buffer3[6]  = cline2[39];
    buffer3[7]  = cline2[40];
    buffer3[8]  = cline2[41];
    buffer3[9]  = cline2[42];
    buffer3[10] = cline2[43];
    buffer3[11] = cline2[44];
    buffer3[12] = cline2[45];
    buffer3[13] = '\0';
    double ndt  = atof(buffer3);

    // d2/dt2 of mean motin
    // nd2t = 1.0e-5.*str2num(cline2(45:50)).*(10^str2num(cline2(51:52)));
    char buffer4[7];
    buffer4[0] = cline2[44];
    buffer4[1] = cline2[45];
    buffer4[2] = cline2[46];
    buffer4[3] = cline2[47];
    buffer4[4] = cline2[48];
    buffer4[5] = cline2[49];
    buffer4[6] = '\0';
    char buffer5[3];
    buffer5[0]  = cline2[50];
    buffer5[1]  = cline2[51];
    buffer5[2]  = '\0';
    double nd2t = 0.00001 * atof(buffer4) * pow(10, atof(buffer5));

    // bstar drag term
    // bstar = 1.0e-5.*str2num(cline2(54:59)).*(10^str2num(cline2(60:61)))./ae;
    char buffer6[7];
    buffer6[0] = cline2[53];
    buffer6[1] = cline2[54];
    buffer6[2] = cline2[55];
    buffer6[3] = cline2[56];
    buffer6[4] = cline2[57];
    buffer6[5] = cline2[58];
    buffer6[6] = '\0';
    char buffer7[3];
    buffer7[0]   = cline2[59];
    buffer7[1]   = cline2[60];
    buffer7[2]   = '\0';
    double bstar = (0.00001 * atof(buffer6) * pow(10, atof(buffer7))) / ae;

    // orbit inclination
    // incl = str2num(cline3(9:16));
    char buffer8[9];
    buffer8[0]  = cline3[8];
    buffer8[1]  = cline3[9];
    buffer8[2]  = cline3[10];
    buffer8[3]  = cline3[11];
    buffer8[4]  = cline3[12];
    buffer8[5]  = cline3[13];
    buffer8[6]  = cline3[14];
    buffer8[7]  = cline3[15];
    buffer8[8]  = '\0';
    double incl = atof(buffer8);

    // right ascension of ascending node
    // raan = str2num(cline3(18:25));
    char buffer9[9];
    buffer9[0]  = cline3[17];
    buffer9[1]  = cline3[18];
    buffer9[2]  = cline3[19];
    buffer9[3]  = cline3[20];
    buffer9[4]  = cline3[21];
    buffer9[5]  = cline3[22];
    buffer9[6]  = cline3[23];
    buffer9[7]  = cline3[24];
    buffer9[8]  = '\0';
    double raan = atof(buffer9);

    // eccentricity
    // ecc = 1.0e-7.*str2num(cline3(27:33));
    char buffer10[8];
    buffer10[0] = cline3[26];
    buffer10[1] = cline3[27];
    buffer10[2] = cline3[28];
    buffer10[3] = cline3[29];
    buffer10[4] = cline3[30];
    buffer10[5] = cline3[31];
    buffer10[6] = cline3[32];
    buffer10[7] = '\0';
    double ecc  = 0.0000001 * atof(buffer10);

    // argument of perigee
    // omegao = str2num(cline3(35:42));
    char buffer11[9];
    buffer11[0]   = cline3[34];
    buffer11[1]   = cline3[35];
    buffer11[2]   = cline3[36];
    buffer11[3]   = cline3[37];
    buffer11[4]   = cline3[38];
    buffer11[5]   = cline3[39];
    buffer11[6]   = cline3[40];
    buffer11[7]   = cline3[41];
    buffer11[8]   = '\0';
    double omegao = atof(buffer11);

    // mean anomaly
    // mo = str2num(cline3(44:51));
    char buffer12[9];
    buffer12[0] = cline3[43];
    buffer12[1] = cline3[44];
    buffer12[2] = cline3[45];
    buffer12[3] = cline3[46];
    buffer12[4] = cline3[47];
    buffer12[5] = cline3[48];
    buffer12[6] = cline3[49];
    buffer12[7] = cline3[50];
    buffer12[8] = '\0';
    double mo   = atof(buffer12);

    // mean motion
    // no = str2num(cline3(53:63));
    char buffer13[12];
    buffer13[0]  = cline3[52];
    buffer13[1]  = cline3[53];
    buffer13[2]  = cline3[54];
    buffer13[3]  = cline3[55];
    buffer13[4]  = cline3[56];
    buffer13[5]  = cline3[57];
    buffer13[6]  = cline3[58];
    buffer13[7]  = cline3[59];
    buffer13[8]  = cline3[60];
    buffer13[9]  = cline3[61];
    buffer13[10] = cline3[62];
    buffer13[11] = '\0';
    double no    = atof(buffer13);

    // revolution number
    // rev = str2num(cline3(64:68));
    char buffer14[6];
    buffer14[0] = cline3[63];
    buffer14[1] = cline3[64];
    buffer14[2] = cline3[65];
    buffer14[3] = cline3[66];
    buffer14[4] = cline3[67];
    buffer14[5] = '\0';
    double rev  = atof(buffer14);

    // epoch=datevec(datenum(yr, 0 , dayofyear));
    double currT;
    currT = datenum(yr, 0, dayofyear, 0, 0, 0, 0);
    std::vector<double> flag2(6);
    flag2 = NF_datevec(currT);
    sat_info satInfo;
    satInfo.epochC = flag2;

    // return of the struct
    // sat.name=deblank(upper(name));
    satInfo.name = strupr(holder);

    // orbit elements
    std::vector<double> tle(11);
    tle.at(0)  = xjdtle;
    tle.at(1)  = ndt;
    tle.at(2)  = nd2t;
    tle.at(3)  = bstar;
    tle.at(4)  = incl;
    tle.at(5)  = raan;
    tle.at(6)  = ecc;
    tle.at(7)  = omegao;
    tle.at(8)  = mo;
    tle.at(9)  = no;
    tle.at(10) = rev;
    satInfo.tle.reserve(11);
    satInfo.tle = tle;

    // epoch in calender date for
    // sat.epochC=epoch;
    // epoch in special date form
    satInfo.epochN = epochN;

    return satInfo;
}

double satellite::NF_fix_s(double x)
{
    if (x >= 0)
    {
        return floor(x); // round down
    }
    else
    {
        return ceil(x); // round up
    }
}

double satellite::julian(double month, double day, double year)
{
    double y, m, a, b, c, jd, jdate;
    y = year;
    m = month;
    b = 0;
    c = 0;

    if (m <= 2)
    {
        y = y - 1;
        m = m + 12;
    }

    if (y < 0)
    {
        c = -0.75;
    }

    // check for valid calendar date
    if (year < 1582)
    {
        // null
    }
    else if (year > 1582)
    {
        // a = fix(y / 100);
        a = NF_fix_s(y / 100);
        b = 2 - a + floor(a / 4);
    }
    else if (month < 10)
    {
        // null
    }
    else if (month > 10)
    {
        // a = fix(y / 100);
        a = NF_fix_s(y / 100);
        b = 2 - a + floor(a / 4);
    }
    else if (day <= 4)
    {
        // null
    }
    else if (day > 14)
    {
        // a = fix(y / 100);
        a = NF_fix_s(y / 100);
        b = 2 - a + floor(a / 4);
    }
    else
    {
        perror("this is an invalid calendar date");
        jdate = 0; // no information
        return jdate;
    }

    // calculate the Julian date
    // jd = fix(365.25 * y + c) + fix(30.6001 * (m + 1));
    jd    = NF_fix_s(365.25 * y + c) + NF_fix_s(30.6001 * (m + 1));
    jdate = jd + day + b + 1720994.5;
    return jdate;
}

double satellite::datenum(double year, double mon, double day, double hour, double imin, double sec, double mil)
{
    double tmp1;
    int    tmp2, tmp3;
    double tmp4, tmp5;
    double dNum;

    // Calculate the serial date number:
    tmp1 = 365 * year + cumdays[(int) mon] + day;
    tmp2 = year / 4 - year / 100 + year / 400;
    tmp3 = ((int) year % 4 != 0) - ((int) year % 100 != 0) + ((int) year % 400 != 0);
    tmp4 = (double) (tmp1 + tmp2 + tmp3);
    tmp5 = (hour * 3600000 + imin * 60000 + sec * 1000 + mil) / 86400000.0;

    dNum = tmp4 + tmp5;

    if (mon > 2)
    {
        if ((((int) year % 4 == 0) && ((int) year % 100 != 0)) || ((int) year % 400 == 0))
        {
            dNum += 1.0;
        }
    }

    return (dNum);
}

/* NF_isleapyear
 *
 * returns a 1 (TRUE) if its a leap year
 *
 */
NF_BOOL satellite::NF_isleapyear(int year)
{
    if (year == 0)
    {
        return TRUE;
    }
    return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

/* NF_eomday
 *
 * These functions return the number of days in a month. The year must be passed
 * to account for leap years. vetors are ok but keep in mind only one year at a time.
 * months start at 1 = jan, to 12 = dec.
 */
double satellite::NF_eomday(double month, double year)
{
    switch ((int) month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        case 2:
            return (28 + NF_isleapyear((int) year));
        default:
            return 0;
    }
}

/* NF_datevec
 *
 * Syntax
 * dm = NF_datevec(dm, sd, n)
 *
 * dm is a double matrix of (empty) date vectors. It is an mx6 matrix.
 * sd is a vector of serial dates. it is a nx1 matrix.
 * n is the numbre of vectors/dates to process. The serial dates in sd are
 * converted into date vectors.
 *
 * The reference day for serial dates is Jan 0, 0000
 */
std::vector<double> satellite::NF_datevec(double serialdate)
{

    std::vector<double> datevector(6);

    // estimate number of years without leap days
    datevector.at(0) = (double) ((int) serialdate / 365);

    // use [2] as a place for number of days:
    // first, subtract \4 days for all the leap years,
    // then add \100 back for the centuries with no leap year
    // then subtract \400 for the leap year every 4th century
    datevector.at(2) = (double) ((int) (serialdate + NF_isleapyear((int) datevector.at(0))) % 365);
    datevector.at(2) -= (double) ((int) datevector.at(0) / 4);
    datevector.at(2) += (double) ((int) datevector.at(0) / 100);
    datevector.at(2) -= (double) ((int) datevector.at(0) / 400);

    // now, while the number of days remains negative or zero,
    // carry the years backwards
    while (datevector.at(2) <= 0)
    {
        datevector.at(0) -= 1;
        datevector.at(2) += 365;
        datevector.at(2) += NF_isleapyear((int) datevector.at(0));
    }

    // at this point we have the correct year in [0] and the
    // correct number of days in [2]. count the months, while
    // using up the days

    datevector.at(1) = 1;
    while (datevector.at(2) > NF_eomday(datevector.at(1), datevector.at(0)))
    {
        datevector.at(2) -= NF_eomday(datevector.at(1), datevector.at(0));
        datevector.at(1)++;
    }
    datevector.at(2) = datevector.at(2) - 1;
    // now we should have the correct day in the first 3 fields,
    // so calculate the time from the remainder of the days
    datevector.at(3) = ((serialdate) - ((int) serialdate));
    datevector.at(3) *= 24;
    datevector.at(4) = ((datevector.at(3)) - ((int) datevector.at(3)));
    datevector.at(4) *= 60;
    datevector.at(5) = ((datevector.at(4)) - ((int) datevector.at(4)));
    datevector.at(5) *= 60;
    datevector.at(3) = (int) datevector.at(3);
    datevector.at(4) = (int) datevector.at(4);
    datevector.at(5) = (int) datevector.at(5);

    return datevector;
}

//[r    rdot]
std::vector<std::pair<double, double>> satellite::SatPOS(std::vector<double> ctime, sat_info sat)
{
    // given time convert into Julian date
    double a  = floor((14 - ctime.at(1)) / 12);
    double y  = ctime.at(0) + 4800 - a;
    double m  = ctime.at(1) + 12 * a - 3;
    double jd = ctime.at(2) + floor((153 * m + 2) / 5) + y * 365 + floor(y / 4) - floor(y / 100) + floor(y / 400) -
                32045 + (ctime.at(5) + 60 * ctime.at(4) + 3600 * (ctime.at(3) - 12)) / 86400;

    // time since epoch in minutes
    long double                            temp0  = jd;
    long double                            temp1  = sat.tle.at(0);
    long double                            temp2  = (temp0 - temp1);
    long double                            tsince = (temp2) * 1440;
    std::vector<std::pair<double, double>> sat_states;

    // orbit calculation by the MEX SGP4/SDP4 implementation
    sat_states = orbit(sat.epochN,
                       sat.tle.at(1),
                       sat.tle.at(2),
                       sat.tle.at(3),
                       sat.tle.at(4),
                       sat.tle.at(5),
                       sat.tle.at(6),
                       sat.tle.at(7),
                       sat.tle.at(8),
                       sat.tle.at(9),
                       sat.tle.at(10),
                       tsince);

    /* scale r and rdot
     * r [km] and rdot [km/s] */
    for (int ij = 0; ij < sat_states.size(); ij++)
    {
        sat_states.at(ij).first  = sat_states.at(ij).first * 6.378137e3;
        sat_states.at(ij).second = sat_states.at(ij).second * (6.378137e3 * 1.44e3 / 8.64e4);
    }
    return sat_states;
}

SatsInfo_TypeMatrix satellite::malloc_SatsInfo_TypeMatrix(int m, int n)
{
    SatsInfo_TypeMatrix S = NULL;
    S                     = (SatsInfo_TypeMatrix) malloc(sizeof(_SatsInfo_TypeMatrix));
    S->m                  = m;
    S->n                  = n;
    if (m == 0 || n == 0)
    {
        S->data = NULL;
    }
    else
    {
        S->data = (SatsInfo_TypeDblPtr) malloc_SatsInfo_Type_Matrix(m, n);
    }
    return S;
}

SatsInfo_Type_Matrix satellite::malloc_SatsInfo_Type_Matrix(int m, int n)
{
    int                  i;
    SatsInfo_Type_Matrix S = NULL;
    S                      = (SatsInfo_Type_Matrix) malloc((size_t) (m * sizeof(SatsInfo_TypePtr)));
    if (S)
    {
        if (m * n > 0)
        {
            S[0] = (SatsInfo_TypePtr) malloc((size_t) ((m * n) * sizeof(SatsInfo_Type)));
            if (!S[0])
            {
                return NULL;
            }
            for (i = 1; i < m; i++)
            {
                S[i] = S[i - 1] + n;
            }
        }
    }

    for (m--; m >= 0; m--)
    {
        i = n;
        for (i--; i >= 0; i--)
        {
            S[m][i].name_of_satellite   = NULL;
            S[m][i].type_of_sat         = NULL;
            S[m][i].source_name         = NULL;
            S[m][i].tle_line_1          = NULL;
            S[m][i].tle_line_2          = NULL;
            S[m][i].constel_name        = NULL;
            S[m][i].type_constel        = NULL;
            S[m][i].epoch_year          = 0.0;
            S[m][i].day_of_year         = 0.0;
            S[m][i].ndt                 = 0.0;
            S[m][i].nd2t                = 0.0;
            S[m][i].bstar               = 0.0;
            S[m][i].inclination         = 0.0;
            S[m][i].raan                = 0.0;
            S[m][i].eccentricity        = 0.0;
            S[m][i].argument_of_perigee = 0.0;
            S[m][i].mean_anomaly        = 0.0;
            S[m][i].mean_motion         = 0.0;
            S[m][i].revolution_number   = 0.0;
            S[m][i].year                = 0.0;
            S[m][i].month               = 0.0;
            S[m][i].day                 = 0.0;
            S[m][i].hour                = 0.0;
            S[m][i].min                 = 0.0;
            S[m][i].sec                 = 0.0;
        }
    }
    return S;
}

String satellite::alloc_String(int n)
{
    String s;
    n    = MAX(n, MALLOC_MINIMUM_DIMENSION);
    s    = (String) malloc((size_t) ((n + ARRAY_INDEX_START) * sizeof(char)));
    s[0] = '\0'; //_ak
    return s;
}

void satellite::sat_config(std::string config_filename)
{
    // Testing XML read.....
    xmlreader_light reader;
    reader.load(config_filename);
    xmlreader_light::nodes_type sat_children = reader.get_nodes(L"satellite_user_parameter/satellite");
    static std::string          param_sat_name;
    static std::string          param_sat_type;

    // reading Constellation info
    xmlreader_light::nodes_type constl_children = reader.get_nodes(L"satellite_user_parameter/constellation");
    int                         loop_count      = 0, yT;
    for (int i = 0; i < (int) constl_children.size(); i++)
    {
        xmlreader_light    read_counter;
        static std::string flag_Test, flag_subTest, ConNamChek, ConSouChek;
        flag_Test = xmlreader_light::wstring_to_string(
            reader.get_child_node_value<string_type>(constl_children[i], L"location", L""));

        if (!flag_Test.empty())
        {
            read_counter.load(flag_Test);
            xmlreader_light::nodes_type CSC = read_counter.get_nodes(L"satellite_user_parameter/satellite");
            loop_count                      = loop_count + (int) CSC.size();
        }

        flag_subTest = xmlreader_light::wstring_to_string(
            reader.get_child_node_value<string_type>(constl_children[i], L"sub_type", L""));

        yT = 0;
        if (!flag_subTest.empty())
        {
            ConNamChek = xmlreader_light::wstring_to_string(
                reader.get_child_node_value<string_type>(constl_children[i], L"name", L""));
            ConSouChek = xmlreader_light::wstring_to_string(
                reader.get_child_node_value<string_type>(constl_children[i], L"source", L""));

            yT = EconstlNum(ConSouChek, ConNamChek);

            loop_count = loop_count + yT;
            flag_subTest.clear();
            // ConSouChek.clear();
            // ConNamChek.clear();
        }

        flag_Test.clear();
    }
    loop_count = loop_count + (int) sat_children.size();

    specified_sat_info = malloc_SatsInfo_TypeMatrix(loop_count, 1);

    for (int i = 0; i < (int) sat_children.size(); i++)
    {
        param_sat_name =
            xmlreader_light::wstring_to_string(reader.get_child_node_value<string_type>(sat_children[i], L"name", L""));
        param_sat_type =
            xmlreader_light::wstring_to_string(reader.get_child_node_value<string_type>(sat_children[i], L"type", L""));

        if (param_sat_type.compare("new") == 0)
        {

            specified_sat_info->data[(int) (i)][1 - 1].name_of_satellite = alloc_String(MAX_STRING_SIZE);
            strcpy(specified_sat_info->data[(int) (i)][1 - 1].name_of_satellite, param_sat_name.c_str());

            specified_sat_info->data[(int) (i)][1 - 1].type_of_sat = alloc_String(MAX_STRING_SIZE);
            strcpy(specified_sat_info->data[(int) (i)][1 - 1].type_of_sat, param_sat_type.c_str());

            specified_sat_info->data[(int) (i)][1 - 1].epoch_year =
                reader.get_value<double>(L"satellite_user_parameter/satellite/epoch_year", 14);
            specified_sat_info->data[(int) (i)][1 - 1].day_of_year =
                reader.get_value<double>(L"satellite_user_parameter/satellite/day_of_year", 302.9178);
            specified_sat_info->data[(int) (i)][1 - 1].ndt =
                reader.get_value<double>(L"satellite_user_parameter/satellite/ndt", -0.00000241);
            specified_sat_info->data[(int) (i)][1 - 1].nd2t =
                reader.get_value<double>(L"satellite_user_parameter/satellite/nd2t", 0);
            specified_sat_info->data[(int) (i)][1 - 1].bstar =
                reader.get_value<double>(L"satellite_user_parameter/satellite/bstar", 0);
            specified_sat_info->data[(int) (i)][1 - 1].inclination =
                reader.get_value<double>(L"satellite_user_parameter/satellite/inclination", 13.4242);
            specified_sat_info->data[(int) (i)][1 - 1].raan =
                reader.get_value<double>(L"satellite_user_parameter/satellite/raan", 330.1528);
            specified_sat_info->data[(int) (i)][1 - 1].eccentricity =
                reader.get_value<double>(L"satellite_user_parameter/satellite/eccentricity", 0.0011);
            specified_sat_info->data[(int) (i)][1 - 1].argument_of_perigee =
                reader.get_value<double>(L"satellite_user_parameter/satellite/argument_of_perigee", 249.2233);
            specified_sat_info->data[(int) (i)][1 - 1].mean_anomaly =
                reader.get_value<double>(L"satellite_user_parameter/satellite/mean_anomaly", 239.8515);
            specified_sat_info->data[(int) (i)][1 - 1].mean_motion =
                reader.get_value<double>(L"satellite_user_parameter/satellite/mean_motion", 1.0022);
            specified_sat_info->data[(int) (i)][1 - 1].revolution_number =
                reader.get_value<double>(L"satellite_user_parameter/satellite/revolution_number", 7699);
        }
        else if (param_sat_type.compare("existing") == 0)
        {

            specified_sat_info->data[(int) (i)][1 - 1].name_of_satellite = alloc_String(MAX_STRING_SIZE);
            strcpy(specified_sat_info->data[(int) (i)][1 - 1].name_of_satellite, param_sat_name.c_str());

            specified_sat_info->data[(int) (i)][1 - 1].type_of_sat = alloc_String(MAX_STRING_SIZE);
            strcpy(specified_sat_info->data[(int) (i)][1 - 1].type_of_sat, param_sat_type.c_str());

            std::string str_hold = reader.get_value<std::string>(L"satellite_user_parameter/satellite/source", " ");
            specified_sat_info->data[(int) (i)][1 - 1].source_name = alloc_String(MAX_STRING_SIZE);
            strcpy(specified_sat_info->data[(int) (i)][1 - 1].source_name, str_hold.c_str());
        }
        else if (param_sat_type.compare("a_tle") == 0)
        {

            std::string line1, line2;
            specified_sat_info->data[(int) (i)][1 - 1].name_of_satellite = alloc_String(MAX_STRING_SIZE);
            strcpy(specified_sat_info->data[(int) (i)][1 - 1].name_of_satellite, param_sat_name.c_str());

            specified_sat_info->data[(int) (i)][1 - 1].type_of_sat = alloc_String(MAX_STRING_SIZE);
            strcpy(specified_sat_info->data[(int) (i)][1 - 1].type_of_sat, param_sat_type.c_str());

            line1 = reader.get_value<std::string>(L"satellite_user_parameter/satellite/line_1", " ");
            specified_sat_info->data[(int) (i)][1 - 1].tle_line_1 = alloc_String(MAX_STRING_SIZE);
            strcpy(specified_sat_info->data[(int) (i)][1 - 1].tle_line_1, line1.c_str());

            line2 = reader.get_value<std::string>(L"satellite_user_parameter/satellite/line_2", " ");
            specified_sat_info->data[(int) (i)][1 - 1].tle_line_2 = alloc_String(MAX_STRING_SIZE);
            strcpy(specified_sat_info->data[(int) (i)][1 - 1].tle_line_2, line2.c_str());

            line1.clear();
            line2.clear();
        }
        else
        {

            std::cout << "Please check your satellite type info"
                      << "\n";
        }
        param_sat_name.clear();
        param_sat_type.clear();
    }

    // reading Constellation info
    // xmlreader_light::nodes_type constl_children = reader.get_nodes(L"satellite_user_parameter/constellation");
    static std::string param_constl_name;
    static std::string param_constl_type;
    static std::string param_constl_loca_1;
    static std::string param_constl_loca;
    static std::string param_constl_subType;
    static std::string param_constl_soureNa;
    // int counter = (int)sat_children.size() + (int)constl_children.size();

    fC = 0;
    for (int i = 0; i < (int) constl_children.size(); i++)
    {

        param_constl_name = xmlreader_light::wstring_to_string(
            reader.get_child_node_value<string_type>(constl_children[i], L"name", L""));
        param_constl_type = xmlreader_light::wstring_to_string(
            reader.get_child_node_value<string_type>(constl_children[i], L"type", L""));
        param_constl_loca = xmlreader_light::wstring_to_string(
            reader.get_child_node_value<string_type>(constl_children[i], L"location", L""));
        param_constl_subType = xmlreader_light::wstring_to_string(
            reader.get_child_node_value<string_type>(constl_children[i], L"sub_type", L""));

        specified_sat_info->data[(int) sat_children.size() + (int) (fC)][0].constel_name =
            alloc_String(MAX_STRING_SIZE);
        strcpy(specified_sat_info->data[(int) sat_children.size() + (int) (fC)][0].constel_name,
               param_constl_name.c_str());

        specified_sat_info->data[(int) sat_children.size() + (int) (fC)][0].type_constel =
            alloc_String(MAX_STRING_SIZE);
        strcpy(specified_sat_info->data[(int) sat_children.size() + (int) (fC)][0].type_constel,
               param_constl_type.c_str());

        if (param_constl_subType.compare("constel") != 0)
        {

            xmlreader_light sub_reader;
            sub_reader.load(param_constl_loca);
            xmlreader_light::nodes_type constl_sat_children =
                sub_reader.get_nodes(L"satellite_user_parameter/satellite");
            static std::string param_constl_sat_name;
            static std::string param_constl_sat_type;

            for (int j = 0; j < (int) constl_sat_children.size(); j++)
            {
                param_constl_sat_name = xmlreader_light::wstring_to_string(
                    reader.get_child_node_value<string_type>(constl_sat_children[j], L"name", L""));
                param_constl_sat_type = xmlreader_light::wstring_to_string(
                    reader.get_child_node_value<string_type>(constl_sat_children[j], L"type", L""));

                if (param_constl_sat_type.compare("new") == 0)
                {

                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].name_of_satellite =
                        alloc_String(MAX_STRING_SIZE);
                    strcpy(specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].name_of_satellite,
                           param_constl_sat_name.c_str());
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].type_of_sat =
                        alloc_String(MAX_STRING_SIZE);
                    strcpy(specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].type_of_sat,
                           param_constl_sat_type.c_str());
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].epoch_year =
                        reader.get_value<double>(L"satellite_user_parameter/satellite/epoch_year", 14);
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].day_of_year =
                        reader.get_value<double>(L"satellite_user_parameter/satellite/day_of_year", 302.9178);
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].ndt =
                        reader.get_value<double>(L"satellite_user_parameter/satellite/ndt", -0.00000241);
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].nd2t =
                        reader.get_value<double>(L"satellite_user_parameter/satellite/nd2t", 0);
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].bstar =
                        reader.get_value<double>(L"satellite_user_parameter/satellite/bstar", 0);
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].inclination =
                        reader.get_value<double>(L"satellite_user_parameter/satellite/inclination", 13.4242);
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].raan =
                        reader.get_value<double>(L"satellite_user_parameter/satellite/raan", 330.1528);
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].eccentricity =
                        reader.get_value<double>(L"satellite_user_parameter/satellite/eccentricity", 0.0011);
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].argument_of_perigee =
                        reader.get_value<double>(L"satellite_user_parameter/satellite/argument_of_perigee", 249.2233);
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].mean_anomaly =
                        reader.get_value<double>(L"satellite_user_parameter/satellite/mean_anomaly", 239.8515);
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].mean_motion =
                        reader.get_value<double>(L"satellite_user_parameter/satellite/mean_motion", 1.0022);
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].revolution_number =
                        reader.get_value<double>(L"satellite_user_parameter/satellite/revolution_number", 7699);
                }
                else if (param_constl_sat_type.compare("existing") == 0)
                {

                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].name_of_satellite =
                        alloc_String(MAX_STRING_SIZE);
                    strcpy(specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].name_of_satellite,
                           param_constl_sat_name.c_str());
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].type_of_sat =
                        alloc_String(MAX_STRING_SIZE);
                    strcpy(specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].type_of_sat,
                           param_constl_sat_type.c_str());
                    std::string str_hold_ =
                        sub_reader.get_value<std::string>(L"satellite_user_parameter/satellite/source", " ");
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].source_name =
                        alloc_String(MAX_STRING_SIZE);
                    strcpy(specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].source_name,
                           str_hold_.c_str());
                    // fC = fC + 1;
                }
                else if (param_constl_sat_type.compare("a_tle") == 0)
                {

                    std::string line1, line2;
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].name_of_satellite =
                        alloc_String(MAX_STRING_SIZE);
                    strcpy(specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].name_of_satellite,
                           param_constl_sat_name.c_str());
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].type_of_sat =
                        alloc_String(MAX_STRING_SIZE);
                    strcpy(specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].type_of_sat,
                           param_constl_sat_type.c_str());
                    line1 = sub_reader.get_value<std::string>(L"satellite_user_parameter/satellite/line_1", " ");
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].tle_line_1 =
                        alloc_String(MAX_STRING_SIZE);
                    strcpy(specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].tle_line_1,
                           line1.c_str());
                    line2 = sub_reader.get_value<std::string>(L"satellite_user_parameter/satellite/line_2", " ");
                    specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].tle_line_2 =
                        alloc_String(MAX_STRING_SIZE);
                    strcpy(specified_sat_info->data[(int) sat_children.size() + (int) (fC)][1 - 1].tle_line_2,
                           line2.c_str());
                    // fC = fC + 1;
                    line1.clear();
                    line2.clear();
                }
                else
                {
                    std::cout << "Please check your satellite type info"
                              << "\n";
                }

                fC = fC + 1;
            }
        }
        else
        {

            param_constl_soureNa = xmlreader_light::wstring_to_string(
                reader.get_child_node_value<string_type>(constl_children[i], L"source", L""));

            int         kp = 0;
            char        cline1[100];
            char        cline2[100];
            char        cline3[100];
            char        holder[100];
            char *      str1, *str2;
            int         counter = 0;
            std::string status;

            FILE* pFile;
            pFile = fopen(param_constl_soureNa.c_str(), "r");
            if (pFile == NULL)
            {
                perror("Error opening file");
            }
            else
            {

                status = "false";
                while (!feof(pFile) != 0)
                {
                    fgets(cline1, MAX_STRING_SIZE, pFile);

                    fgets(cline2, MAX_STRING_SIZE, pFile);
                    fgets(cline3, MAX_STRING_SIZE, pFile);

                    std::string str_a, str_b;
                    str1  = strlwr(cline1);
                    str_a = str1;

                    strcpy(holder, param_constl_name.c_str());
                    str2  = strlwr(holder);
                    str_b = str2;

                    std::size_t found = str_a.find(str_b);

                    std::string str_check = cline1;
                    str_check.resize(40);

                    if (found != std::string::npos)
                    {

                        fC = fC + 1;
                        str_check.erase(0, 1);
                        specified_sat_info->data[(int) sat_children.size() - 1 + (int) (fC)][1 - 1].name_of_satellite =
                            alloc_String(MAX_STRING_SIZE);
                        strcpy(specified_sat_info->data[(int) sat_children.size() - 1 + (int) (fC)][1 - 1]
                                   .name_of_satellite,
                               str_check.c_str());
                        specified_sat_info->data[(int) sat_children.size() - 1 + (int) (fC)][1 - 1].type_of_sat =
                            alloc_String(MAX_STRING_SIZE);
                        strcpy(specified_sat_info->data[(int) sat_children.size() - 1 + (int) (fC)][1 - 1].type_of_sat,
                               param_constl_type.c_str());
                        specified_sat_info->data[(int) sat_children.size() - 1 + (int) (fC)][1 - 1].source_name =
                            alloc_String(MAX_STRING_SIZE);
                        strcpy(specified_sat_info->data[(int) sat_children.size() - 1 + (int) (fC)][1 - 1].source_name,
                               param_constl_soureNa.c_str());
                    }
                }
            }
            // fC = fC + kp ;
        }

        param_constl_name.clear();
        param_constl_type.clear();
    }
}

States_TypeMatrix satellite::GetStates(std::vector<double> ctime)
{

    sat_info          satInfo;
    States_TypeMatrix states;

    states = malloc_States_TypeMatrix(specified_sat_info->m, 1);

    for (int i = 0; i < specified_sat_info->m; i++)
    {
        states->data[i][0].position.malloc(3, 1, 0);
        states->data[i][0].velocity.malloc(3, 1, 0);
        states->data[i][0].ctime.malloc(6, 1, 0);
        states->data[i][0].SatName = NULL;
    }

    for (int i = 0; i < specified_sat_info->m; i++)
    {

        if (strcmp(specified_sat_info->data[(int) (i)][1 - 1].type_of_sat, "new") == 0)
        {
            satInfo = new2kep(i, specified_sat_info);
            SatPOS_Wrapper(i, ctime, satInfo, states);
        }
        if (strcmp(specified_sat_info->data[(int) (i)][1 - 1].type_of_sat, "existing") == 0)
        {
            satInfo = norad2kep(specified_sat_info->data[(int) (i)][1 - 1].source_name,
                                specified_sat_info->data[(int) (i)][1 - 1].name_of_satellite);
            SatPOS_Wrapper(i, ctime, satInfo, states);
        }
        if (strcmp(specified_sat_info->data[(int) (i)][1 - 1].type_of_sat, "a_tle") == 0)
        {
            satInfo = tle2kep(specified_sat_info->data[(int) (i)][1 - 1].tle_line_1,
                              specified_sat_info->data[(int) (i)][1 - 1].tle_line_2);
            SatPOS_Wrapper(i, ctime, satInfo, states);
        }
    }

    return (states);
}

States_TypeMatrix satellite::GetState_Fprints(std::vector<double> ctime, double coverage)
{
    sat_info          satInfo;
    States_TypeMatrix states;

    states = malloc_States_TypeMatrix(specified_sat_info->m, 1);

    for (int i = 0; i < specified_sat_info->m; i++)
    {
        states->data[i][0].position.malloc(3, 1, 0);
        states->data[i][0].velocity.malloc(3, 1, 0);
        states->data[i][0].ctime.malloc(6, 1, 0);
        states->data[i][0].SatName = NULL;
        states->data[i][0].lat     = 0.0;
        states->data[i][0].lon     = 0.0;
        states->data[i][0].radii;
    }

    for (int i = 0; i < specified_sat_info->m; i++)
    {

        if (strcmp(specified_sat_info->data[(int) (i)][1 - 1].type_of_sat, "new") == 0)
        {
            satInfo = new2kep(i, specified_sat_info);
            SatPOS_Wrapper_footPrint(i, ctime, satInfo, states, coverage);
        }
        if (strcmp(specified_sat_info->data[(int) (i)][1 - 1].type_of_sat, "existing") == 0)
        {
            satInfo = norad2kep(specified_sat_info->data[(int) (i)][1 - 1].source_name,
                                specified_sat_info->data[(int) (i)][1 - 1].name_of_satellite);
            SatPOS_Wrapper_footPrint(i, ctime, satInfo, states, coverage);
        }
        if (strcmp(specified_sat_info->data[(int) (i)][1 - 1].type_of_sat, "a_tle") == 0)
        {
            satInfo = tle2kep(specified_sat_info->data[(int) (i)][1 - 1].tle_line_1,
                              specified_sat_info->data[(int) (i)][1 - 1].tle_line_2);
            SatPOS_Wrapper_footPrint(i, ctime, satInfo, states, coverage);
        }
    }

    return (states);
}

States_TypeMatrix satellite::GetState(std::string name_of_satellite, std::vector<double> ctime)
{
    sat_info          satInfo;
    States_TypeMatrix states;
    int               yT;
    std::string       source_name;
    source_name = "TLE.3le";

    yT                        = EconstlNum(source_name, name_of_satellite);
    specified_sat_info_direct = malloc_SatsInfo_TypeMatrix(yT, 1);

    states = malloc_States_TypeMatrix(yT, 1);

    for (int i = 0; i < yT; i++)
    {
        states->data[i][0].position.malloc(3, 1, 0);
        states->data[i][0].velocity.malloc(3, 1, 0);
        states->data[i][0].ctime.malloc(6, 1, 0);
        states->data[i][0].SatName = NULL;
    }

    if (yT == 1)
    {
        specified_sat_info_direct->data[(int) (yT) -1][1 - 1].name_of_satellite = alloc_String(MAX_STRING_SIZE);
        strcpy(specified_sat_info_direct->data[(int) (yT) -1][1 - 1].name_of_satellite, name_of_satellite.c_str());
        specified_sat_info_direct->data[(int) (yT) -1][1 - 1].source_name = alloc_String(MAX_STRING_SIZE);
        strcpy(specified_sat_info_direct->data[(int) (yT) -1][1 - 1].source_name, source_name.c_str());
        satInfo = norad2kep(specified_sat_info_direct->data[(int) (yT) -1][1 - 1].source_name,
                            specified_sat_info_direct->data[(int) (yT) -1][1 - 1].name_of_satellite);

        SatSingPOS_Wrapper(0, ctime, satInfo, states);
    }
    else
    {
        char        cline1[100];
        char        cline2[100];
        char        cline3[100];
        char        holder[100];
        char *      str1, *str2;
        int         counter = 0;
        std::string status;

        FILE* pFile;
        pFile = fopen(source_name.c_str(), "r");
        if (pFile == NULL)
        {
            perror("Error opening file");
        }
        else
        {

            status = "false";
            while (!feof(pFile) != 0)
            {
                fgets(cline1, MAX_STRING_SIZE, pFile);
                fgets(cline2, MAX_STRING_SIZE, pFile);
                fgets(cline3, MAX_STRING_SIZE, pFile);

                std::string str_a, str_b;
                str1  = strlwr(cline1);
                str_a = str1;

                strcpy(holder, name_of_satellite.c_str());
                str2  = strlwr(holder);
                str_b = str2;

                std::size_t found = str_a.find(str_b);

                std::string str_check = cline1;
                str_check.resize(40);

                if (found != std::string::npos)
                {
                    str_check.erase(0, 1);
                    specified_sat_info_direct->data[(int) (counter)][1 - 1].name_of_satellite =
                        alloc_String(MAX_STRING_SIZE);
                    strcpy(specified_sat_info_direct->data[(int) (counter)][1 - 1].name_of_satellite,
                           str_check.c_str());
                    specified_sat_info_direct->data[(int) (counter)][1 - 1].source_name = alloc_String(MAX_STRING_SIZE);
                    strcpy(specified_sat_info_direct->data[(int) (counter)][1 - 1].source_name, source_name.c_str());
                    counter = counter + 1;
                }
            }
        }

        for (int i = 0; i < yT; i++)
        {

            satInfo = norad2kep(specified_sat_info_direct->data[(int) (i)][1 - 1].source_name,
                                specified_sat_info_direct->data[(int) (i)][1 - 1].name_of_satellite);
            SatSingPOS_Wrapper(i, ctime, satInfo, states);
        }
    }
    return (states);
}

States_TypeMatrix satellite::GetState_Fprint(std::string name_of_satellite, std::vector<double> ctime, double coverage)
{
    sat_info          satInfo;
    States_TypeMatrix states;
    int               yT;
    std::string       source_name;
    source_name = "TLE.3le";

    yT                        = EconstlNum(source_name, name_of_satellite);
    specified_sat_info_direct = malloc_SatsInfo_TypeMatrix(yT, 1);

    states = malloc_States_TypeMatrix(yT, 1);

    for (int i = 0; i < yT; i++)
    {
        states->data[i][0].position.malloc(3, 1, 0);
        states->data[i][0].velocity.malloc(3, 1, 0);
        states->data[i][0].ctime.malloc(6, 1, 0);
        states->data[i][0].SatName = NULL;
        states->data[i][0].lat     = 0.0;
        states->data[i][0].lon     = 0.0;
    }

    if (yT == 1)
    {
        specified_sat_info_direct->data[(int) (yT) -1][1 - 1].name_of_satellite = alloc_String(MAX_STRING_SIZE);
        strcpy(specified_sat_info_direct->data[(int) (yT) -1][1 - 1].name_of_satellite, name_of_satellite.c_str());
        specified_sat_info_direct->data[(int) (yT) -1][1 - 1].source_name = alloc_String(MAX_STRING_SIZE);
        strcpy(specified_sat_info_direct->data[(int) (yT) -1][1 - 1].source_name, source_name.c_str());
        satInfo = norad2kep(specified_sat_info_direct->data[(int) (yT) -1][1 - 1].source_name,
                            specified_sat_info_direct->data[(int) (yT) -1][1 - 1].name_of_satellite);

        SatSingPOS_Wrapper_footPrint(0, ctime, satInfo, states, coverage);
    }
    else
    {
        char        cline1[100];
        char        cline2[100];
        char        cline3[100];
        char        holder[100];
        char *      str1, *str2;
        int         counter = 0;
        std::string status;

        FILE* pFile;
        pFile = fopen(source_name.c_str(), "r");
        if (pFile == NULL)
        {
            perror("Error opening file");
        }
        else
        {

            status = "false";
            while (!feof(pFile) != 0)
            {
                fgets(cline1, MAX_STRING_SIZE, pFile);
                fgets(cline2, MAX_STRING_SIZE, pFile);
                fgets(cline3, MAX_STRING_SIZE, pFile);

                std::string str_a, str_b;
                str1  = strlwr(cline1);
                str_a = str1;

                strcpy(holder, name_of_satellite.c_str());
                str2  = strlwr(holder);
                str_b = str2;

                std::size_t found = str_a.find(str_b);

                std::string str_check = cline1;
                str_check.resize(40);

                if (found != std::string::npos)
                {
                    str_check.erase(0, 1);
                    specified_sat_info_direct->data[(int) (counter)][1 - 1].name_of_satellite =
                        alloc_String(MAX_STRING_SIZE);
                    strcpy(specified_sat_info_direct->data[(int) (counter)][1 - 1].name_of_satellite,
                           str_check.c_str());
                    specified_sat_info_direct->data[(int) (counter)][1 - 1].source_name = alloc_String(MAX_STRING_SIZE);
                    strcpy(specified_sat_info_direct->data[(int) (counter)][1 - 1].source_name, source_name.c_str());
                    counter = counter + 1;
                }
            }
        }

        for (int i = 0; i < yT; i++)
        {

            satInfo = norad2kep(specified_sat_info_direct->data[(int) (i)][1 - 1].source_name,
                                specified_sat_info_direct->data[(int) (i)][1 - 1].name_of_satellite);
            SatSingPOS_Wrapper_footPrint(i, ctime, satInfo, states, coverage);
        }
    }

    return (states);
}

States_TypeMatrix malloc_States_TypeMatrix(int m, int n)
{
    States_TypeMatrix S = NULL;
    S                   = (States_TypeMatrix) malloc(sizeof(_States_TypeMatrix));
    S->m                = m;
    S->n                = n;
    if (m == 0 || n == 0)
    {
        S->data = NULL;
    }
    else
    {
        S->data = (States_TypeDblPtr) malloc_States_Type_Matrix(m, n);
    }
    return S;
}

States_Type_Matrix malloc_States_Type_Matrix(int m, int n)
{
    int                i;
    States_Type_Matrix S = NULL;
    S                    = (States_Type_Matrix) malloc((size_t) (m * sizeof(States_TypePtr)));
    if (S)
    {
        if (m * n > 0)
        {
            S[0] = (States_TypePtr) malloc((size_t) ((m * n) * sizeof(States_Type)));
            if (!S[0])
            {
                return NULL;
            }
            for (i = 1; i < m; i++)
            {
                S[i] = S[i - 1] + n;
            }
        }
    }

    for (m--; m >= 0; m--)
    {
        i = n;
        for (i--; i >= 0; i--)
        {
            memset(&S[m][i].position, 0, sizeof(doubleMatrix));
            memset(&S[m][i].velocity, 0, sizeof(doubleMatrix));
            memset(&S[m][i].ctime, 0, sizeof(doubleMatrix));
            S[m][i].lat     = 0.0;
            S[m][i].lon     = 0.0;
            S[m][i].radii   = 0.0;
            S[m][i].SatName = NULL;
        }
    }
    return S;
}

DoubleMatrix alloc_DoubleMatrix_val(int m, int n, double val)
{
    DoubleMatrix M;
    int          x, y;
    M = alloc_DoubleMatrix(m, n);
    for (x = 0; x < m; x++)
    {
        for (y = 0; y < n; y++)
        {
            M[x][y] = val;
        }
    }
    return M;
}

DoubleMatrix alloc_DoubleMatrix(int m, int n)
{
    DoubleMatrix M;
    int          i;
    m = MAX(m, MALLOC_MINIMUM_DIMENSION);
    n = MAX(n, MALLOC_MINIMUM_DIMENSION);
    M = (DoubleMatrix) malloc((size_t) ((m + ARRAY_INDEX_START) * sizeof(double*)));
    if (M)
    {
        M[0] = (double*) malloc((size_t) (((m + ARRAY_INDEX_START) * (n + ARRAY_INDEX_START)) * sizeof(double)));
        if (!M[0])
        {
            return NULL;
        }
        for (i = 1; i < m + ARRAY_INDEX_START; i++)
            M[i] = M[i - 1] + n + ARRAY_INDEX_START;
    }
    return M;
}

template <class T, class T2, class T3>
T t_set_scalar_v(T C, T2 B, int n)
{
    for (--n; n >= 0; n--)
    {
        C[n] = (T3) B;
    }
    return C;
}
// matrix
template <class T, class T2, class T3>
T t_set_scalar_m(T C, T2 B, int m, int n)
{
    int x = --n;
    ;
    for (--m; m >= 0; m--)
    {
        for (x = n; x >= 0; x--)
        {
            C[m][x] = (T3) B;
        }
    }
    return C;
}

template <class T, class T2, class T3>
T t_copy_v(T A, T2 B, int n)
{
    for (--n; n >= 0; n--)
    {
        A[n] = (T3) B[n];
    }
    return A;
}
// matrix
template <class T, class T2, class T3>
T t_copy_m(T A, T2 B, int m, int n)
{
    int x = --n;
    for (--m; m >= 0; m--)
    {
        for (x = n; x >= 0; x--)
        {
            A[m][x] = (T3) B[m][x];
        }
    }
    return A;
}

void free_DoubleMatrix(DoubleMatrix* M)
{
    if (*M)
    {
        free((*M)[0]);
        free(*M);
        *M = NULL;
    }
}

DoubleMatrix copy_DoubleMatrix(int m, int n, DoubleMatrix A, DoubleMatrix B)
{
    int x, y;
    if ((A == NULL) || (B == NULL))
    {
        return B;
    }
    for (x = 0; x < m; x++)
    {
        for (y = 0; y < n; y++)
        {
            B[x][y] = A[x][y];
        }
    }
    return B;
}

DoubleMatrix resize_DoubleMatrix_val(int Am, int An, int Bm, int Bn, DoubleMatrix A, double val)
{
    DoubleMatrix B;
    B = alloc_DoubleMatrix_val(Bm, Bn, val);
    copy_DoubleMatrix(MIN(Am, Bm), MIN(An, Bn), A, B);
    FREE_DOUBLE_MATRIX(A);
    return B;
}

// resize
void doubleMatrix::resize(int m, int n)
{
    if ((m < 0) || (n < 0))
    {
        // nf_internal_error(0, NF_ERR_POS_INT, "");
    }
    DoubleMatrix newV;
    if ((n > this->nmax) || (m > this->mmax))
    {
        newV = alloc_DoubleMatrix(m, n);
        t_copy_m<DoubleMatrix, DoubleMatrix, double>(newV, data, MIN(m, this->m), MIN(n, this->n));
        free_DoubleMatrix(&data);
        this->data = newV;
        this->n    = n;
        this->m    = m;
        this->nmax = MAX(n, MALLOC_MINIMUM_DIMENSION);
        this->mmax = MAX(m, MALLOC_MINIMUM_DIMENSION);
    }
    else
    {
        this->n = n;
        this->m = m;
    }
}

void doubleMatrix::resize(int m, int n, double val)
{
    if ((m < 0) || (n < 0))
    {
        // nf_internal_error(0, NF_ERR_POS_INT, "");
    }
    DoubleMatrix newV;
    if ((n > this->nmax) || (m > this->mmax))
    {
        newV = alloc_DoubleMatrix_val(m, n, val);
        t_copy_m<DoubleMatrix, DoubleMatrix, double>(newV, data, MIN(m, this->m), MIN(n, this->n));
        free_DoubleMatrix(&data);
        this->data = newV;
        this->n    = n;
        this->m    = m;
        this->nmax = MAX(n, MALLOC_MINIMUM_DIMENSION);
        this->mmax = MAX(m, MALLOC_MINIMUM_DIMENSION);
    }
    else
    {
        this->n = n;
        this->m = m;
    }
}

// manual malloc. If memory is already allocated, this calls resize instead
void doubleMatrix::malloc(int m, int n)
{
    if ((m < 0) || (n < 0))
    {
        // nf_internal_error(0, NF_ERR_POS_INT, "");
    }
    if (!this->data)
    {
        this->data = alloc_DoubleMatrix(m, n);
        this->n    = n;
        this->m    = m;
        this->nmax = MAX(n, MALLOC_MINIMUM_DIMENSION);
        this->mmax = MAX(m, MALLOC_MINIMUM_DIMENSION);
    }
    else
    {
        // if there is already data there, then resize instead
        resize(m, n);
    }
}
void doubleMatrix::malloc(int m, int n, double val)
{
    if ((m < 0) || (n < 0))
    {
        // nf_internal_error(0, NF_ERR_POS_INT, "");
    }
    if (!this->data)
    {
        this->data = alloc_DoubleMatrix_val(m, n, val);
        this->n    = n;
        this->m    = m;
        this->nmax = MAX(n, MALLOC_MINIMUM_DIMENSION);
        this->mmax = MAX(m, MALLOC_MINIMUM_DIMENSION);
    }
    else
    {
        // if there is already data there, then resize instead
        t_set_scalar_m<DoubleMatrix, double, double>(this->data, val, m, n);
        resize(m, n, val);
    }
}

sat_info satellite::tle2kep(char* cline2, char* cline3)
{
    char        holder[100];
    char *      str1, *str2;
    double      yr, dayofyear, epochN;
    std::string status;

    // epoch year (2 digits)
    // yr = str2num(cline2(19:20)); matlab
    char buffer1[3];
    buffer1[0] = cline2[18];
    buffer1[1] = cline2[19];
    buffer1[2] = '\0';
    yr         = atof(buffer1);

    // epoch day fraction
    // dayofyear = str2num(cline2(21:32));  matlab
    char buffer2[14];
    buffer2[0]  = cline2[20];
    buffer2[1]  = cline2[21];
    buffer2[2]  = cline2[22];
    buffer2[3]  = cline2[23];
    buffer2[4]  = cline2[24];
    buffer2[5]  = cline2[25];
    buffer2[6]  = cline2[26];
    buffer2[7]  = cline2[27];
    buffer2[8]  = cline2[28];
    buffer2[9]  = cline2[29];
    buffer2[10] = cline2[30];
    buffer2[11] = cline2[31];
    buffer2[12] = cline2[32];
    buffer2[13] = '\0';
    dayofyear   = atof(buffer2);

    // NORAD specification
    // epochN=(1e3.*yr)+ dayofyear;  matlab
    epochN = (1000 * yr) + dayofyear;

    // time conversion
    // if (yr < 57) yr = 2000 + yr; else yr = 1900 + yr; end matlab
    if (yr < 57)
    {
        yr = 2000 + yr;
    }
    else
    {
        yr = 1900 + yr;
    }

    double xjdtmp = julian(1, 0, yr);

    // Julian date of the epoch
    double xjdtle = xjdtmp + dayofyear;

    // d/dt of mean motin
    // ndt = str2num(cline2(34:43)); matlab
    char buffer3[14];
    buffer3[0]  = cline2[33];
    buffer3[1]  = cline2[34];
    buffer3[2]  = cline2[35];
    buffer3[3]  = cline2[36];
    buffer3[4]  = cline2[37];
    buffer3[5]  = cline2[38];
    buffer3[6]  = cline2[39];
    buffer3[7]  = cline2[40];
    buffer3[8]  = cline2[41];
    buffer3[9]  = cline2[42];
    buffer3[10] = cline2[43];
    buffer3[11] = cline2[44];
    buffer3[12] = cline2[45];
    buffer3[13] = '\0';
    double ndt  = atof(buffer3);

    // d2/dt2 of mean motin
    // nd2t = 1.0e-5.*str2num(cline2(45:50)).*(10^str2num(cline2(51:52)));
    char buffer4[7];
    buffer4[0] = cline2[44];
    buffer4[1] = cline2[45];
    buffer4[2] = cline2[46];
    buffer4[3] = cline2[47];
    buffer4[4] = cline2[48];
    buffer4[5] = cline2[49];
    buffer4[6] = '\0';
    char buffer5[3];
    buffer5[0]  = cline2[50];
    buffer5[1]  = cline2[51];
    buffer5[2]  = '\0';
    double nd2t = 0.00001 * atof(buffer4) * pow(10, atof(buffer5));

    // bstar drag term
    // bstar = 1.0e-5.*str2num(cline2(54:59)).*(10^str2num(cline2(60:61)))./ae;
    char buffer6[7];
    buffer6[0] = cline2[53];
    buffer6[1] = cline2[54];
    buffer6[2] = cline2[55];
    buffer6[3] = cline2[56];
    buffer6[4] = cline2[57];
    buffer6[5] = cline2[58];
    buffer6[6] = '\0';
    char buffer7[3];
    buffer7[0]   = cline2[59];
    buffer7[1]   = cline2[60];
    buffer7[2]   = '\0';
    double bstar = (0.00001 * atof(buffer6) * pow(10, atof(buffer7))) / ae;

    // orbit inclination
    // incl = str2num(cline3(9:16));
    char buffer8[9];
    buffer8[0]  = cline3[8];
    buffer8[1]  = cline3[9];
    buffer8[2]  = cline3[10];
    buffer8[3]  = cline3[11];
    buffer8[4]  = cline3[12];
    buffer8[5]  = cline3[13];
    buffer8[6]  = cline3[14];
    buffer8[7]  = cline3[15];
    buffer8[8]  = '\0';
    double incl = atof(buffer8);

    // right ascension of ascending node
    // raan = str2num(cline3(18:25));
    char buffer9[9];
    buffer9[0]  = cline3[17];
    buffer9[1]  = cline3[18];
    buffer9[2]  = cline3[19];
    buffer9[3]  = cline3[20];
    buffer9[4]  = cline3[21];
    buffer9[5]  = cline3[22];
    buffer9[6]  = cline3[23];
    buffer9[7]  = cline3[24];
    buffer9[8]  = '\0';
    double raan = atof(buffer9);

    // eccentricity
    // ecc = 1.0e-7.*str2num(cline3(27:33));
    char buffer10[8];
    buffer10[0] = cline3[26];
    buffer10[1] = cline3[27];
    buffer10[2] = cline3[28];
    buffer10[3] = cline3[29];
    buffer10[4] = cline3[30];
    buffer10[5] = cline3[31];
    buffer10[6] = cline3[32];
    buffer10[7] = '\0';
    double ecc  = 0.0000001 * atof(buffer10);

    // argument of perigee
    // omegao = str2num(cline3(35:42));
    char buffer11[9];
    buffer11[0]   = cline3[34];
    buffer11[1]   = cline3[35];
    buffer11[2]   = cline3[36];
    buffer11[3]   = cline3[37];
    buffer11[4]   = cline3[38];
    buffer11[5]   = cline3[39];
    buffer11[6]   = cline3[40];
    buffer11[7]   = cline3[41];
    buffer11[8]   = '\0';
    double omegao = atof(buffer11);

    // mean anomaly
    // mo = str2num(cline3(44:51));
    char buffer12[9];
    buffer12[0] = cline3[43];
    buffer12[1] = cline3[44];
    buffer12[2] = cline3[45];
    buffer12[3] = cline3[46];
    buffer12[4] = cline3[47];
    buffer12[5] = cline3[48];
    buffer12[6] = cline3[49];
    buffer12[7] = cline3[50];
    buffer12[8] = '\0';
    double mo   = atof(buffer12);

    // mean motion
    // no = str2num(cline3(53:63));
    char buffer13[12];
    buffer13[0]  = cline3[52];
    buffer13[1]  = cline3[53];
    buffer13[2]  = cline3[54];
    buffer13[3]  = cline3[55];
    buffer13[4]  = cline3[56];
    buffer13[5]  = cline3[57];
    buffer13[6]  = cline3[58];
    buffer13[7]  = cline3[59];
    buffer13[8]  = cline3[60];
    buffer13[9]  = cline3[61];
    buffer13[10] = cline3[62];
    buffer13[11] = '\0';
    double no    = atof(buffer13);

    // revolution number
    // rev = str2num(cline3(64:68));
    char buffer14[6];
    buffer14[0] = cline3[63];
    buffer14[1] = cline3[64];
    buffer14[2] = cline3[65];
    buffer14[3] = cline3[66];
    buffer14[4] = cline3[67];
    buffer14[5] = '\0';
    double rev  = atof(buffer14);

    // epoch=datevec(datenum(yr, 0 , dayofyear));
    double currT;
    currT = datenum(yr, 0, dayofyear, 0, 0, 0, 0);
    std::vector<double> flag2(6);
    flag2 = NF_datevec(currT);
    sat_info satInfo;
    satInfo.epochC = flag2;

    // return of the struct
    // sat.name=deblank(upper(name));
    satInfo.name = strupr(holder);

    // orbit elements
    std::vector<double> tle(11);
    tle.at(0)  = xjdtle;
    tle.at(1)  = ndt;
    tle.at(2)  = nd2t;
    tle.at(3)  = bstar;
    tle.at(4)  = incl;
    tle.at(5)  = raan;
    tle.at(6)  = ecc;
    tle.at(7)  = omegao;
    tle.at(8)  = mo;
    tle.at(9)  = no;
    tle.at(10) = rev;
    satInfo.tle.reserve(11);
    satInfo.tle    = tle;
    satInfo.epochN = epochN;

    return satInfo;
}

void satellite::SatPOS_Wrapper(int i, std::vector<double> ctime, sat_info satInfo, States_TypeMatrix states)
{

    // find the states
    std::vector<std::pair<double, double>> Sat_states;

    Sat_states                     = SatPOS(ctime, satInfo);
    states->data[i][1 - 1].SatName = alloc_String(MAX_STRING_SIZE);
    strcpy(states->data[i][1 - 1].SatName, specified_sat_info->data[(int) (i)][1 - 1].name_of_satellite);
    for (int ii = 0; ii < Sat_states.size(); ii++)
    {
        states->data[i][1 - 1].position.data[ii][0] = Sat_states.at(ii).first;
        states->data[i][1 - 1].velocity.data[ii][0] = Sat_states.at(ii).second;
    }

    for (int jj = 0; jj < ctime.size(); jj++)
    {
        states->data[i][1 - 1].ctime.data[jj][0] = ctime.at(jj);
    }
}

void satellite::SatSingPOS_Wrapper(int i, std::vector<double> ctime, sat_info satInfo, States_TypeMatrix states)
{

    // find the states
    std::vector<std::pair<double, double>> Sat_states;
    Sat_states                     = SatPOS(ctime, satInfo);
    states->data[i][1 - 1].SatName = alloc_String(MAX_STRING_SIZE);
    strcpy(states->data[i][1 - 1].SatName, specified_sat_info_direct->data[(int) (i)][1 - 1].name_of_satellite);
    for (int ii = 0; ii < Sat_states.size(); ii++)
    {
        states->data[i][1 - 1].position.data[ii][0] = Sat_states.at(ii).first;
        states->data[i][1 - 1].velocity.data[ii][0] = Sat_states.at(ii).second;
    }

    for (int jj = 0; jj < ctime.size(); jj++)
    {
        states->data[i][1 - 1].ctime.data[jj][0] = ctime.at(jj);
    }
}

sat_info satellite::new2kep(int i, SatsInfo_TypeMatrix specified_sat_info)
{
    sat_info satInfo;
    double   yr        = specified_sat_info->data[(int) (i)][1 - 1].epoch_year;
    double   dayofyear = specified_sat_info->data[(int) (i)][1 - 1].day_of_year;
    // satInfo.name = strupr(specified_sat_info->data[(int)(i)][1 - 1].name_of_satellite);
    double epochN  = (1000 * yr) + dayofyear;
    satInfo.epochN = epochN;

    if (yr < 57)
    {
        yr = 2000 + yr;
    }
    else
    {
        yr = 1900 + yr;
    }

    double xjdtmp = julian(1, 0, yr);

    // Julian date of the epoch
    double xjdtle = xjdtmp + dayofyear;

    double currT;
    currT = datenum(yr, 0, dayofyear, 0, 0, 0, 0);
    std::vector<double> flag1(6);
    flag1          = NF_datevec(currT);
    satInfo.epochC = flag1;

    // orbit elements
    std::vector<double> tle(11);
    tle.at(0)  = xjdtle;
    tle.at(1)  = specified_sat_info->data[(int) (i)][1 - 1].ndt;
    tle.at(2)  = specified_sat_info->data[(int) (i)][1 - 1].nd2t;
    tle.at(3)  = specified_sat_info->data[(int) (i)][1 - 1].bstar;
    tle.at(4)  = specified_sat_info->data[(int) (i)][1 - 1].inclination;
    tle.at(5)  = specified_sat_info->data[(int) (i)][1 - 1].raan;
    tle.at(6)  = specified_sat_info->data[(int) (i)][1 - 1].eccentricity;
    tle.at(7)  = specified_sat_info->data[(int) (i)][1 - 1].argument_of_perigee;
    tle.at(8)  = specified_sat_info->data[(int) (i)][1 - 1].mean_anomaly;
    tle.at(9)  = specified_sat_info->data[(int) (i)][1 - 1].mean_motion;
    tle.at(10) = specified_sat_info->data[(int) (i)][1 - 1].revolution_number;
    satInfo.tle.reserve(11);
    satInfo.tle = tle;

    return (satInfo);
}

SatsInfo_TypeMatrix satellite::resize_SatsInfoTypeMatrix(SatsInfo_TypeMatrix old, int m, int n)
{
    SatsInfo_Type_Matrix S = NULL;
    int                  i, j;
    S         = old->data;
    old->data = (SatsInfo_TypeDblPtr) malloc_SatsInfo_Type_Matrix(m, n);
    for (i = 0; i < MIN(m, old->m); i++)
    {
        for (j = 0; j < MIN(n, old->n); j++)
        {

            old->data[i][j].name_of_satellite   = S[i][j].name_of_satellite;
            old->data[i][j].type_of_sat         = S[i][j].type_of_sat;
            old->data[i][j].source_name         = S[i][j].source_name;
            old->data[i][j].tle_line_1          = S[i][j].tle_line_1;
            old->data[i][j].tle_line_2          = S[i][j].tle_line_2;
            old->data[i][j].constel_name        = S[i][j].constel_name;
            old->data[i][j].type_constel        = S[i][j].type_constel;
            old->data[i][j].epoch_year          = S[i][j].epoch_year;
            old->data[i][j].day_of_year         = S[i][j].day_of_year;
            old->data[i][j].ndt                 = S[i][j].ndt;
            old->data[i][j].nd2t                = S[i][j].nd2t;
            old->data[i][j].bstar               = S[i][j].bstar;
            old->data[i][j].inclination         = S[i][j].inclination;
            old->data[i][j].raan                = S[i][j].raan;
            old->data[i][j].eccentricity        = S[i][j].eccentricity;
            old->data[i][j].argument_of_perigee = S[i][j].argument_of_perigee;
            old->data[i][j].mean_anomaly        = S[i][j].mean_anomaly;
            old->data[i][j].mean_motion         = S[i][j].mean_motion;
            old->data[i][j].revolution_number   = S[i][j].revolution_number;
            old->data[i][j].year                = S[i][j].year;
            old->data[i][j].month               = S[i][j].month;
            old->data[i][j].day                 = S[i][j].day;
            old->data[i][j].hour                = S[i][j].hour;
            old->data[i][j].min                 = S[i][j].min;
            old->data[i][j].sec                 = S[i][j].sec;
        }
    }
    if (S)
    {
        free(S);
    }
    old->n = n;
    old->m = m;
    return old;
}

std::string satellite::find_fullPath(std::string ginen_Name)
{
    char        cCurrentPath[FILENAME_MAX];
    std::string str_a;
    GetCurrentDir(cCurrentPath, sizeof(cCurrentPath));
    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
    str_a                                  = cCurrentPath;
    std::string full_path;
    str_a.resize(str_a.size() - 4);
    full_path.append(str_a);
    full_path.append("\\config\\");
    full_path.append(ginen_Name);
    std::cout << "\n" << full_path << "\n";
    return (full_path);
}

void satellite::printf_states(States_TypeMatrix states)
{
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Satellite Trajectory Simulator				  " << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "												  " << std::endl;
    std::cout << "by:											  " << std::endl;
    std::cout << "Rajiv Sithiravel and Thiagalingam Kirubarajan   " << std::endl;
    std::cout << "McMaster University							  " << std::endl;
    std::cout << "Hamilton, Ontario,							  " << std::endl;
    std::cout << "Canada.										  " << std::endl;
    std::cout << "												  " << std::endl;
    std::cout << "Current time:  " << currentDateTime() << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    for (int i = 0; i < states->m; i++)
    {
        std::cout << "Satellite Name: " << states->data[i][1 - 1].SatName << "\n";

        std::cout << "Time: "
                  << "\t"
                  << "[";
        for (int jj = 0; jj < states->data[i][1 - 1].ctime.m; jj++)
        {
            if (jj < 2)
            {
                std::cout << states->data[i][1 - 1].ctime.data[jj][0] << "-";
            }
            if (jj == 2)
            {
                std::cout << states->data[i][1 - 1].ctime.data[jj][0] << " ";
            }
            if ((jj > 2) && (jj < 5))
            {
                std::cout << states->data[i][1 - 1].ctime.data[jj][0] << ":";
            }
            if (jj == 5)
            {
                std::cout << states->data[i][1 - 1].ctime.data[jj][0];
            }
        }
        std::cout << "]";

        std::cout << "\n"
                  << "States: "
                  << "\n";
        std::cout << "\n"
                  << "Position"
                  << "\t"
                  << "Velocity"
                  << "\n";
        for (int ii = 0; ii < states->data[i][1 - 1].position.m; ii++)
        {
            std::cout << states->data[i][1 - 1].position.data[ii][0] << "\t"
                      << states->data[i][1 - 1].velocity.data[ii][0] << "\n";
        }
        std::cout << "\n\n";
    }
}

const std::string satellite::currentDateTime()
{
    time_t    now = time(0);
    struct tm tstruct;
    char      buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

int satellite::EconstlNum(std::string source_name, std::string name_of_satellite)
{
    int couTr = 0;

    couTr = norad2kep_constel(source_name.c_str(), name_of_satellite.c_str());

    return (couTr);
}

int satellite::norad2kep_constel(const char* file, const char* name)
{
    char        cline1[100];
    char        cline2[100];
    char        cline3[100];
    char        holder[100];
    char *      str1, *str2;
    int         counter = 0;
    std::string status;

    FILE* pFile;
    pFile = fopen(file, "r");
    if (pFile == NULL)
    {
        perror("Error opening file");
    }
    else
    {
        status = "false";
        while (!feof(pFile) != 0)
        {
            fgets(cline1, MAX_STRING_SIZE, pFile);
            fgets(cline2, MAX_STRING_SIZE, pFile);
            fgets(cline3, MAX_STRING_SIZE, pFile);

            std::string str_a, str_b;
            str1  = strlwr(cline1);
            str_a = str1;

            strcpy(holder, name);
            str2  = strlwr(holder);
            str_b = str2;

            std::size_t found = str_a.find(str_b);
            if (found != std::string::npos)
            {
                counter = counter + 1;
                status  = "true";
            }
        }
    }

    if (status.compare("true") != 0)
    {
        perror("satellite with the name given is not in the list");
    }
    fclose(pFile);

    return counter;
}

void satellite::SatPOS_Wrapper_footPrint(int                 i,
                                         std::vector<double> ctime,
                                         sat_info            satInfo,
                                         States_TypeMatrix   states,
                                         double              coverage)
{

    // find the states
    std::vector<std::pair<std::pair<double, double>, double>> footPrint;

    footPrint                      = SatPOS_footPrint(ctime, satInfo, coverage);
    states->data[i][1 - 1].SatName = alloc_String(MAX_STRING_SIZE);
    strcpy(states->data[i][1 - 1].SatName, specified_sat_info->data[(int) (i)][1 - 1].name_of_satellite);
    for (int ii = 0; ii < footPrint.size(); ii++)
    {
        states->data[i][1 - 1].lat   = footPrint.at(ii).first.first;  // lat
        states->data[i][1 - 1].lon   = footPrint.at(ii).first.second; // lon
        states->data[i][1 - 1].radii = footPrint.at(ii).second;       // radi
    }

    for (int jj = 0; jj < ctime.size(); jj++)
    {
        states->data[i][1 - 1].ctime.data[jj][0] = ctime.at(jj);
    }
}

void satellite::SatSingPOS_Wrapper_footPrint(int                 i,
                                             std::vector<double> ctime,
                                             sat_info            satInfo,
                                             States_TypeMatrix   states,
                                             double              coverage)
{

    // find the states
    std::vector<std::pair<std::pair<double, double>, double>> footPrint;
    footPrint = SatPOS_footPrint(ctime, satInfo, coverage);

    states->data[i][1 - 1].SatName = alloc_String(MAX_STRING_SIZE);
    strcpy(states->data[i][1 - 1].SatName, specified_sat_info_direct->data[(int) (i)][1 - 1].name_of_satellite);
    for (int ii = 0; ii < footPrint.size(); ii++)
    {
        states->data[i][1 - 1].lat   = footPrint.at(ii).first.first;  // lat
        states->data[i][1 - 1].lon   = footPrint.at(ii).first.second; // lon
        states->data[i][1 - 1].radii = footPrint.at(ii).second;       // radi
    }

    for (int jj = 0; jj < ctime.size(); jj++)
    {
        states->data[i][1 - 1].ctime.data[jj][0] = ctime.at(jj);
    }
}

//[r    rdot]
std::vector<std::pair<std::pair<double, double>, double>>
    satellite::SatPOS_footPrint(std::vector<double> ctime, sat_info sat, double coverage)
{
    // given time convert into Julian date
    double a  = floor((14 - ctime.at(1)) / 12);
    double y  = ctime.at(0) + 4800 - a;
    double m  = ctime.at(1) + 12 * a - 3;
    double jd = ctime.at(2) + floor((153 * m + 2) / 5) + y * 365 + floor(y / 4) - floor(y / 100) + floor(y / 400) -
                32045 + (ctime.at(5) + 60 * ctime.at(4) + 3600 * (ctime.at(3) - 12)) / 86400;

    // time since epoch in minutes
    long double                            temp0  = jd;
    long double                            temp1  = sat.tle.at(0);
    long double                            temp2  = (temp0 - temp1);
    long double                            tsince = (temp2) * 1440;
    std::vector<std::pair<double, double>> sat_states;

    // orbit calculation by the MEX SGP4/SDP4 implementation
    sat_states = orbit(sat.epochN,
                       sat.tle.at(1),
                       sat.tle.at(2),
                       sat.tle.at(3),
                       sat.tle.at(4),
                       sat.tle.at(5),
                       sat.tle.at(6),
                       sat.tle.at(7),
                       sat.tle.at(8),
                       sat.tle.at(9),
                       sat.tle.at(10),
                       tsince);

    /* scale r and rdot
     * r [km] and rdot [km/s] */
    for (int ij = 0; ij < sat_states.size(); ij++)
    {
        sat_states.at(ij).first  = sat_states.at(ij).first * 6.378137e3;
        sat_states.at(ij).second = sat_states.at(ij).second * (6.378137e3 * 1.44e3 / 8.64e4);
    }

    std::vector<std::pair<double, double>> lat_lon_v;
    lat_lon_v = latlon(sat_states, jd);

    // convert to degrees
    lat_lon_v.at(0).first  = (lat_lon_v.at(0).first * 180) / pi;  // lat
    lat_lon_v.at(0).second = (lat_lon_v.at(0).second * 180) / pi; // lon

    if (lat_lon_v.at(0).second > 180)
    {
        lat_lon_v.at(0).second = lat_lon_v.at(0).second - 360;
    }

    if (lat_lon_v.at(0).first > 90)
    {
        lat_lon_v.at(0).first = lat_lon_v.at(0).first - 180;
    }

    double radi = footP_radius(sat_states, lat_lon_v.at(0).first, lat_lon_v.at(0).second, coverage);

    std::vector<std::pair<std::pair<double, double>, double>> lat_lon_out(1);

    lat_lon_out.at(0).first.first  = lat_lon_v.at(0).first;  // lat
    lat_lon_out.at(0).first.second = lat_lon_v.at(0).second; // lon
    lat_lon_out.at(0).second       = radi;                   // radius

    return lat_lon_out;
}

// calculates the geodetic position of an object given its ECI position pos and time
// function[lat, lon] = latlon(pos, ctime)
std::vector<std::pair<double, double>> satellite::latlon(std::vector<std::pair<double, double>> pos, double ctime)
{
    double fT = 3.35281066474748e-3;
    // double xkmper = 6.378137e3; defined already
    double theta, lon, rT, lat, phi, cT, e2;

    std::vector<std::pair<double, double>> lat_lon(1);

    // theta = actan2(pos(2), pos(1));						// radians
    theta = actan2(pos.at(1).first, pos.at(0).first);

    // lon = mod(theta - thetag(ctime), 2 * pi);				// radians
    double flag_1 = theta - thetag(ctime);
    double flag_2 = 2 * pi;
    lon           = mod_(flag_1, flag_2); // radians

    double flag_3 = (pos.at(0).first * pos.at(0).first) + (pos.at(1).first * pos.at(1).first);
    rT            = sqrt(flag_3);
    e2            = fT * (2 - fT);
    lat           = actan2(pos.at(2).first, rT);

    // initial step
    phi           = lat;
    double flag_4 = 1 - e2 * (sin(phi) * sin(phi));
    cT            = 1 / sqrt(flag_4);

    double flag_5 = pos.at(2).first + xkmper * cT * e2 * sin(phi);
    lat           = actan2(flag_5, rT);

    while (abs(lat - phi) >= 1e-10)
    {
        phi           = lat;
        double flag_6 = 1 - e2 * (sin(phi) * sin(phi));
        cT            = 1 / sqrt(flag_6);

        double flag_7 = pos.at(2).first + xkmper * cT * e2 * sin(phi);
        lat           = actan2(flag_7, rT);
    };

    if (lat > (pi / 2))
    {
        lat = lat - 2 * pi;
    }

    lat_lon.at(0).first  = lat;
    lat_lon.at(0).second = lon;

    return (lat_lon);
}

// four - quadrant arctan function
double satellite::actan2(double x, double y)
{
    double fqd;
    if (y < 0)
    {
        fqd = pi + atan(x / y);
        return (fqd);
    }
    if ((y == 0) && (x > 0))
    {
        fqd = pi / 2;
        return (fqd);
    }
    if ((y == 0) && (x <= 0))
    {
        fqd = 3 * pi / 2;
        return (fqd);
    }
    if ((y > 0) && (x > 0))
    {
        fqd = atan(x / y);
        return (fqd);
    }
    if ((y > 0) && (x <= 0))
    {
        fqd = 2 * pi + atan(x / y);
        return (fqd);
    }
}

// calculates the Greenwich Mean Sidereal Time
double satellite::thetag(double jd)
{
    double UT = (jd + 0.5) - floor(jd + 0.5);
    jd        = jd - UT;

    double TU = (jd - 2451545.0) / 36525;

    double tmp = 24110.54841 + 8640184.812866 * TU + ((0.093104 * TU) * (0.093104 * TU)) -
                 ((0.0000062 * TU) * (0.0000062 * TU) * (0.0000062 * TU));

    double temp_go = tmp + secday * omega_E * UT;

    tmp = mod_(temp_go, secday);

    double gmst = (2 * pi * tmp) / secday;

    return (gmst);
}

/* returns arg1 mod arg2 */
double satellite::mod_(double arg1, double arg2)
{
    int    i;
    double ret_val;

    ret_val = arg1;
    i       = ret_val / arg2;
    ret_val -= i * arg2;

    if (ret_val < 0.0)
        ret_val += arg2;

    return ret_val;
}

void satellite::print_footPrint(States_TypeMatrix states)
{
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Satellite Trajectory Simulator				  " << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "												  " << std::endl;
    std::cout << "by:											  " << std::endl;
    std::cout << "Rajiv Sithiravel and Thiagalingam Kirubarajan   " << std::endl;
    std::cout << "McMaster University							  " << std::endl;
    std::cout << "Hamilton, Ontario,							  " << std::endl;
    std::cout << "Canada.										  " << std::endl;
    std::cout << "												  " << std::endl;
    std::cout << "Current time:  " << currentDateTime() << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    for (int i = 0; i < states->m; i++)
    {
        std::cout << "Satellite Name: " << states->data[i][1 - 1].SatName << "\n";

        std::cout << "Time: "
                  << "\t"
                  << "[";
        for (int jj = 0; jj < states->data[i][1 - 1].ctime.m; jj++)
        {
            if (jj < 2)
            {
                std::cout << states->data[i][1 - 1].ctime.data[jj][0] << "-";
            }
            if (jj == 2)
            {
                std::cout << states->data[i][1 - 1].ctime.data[jj][0] << " ";
            }
            if ((jj > 2) && (jj < 5))
            {
                std::cout << states->data[i][1 - 1].ctime.data[jj][0] << ":";
            }
            if (jj == 5)
            {
                std::cout << states->data[i][1 - 1].ctime.data[jj][0];
            }
        }
        std::cout << "]";

        std::cout << "\n"
                  << "Foot Print Info: "
                  << "\n";
        std::cout << "\n"
                  << "latitude"
                  << "\t"
                  << "longitude"
                  << "\t"
                  << "radius"
                  << "\n";

        std::cout << states->data[i][1 - 1].lat << "\t" << states->data[i][1 - 1].lon << "\t"
                  << states->data[i][1 - 1].radii << "\n";

        std::cout << "\n\n";
    }
}

States_TypeMatrix satellite::footPrint(std::string name_of_satellite, std::vector<double> ctime, double coverage)
{
    States_TypeMatrix footPrint;

    footPrint = GetState_Fprint(name_of_satellite, ctime, coverage);

    return (footPrint);
}

States_TypeMatrix satellite::footPrints(std::vector<double> ctime, double coverage)
{
    States_TypeMatrix footPrint;

    footPrint = GetState_Fprints(ctime, coverage);

    return (footPrint);
}

double satellite::footP_radius(std::vector<std::pair<double, double>> states, double lat, double lon, double coverage)
{

    double              x = states.at(0).first * 1000;
    double              y = states.at(1).first * 1000;
    double              z = states.at(2).first * 1000;
    std::vector<double> answ;

    answ = ecef2lla(x, y, z);

    double flag_1 = deg2rad * (coverage / 2);

    double rad_flag = tan(flag_1) * answ.at(2);

    double R      = 6378137;
    double flag_2 = deg2rad * lat;
    double flag_3 = deg2rad * lon;
    double x1     = R * cos(flag_2) * cos(flag_3) + rad_flag;
    double y1     = R * cos(flag_2) * sin(flag_3) + rad_flag;
    double z1     = R * sin(flag_2);
    double flag_4 = z1 / R;
    double lat4   = asin(flag_4);
    double lon4   = atan2(y1, x1);
    double flag_5 = (180 / pi) * lon4;
    double alfa   = abs(abs(lon) - abs(flag_5));

    return (alfa);
}

// function[lat, lon, alt] = ecef2lla(x, y, z)
std::vector<double> satellite::ecef2lla(double x, double y, double z)
{

    // WGS84 ellipsoid constants :
    double a = 6378137;
    double e = 8.1819190842622e-2;

    std::vector<double> ecellA(3);

    // calculations:
    double flag_1, flag_2, flag_3, flag_4, flag_5;
    flag_1     = a * a * (1 - (e * e));
    double b   = sqrt(flag_1);
    flag_2     = ((a * a) - (b * b)) / (b * b);
    double ep  = sqrt(flag_2);
    flag_3     = (x * x) + (y * y);
    double p   = sqrt(flag_3);
    flag_4     = a * z;
    flag_5     = b * p;
    double th  = atan2(flag_4, flag_5);
    double lon = atan2(y, x);
    double fl6, fl7, fl8;
    fl6        = (z + (ep * ep) * b * (sin(th) * sin(th) * sin(th)));
    fl7        = (p - (e * e) * a * (cos(th) * cos(th) * cos(th)));
    double lat = atan2(fl6, fl7);
    fl8        = 1 - (e * e) * (sin(lat) * sin(lat));
    double fl9 = sqrt(fl8);
    double N   = a / fl9;
    double alt = (p / cos(lat)) - N;

    // return lon in range[0, 2 * pi)
    lon = mod_(lon, 2 * pi);

    // correct for numerical instability in altitude near exact poles :
    // (after this correction, error is about 2 millimeters, which is about
    // the same as the numerical precision of the overall function)

    // k = abs(x) < 1 & abs(y) < 1;
    // alt(k) = abs(z(k)) - b;

    if ((abs(x) < 1) && (abs(y) < 1))
    {
        alt = abs(z) - b;
    }

    ecellA.at(0) = lat;
    ecellA.at(1) = lon;
    ecellA.at(2) = alt;

    return (ecellA);
}
