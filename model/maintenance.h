#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include <string>

namespace models
{

class Maintenance
{
public:
    Maintenance();
    void updateMaintenance();

private:
    int id;
    std::string heading;
    std::string description;
    std::string startTime;
    std::string endtime;

    struct {
        int xMin;
        int yMin;
        int xMax;
        int yMax;
    } boundingBox;
};

}

#endif
