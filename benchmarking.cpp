#include <QtWidgets>
#include <qtest.h>
#include <booleanlatticemodel.h>

class TestBenchmark : public QObject
{
    Q_OBJECT
private slots:
    void blModel_prepare();
    void blModel();
};

void TestBenchmark::blModel_prepare()
{
    QTest::addColumn<int>("atomsNum");
    QTest::newRow("64atoms") << 64;
}

void TestBenchmark::blModel()
{
    QFETCH(int, atomsNum);

    BooleanLatticeModel model;
    for(int i = 0; i < atomsNum; ++i){
        model.addAtom(QString::number(i));
    }

    QBENCHMARK{
        model.createBLItems();
    }

    QBENCHMARK{
        model.createLines();
    }

    Q_UNUSED(model);
}

//QTEST_MAIN(TestBenchmark)
