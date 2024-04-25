#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication::setApplicationName("myMusicPlayer");
    QApplication::setOrganizationName("CrystallisR");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QApplication app(argc, argv);

    QFile themeFile(":css/styles/normalTheme.css");
    themeFile.open(QFile::ReadOnly);
    QString appStyleSheet(themeFile.readAll());
    app.setStyleSheet(appStyleSheet);

    app.setWindowIcon(QIcon(":icons/res/music_note_r1.png"));

    MainWindow w;
    w.show();
    return app.exec();
}
