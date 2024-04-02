#include <QApplication>
#include "./Classes/Header/MainWindow.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Image Editor and Viewer");
    window.show();
    return QApplication::exec();
}
