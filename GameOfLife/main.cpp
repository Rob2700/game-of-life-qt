#include "guimanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GUIManager w;
    w.show();
    return QCoreApplication::exec();
}
