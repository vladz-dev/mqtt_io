#include "jobhandler/worker.h"

#include <QCoreApplication>


void workerFunc();

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Worker w;
    w.run();

    return app.exec();
}

