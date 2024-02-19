#ifndef __DISTANCELATLONG_H__
#define __DISTANCELATLONG_H__

#define DEG2RAD(x) (3.1415926535897*(x)/180.0)
#define RAD2DEG(x) ((x)*180.0/3.1415926535897)

#define KILOMETER2METER(x)						((x)*1000.0)
#define FOOT2METER(x)							((x)*0.3048)
#define NAUTICAL_MILE2METER(x)					((x)*1852.0)
#define METER2NAUTICAL_MILE(x)					((x)/1852.0)
#define METER2KILOMETER(x)						((x)/1000.0)
#define METER2FOOT(x)							((x)/0.3048)

#define EARTH_RADIUS		6378.137

#define MEASUREMENT_UNIT_UNDEFINED			-1
#define MEASUREMENT_UNIT_NONE				0
#define MEASUREMENT_UNIT_METER				1
#define MEASUREMENT_UNIT_KILOMETER			2
#define MEASUREMENT_UNIT_NAUTICAL_MILE		3
#define MEASUREMENT_UNIT_FOOT				4
#define MEASUREMENT_UNIT_DEGREE				5
#define MEASUREMENT_UNIT_RADIAN				6
#define MEASUREMENT_UNIT_METER_PER_SECOND	7
#define MEASUREMENT_UNIT_KILOMETER_PER_HOUR	8
#define MEASUREMENT_UNIT_KNOT				9
#define MEASUREMENT_UNIT_FOOT_PER_SECOND	10
#define MEASUREMENT_UNIT_HZ					11
#define MEASUREMENT_UNIT_DEGREE_PER_SECOND	12
#define MEASUREMENT_UNIT_RADIAN_PER_SECOND	13
#define MEASUREMENT_UNIT_DB					14
#define MEASUREMENT_UNIT_SECOND				15
#define MEASUREMENT_UNIT_HOUR				16
#define MEASUREMENT_UNIT_MINUTE             17
#define MEASUREMENT_UNIT_FOOT_PER_MINUTE	18

UISOFT_BEGIN_NAMESPACE(backend)

class distancelatlong
{
public:

	/** Convert the distance from one unit to another */
	static double FTX_distance_unit_conversion(int current_unit, int new_unit, double distance)
	{
		double new_distance;

		if (current_unit == new_unit)
		{
			return distance;
		}
		else
		{
			switch (current_unit)
			{
			case MEASUREMENT_UNIT_METER:
				switch (new_unit)
				{
				case MEASUREMENT_UNIT_KILOMETER:
					new_distance = METER2KILOMETER(distance);
					break;
				case MEASUREMENT_UNIT_NAUTICAL_MILE:
					new_distance = METER2NAUTICAL_MILE(distance);
					break;
				case MEASUREMENT_UNIT_FOOT:
					new_distance = METER2FOOT(distance);
					break;
				default:
					new_distance = distance;	//CHECK
					break;
				}
				break;
			case MEASUREMENT_UNIT_KILOMETER:
				switch (new_unit)
				{
				case MEASUREMENT_UNIT_METER:
					new_distance = KILOMETER2METER(distance);
					break;
				case MEASUREMENT_UNIT_NAUTICAL_MILE:
					new_distance = KILOMETER2METER(distance);
					new_distance = METER2NAUTICAL_MILE(new_distance);
					break;
				case MEASUREMENT_UNIT_FOOT:
					new_distance = KILOMETER2METER(distance);
					new_distance = METER2FOOT(new_distance);
					break;
				default:
					new_distance = distance;	//CHECK
					break;
				}
				break;
			case MEASUREMENT_UNIT_NAUTICAL_MILE:
				switch (new_unit)
				{
				case MEASUREMENT_UNIT_METER:
					new_distance = NAUTICAL_MILE2METER(distance);
					break;
				case MEASUREMENT_UNIT_KILOMETER:
					new_distance = NAUTICAL_MILE2METER(distance);
					new_distance = METER2KILOMETER(new_distance);
					break;
				case MEASUREMENT_UNIT_FOOT:
					new_distance = NAUTICAL_MILE2METER(distance);
					new_distance = METER2FOOT(new_distance);
					break;
				default:
					new_distance = distance;	//CHECK
					break;
				}
				break;
			case MEASUREMENT_UNIT_FOOT:
				switch (new_unit)
				{
				case MEASUREMENT_UNIT_METER:
					new_distance = FOOT2METER(distance);
					break;
				case MEASUREMENT_UNIT_KILOMETER:
					new_distance = FOOT2METER(distance);
					new_distance = METER2KILOMETER(new_distance);
					break;
				case MEASUREMENT_UNIT_NAUTICAL_MILE:
					new_distance = FOOT2METER(distance);
					new_distance = METER2NAUTICAL_MILE(new_distance);
					break;
				default:
					new_distance = distance;	//CHECK
					break;
				}
				break;
			default:
				new_distance = distance;	//CHECK
				break;
			}
		}

		return new_distance;
	}




