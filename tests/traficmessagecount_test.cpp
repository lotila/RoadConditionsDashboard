#include "APIC/apiclient.h"
#include "model/trafficmessages.h"

#include <QTest>

#include <iostream>

void timeSeriesPrettyPrinter(const std::vector<util::TimeValuePair>& values)
{
    for (const util::TimeValuePair& pair : values)
    {
        std::cout << std::to_string(pair.time) << " => " << std::to_string(pair.value) << std::endl;
    }
}

class TraficMessageCount_test: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        APIClient::INIT_STATUS status = APIClient::initializeAPIClient();

        QVERIFY(status == APIClient::INIT_STATUS::OK);


    }

    void test()
    {
        models::TrafficMessages msgs;

        msgs.updateTrafficMessageCount({60.2, 24.9}, {-100, 0});

        std::vector<util::TimeValuePair> result = msgs.getTrafficMessageCount();

        timeSeriesPrettyPrinter(result);

        QVERIFY(result.size() > 0);
    }


    void cleanupTestCase()
    {
        APIClient::destructAPIClient();
    }
};


QTEST_APPLESS_MAIN(TraficMessageCount_test)
#include "traficmessagecount_test.moc"
