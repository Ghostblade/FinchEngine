/********************************************************************************
** Form generated from reading UI file 'finchengine.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINCHENGINE_H
#define UI_FINCHENGINE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <glwidget.h>

QT_BEGIN_NAMESPACE

class Ui_FinchEngineClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    GLWidget *openGLWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FinchEngineClass)
    {
        if (FinchEngineClass->objectName().isEmpty())
            FinchEngineClass->setObjectName(QStringLiteral("FinchEngineClass"));
        FinchEngineClass->resize(1024, 767);
        centralWidget = new QWidget(FinchEngineClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        openGLWidget = new GLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));

        gridLayout->addWidget(openGLWidget, 0, 0, 1, 1);

        FinchEngineClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(FinchEngineClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FinchEngineClass->setStatusBar(statusBar);

        retranslateUi(FinchEngineClass);

        QMetaObject::connectSlotsByName(FinchEngineClass);
    } // setupUi

    void retranslateUi(QMainWindow *FinchEngineClass)
    {
        FinchEngineClass->setWindowTitle(QApplication::translate("FinchEngineClass", "FinchEngine", 0));
    } // retranslateUi

};

namespace Ui {
    class FinchEngineClass: public Ui_FinchEngineClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINCHENGINE_H
