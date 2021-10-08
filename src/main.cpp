#include "Icon.h"
#include "ComplainUtf8.h"
#include "mainwindow.h"

#include <QTranslator>
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <Windows.h>

#define __APP_NAME__ "OCC_QT_Demo"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(mdi);
    QApplication app(argc, argv);
	app.setWindowIcon(QIcon(ICON_APP));
    QCoreApplication::setApplicationName(__APP_NAME__);
    QCoreApplication::setOrganizationName("���ߣ�bilibili Ģ��_AJUNE");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription(__APP_NAME__" v1.0");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("�ļ�", "���ļ�.");
    parser.process(app);

    MainWindow mainWin;
    foreach (const QString &fileName, parser.positionalArguments())
        mainWin.openFile(fileName);
	//MbdUI::SetUIer(new QT_UIer(&mainWin)); //����ȫ�ֶԻ���

    mainWin.show();
    return app.exec();
}
