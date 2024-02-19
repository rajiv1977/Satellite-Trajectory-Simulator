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

int main(int argc, char* argv[])
{
    // declaration
    satellite         sat;
    sat_info          satInfo;
    std::string       config_filename;
    States_TypeMatrix states, state;

    config_filename = "test_0.xml"; // location of the config file
    std::vector<double> ctime(6);   // time
    ctime.at(0) = 2024;             // year
    ctime.at(1) = 10;               // month
    ctime.at(2) = 31;               // day
    ctime.at(3) = 1;                // hour
    ctime.at(4) = 10;               // min
    ctime.at(5) = 20;               // sec

    // Input from Config file
    sat.sat_config(config_filename); // configuration (given user info)
    states = sat.GetStates(ctime);   // determine the states for user info
    sat.printf_states(states);       // output states [position (km), velocity (Km/s)]

    // Input of desire satellite or constellation
    std::string name_of_sat =
        "COSMOS 475"; // name of a satellite or a constellation eg., VANGUARD 1, COSMOS 475, GLONASS, NAVSTAR
    state = sat.GetState(name_of_sat, ctime); // determine the states for a given sat or constl for a given time
    sat.printf_states(state);                 // output states [position (km), velocity (Km/s)]

    // Foot print calculation of satellite or constellation
    double coverage = 5; // Coverage (degree)

    // Input of desire satellite or constellation
    state = sat.footPrint(name_of_sat, ctime, coverage); // foot print calculations for a desire satellite/constellation
    sat.print_footPrint(state);                          // foot print output [lat, lon, radi]

    // Input from Config file
    state = sat.footPrints(ctime, coverage); // for user input foot print calculations
    sat.print_footPrint(state);              // foot print output [lat, lon, radi]

    return 0;
}
