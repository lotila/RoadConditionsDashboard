#ifndef TRAFFICMESSAGES_H
#define TRAFFICMESSAGES_H

#include <string>

namespace models
{

class TrafficMessages
{
public:
    TrafficMessages();
    void updateTrafficMessages();

private:
    // Jos vaan määrä, niin näitä ei tarvita
    int id;
    std::string type;

    // Miltä alueelta tm:iä otetaan
    struct {
        int xMin;
        int yMin;
        int xMax;
        int yMax;
    } boundingBox;
};

}

#endif
