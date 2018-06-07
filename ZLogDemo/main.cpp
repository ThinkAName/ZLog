#include "Dialog.h"
#include <QApplication>
#include "zLog/z_log.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    ZLogger(LM_UNKNOWN, LT_UNKNOWN, LL_DEBUG, "Good job!");

    return a.exec();
}
