#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

class Airport{
    private:
    //write down all the varioable present in the .data file
    int airportID;
    std::string name;
    std::string city;
    std::string country;
    std::string iata;
    std::string icao;
    double latitude;
    double longitude;
    int altitude;
    int timezone;
    char dst;
    
    public:
    Airport() : airportID(0), latitude(0.0), longitude(0.0) {}
    



    
};

