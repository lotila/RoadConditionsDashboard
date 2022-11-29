#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include <string>
#include <unordered_map>



namespace models
{

class Maintenance
{
public:
    Maintenance();
    void updateMaintenance();

    std::unordered_map<std::string, int> getMaintanance();

private:


    std::unordered_map<std::string,int> pieData;
    //int id;
    //std::string heading;
    //std::string description;
    //std::string startTime;
    //std::string endtime;

    //struct {
    //    int xMin;
    //    int yMin;
    //    int xMax;
    //    int yMax;
    //} boundingBox;
};

}

#endif
