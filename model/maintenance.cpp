#include "maintenance.h"

#include <iostream>
#include <stdlib.h>

namespace models
{

Maintenance::Maintenance()
{



    // test data
    this->pieData = {{"kakku",1 }, {"pala",2}};
}

void Maintenance::updateMaintenance()
{

}



 std::unordered_map<std::string, int> Maintenance::getMaintanance()
 {
     if( this->pieData.empty())
     {
         std::cerr << "No maintanance data" << std::endl;
         return {};
     }
     return this->pieData;
 }

}
