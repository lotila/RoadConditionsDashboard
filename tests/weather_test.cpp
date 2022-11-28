#include "APIC/apiclient.h"
#include "model/weather.h"
#include "util.h"

#include <QTest>

#include <iostream>

void timeSeriesPrettyPrinter(const std::vector<util::TimeValuePair>& values)
{
    for (const util::TimeValuePair& pair : values)
    {
        std::cout << std::to_string(pair.time) << " => " << std::to_string(pair.value) << std::endl;
    }
}

class Weather_test: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        APIClient::INIT_STATUS status = APIClient::initializeAPIClient();

        QVERIFY(status == APIClient::INIT_STATUS::OK);


    }

    void temperatureTest()
    {
        models::Weather weather;

        weather.updateTemperature({60.2, 24.9}, {-12, 24});

        std::vector<util::TimeValuePair> result = weather.getTemperature();

        timeSeriesPrettyPrinter(result);

        QVERIFY(result.size() > 0);
    }

    void windTest()
    {
        models::Weather weather;

        weather.updateWind({60.2, 24.9}, {-12, 24});

        std::vector<util::TimeValuePair> result = weather.getWind();

        timeSeriesPrettyPrinter(result);

        QVERIFY(result.size() > 0);
    }

    void rainTest()
    {
        models::Weather weather;

        weather.updateRain({60.2, 24.9}, {-12, 24});

        std::vector<util::TimeValuePair> result = weather.getRain();

        timeSeriesPrettyPrinter(result);

        QVERIFY(result.size() > 0);
    }

    void cloudinessTest()
    {
        models::Weather weather;

        weather.updateCloudiness({60.2, 24.9}, {-12, 24});

        std::vector<util::TimeValuePair> result = weather.getCloudiness();

        timeSeriesPrettyPrinter(result);

        QVERIFY(result.size() > 0);
    }


    void cleanupTestCase()
    {
        APIClient::destructAPIClient();
    }
};


QTEST_APPLESS_MAIN(Weather_test)
#include "weather_test.moc"
