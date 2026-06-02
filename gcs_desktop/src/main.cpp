#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    // high resalution support
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    
    QApplication app(argc, argv);
    
    MainWindow w;
    w.setWindowTitle("Ground Control Station");
    w.resize(1024, 768);
    w.show();
    
    return app.exec();
}