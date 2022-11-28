#include "timeStamp.h"

#include <QTest>

#include <iostream>

class timeStamp_test: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {}

    void test()
    {
        for (int i = -24; i < 25; ++i)
        {
            std::cout << isoDateHoursFromNow(i) << std::endl;
        }
    }


    void cleanupTestCase() {}
};


QTEST_APPLESS_MAIN(timeStamp_test)
#include "timestamp_test.moc"
