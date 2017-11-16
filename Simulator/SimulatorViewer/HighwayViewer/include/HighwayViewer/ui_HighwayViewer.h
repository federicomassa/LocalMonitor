/********************************************************************************
** Form generated from reading UI file and copied in this folder
**
** Created by: Qt User Interface Compiler version 5.9.2
********************************************************************************/

#ifndef HIGHWAY_VIEWER_UI_H
#define HIGHWAY_VIEWER_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

#include <iostream>

QT_BEGIN_NAMESPACE

class Ui_HighwayViewer
{
public:
    QWidget *centralWidget;
    QGraphicsView *graphicsView;

    void setupUi(QMainWindow *MainWindow)
    {

		
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("HighwayViewer"));
		
        MainWindow->resize(400, 300);
		
        centralWidget = new QWidget(MainWindow);

        centralWidget->setObjectName(QStringLiteral("centralWidget"));

        graphicsView = new QGraphicsView(centralWidget);

        graphicsView->setObjectName(QStringLiteral("graphicsView"));

        graphicsView->setGeometry(QRect(80, 40, 256, 192));

        MainWindow->setCentralWidget(centralWidget);


        retranslateUi(MainWindow);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class HighwayViewer: public Ui_HighwayViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