	static double FTP_find_geographical_distance(double lat1,double lon1, double lat2, double lon2, int units_for_distance, int units_for_angle)
	{
		/** haversine formula http://www.movable-type.co.uk/scripts/latlong.html */
		double distance, dlon, dlat, a, c;

		if (units_for_angle == MEASUREMENT_UNIT_DEGREE)
		{
			lat1 = DEG2RAD(lat1);
			lon1 = DEG2RAD(lon1);
			lat2 = DEG2RAD(lat2);
			lon2 = DEG2RAD(lon2);
		}

		dlon = (lon2 - lon1);
		dlat = (lat2 - lat1);
		a =  pow(sin(dlat * 0.5),2)+ cos(lat1) * cos(lat2) * pow(sin(dlon * 0.5),2);
		c = 2.0 * atan2(sqrt(a), sqrt(1-a));
		distance = EARTH_RADIUS * c;

		distance = FTX_distance_unit_conversion(MEASUREMENT_UNIT_KILOMETER, units_for_distance, distance);

		return distance;

	}

	static double FTP_find_distance_between_longitudes(double lat, double dLon, int units_for_distance, int units_for_angle)
	{
		/** http://calgary.rasc.ca/latlong.htm */
		double distance;

		if (units_for_angle == MEASUREMENT_UNIT_DEGREE)
		{
			lat = DEG2RAD(lat);        
		}

		if (units_for_angle == MEASUREMENT_UNIT_RADIAN)
		{
			dLon = RAD2DEG(dLon);
		}    

		distance = (111.41288*cos(lat) - 0.09350*cos(3*lat) + 0.00012*cos(5*lat))*dLon;

		distance = FTX_distance_unit_conversion(MEASUREMENT_UNIT_KILOMETER, units_for_distance, distance);

		return distance;
	}

	static double FTP_find_distance_between_latitudes(double lat, double dLat, int units_for_distance, int units_for_angle)
	{
		/** http://calgary.rasc.ca/latlong.htm */
		double distance;

		if (units_for_angle == MEASUREMENT_UNIT_DEGREE)
		{
			lat = DEG2RAD(lat);        
		}

		if (units_for_angle == MEASUREMENT_UNIT_RADIAN)
		{
			dLat = RAD2DEG(dLat);
		}

		distance = (111.13295 - 0.55982*cos(2*lat) + 0.00117*cos(4*lat))*dLat;

		distance = FTX_distance_unit_conversion(MEASUREMENT_UNIT_KILOMETER, units_for_distance, distance);

		return distance;
	}

	static double FTP_find_delta_longitude(double lat, double d, int units_for_distance, int units_for_angle)
	{
		/** http://calgary.rasc.ca/latlong.htm */
		double delta, distance_per_degree;

		if (units_for_angle == MEASUREMENT_UNIT_DEGREE)
		{
			lat = DEG2RAD(lat);        
		}

		distance_per_degree = 111.41288*cos(lat) - 0.09350*cos(3*lat) + 0.00012*cos(5*lat);

		d = FTX_distance_unit_conversion(units_for_distance, MEASUREMENT_UNIT_KILOMETER, d);

		delta = d/distance_per_degree;

		if (units_for_angle == MEASUREMENT_UNIT_RADIAN)
		{
			delta = DEG2RAD(delta);
		}

		return delta;
	}

	static double FTP_find_delta_latitude(double lat, double d, int units_for_distance, int units_for_angle)
	{
		/** http://calgary.rasc.ca/latlong.htm */
		double delta, distance_per_degree;

		if (units_for_angle == MEASUREMENT_UNIT_DEGREE)
		{
			lat = DEG2RAD(lat);        
		}

		distance_per_degree = 111.13295 - 0.55982*cos(2*lat) + 0.00117*cos(4*lat);

		d = FTX_distance_unit_conversion(units_for_distance, MEASUREMENT_UNIT_KILOMETER, d);

		delta = d/distance_per_degree;

		if (units_for_angle == MEASUREMENT_UNIT_RADIAN)
		{
			delta = DEG2RAD(delta);
		}

		return delta;
	}

};

UISOFT_END_NAMESPACE
#endif // __DISTANCELATLONG_H__