#include <QApplication>

#include "gui/MainWindow.h"
#include "controller/controller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();

    return app.exec();
}