#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include "util.h"

#include <exception>
#include <string>
#include <unordered_map>

namespace models
{

class Maintenance
{
public:
    Maintenance();

    /**
     * @param timeSlot sould be in the past.
     * timeSlot also have to be maximum 24 hours in length. Throws TooLongTimeSlot
     *  exception if this precondition is not met.
     */
    void updateMaintenance(const util::Coord& coord, const util::TimeSlot& timeSlot);

    std::unordered_map<std::string, int> getMaintanance();

    class TooLongTimeSlot: std::exception
    {
    public:
        const char* what()
        {
            return "Time Slot to retrieve maintenance data is max 24h.";
        }
    };

private:
    std::unordered_map<std::string, int> data;
};

}

#endif
