/********************************************************************************
** Form generated from reading UI file 'kmainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KMAINWINDOW_H
#define UI_KMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KMainWindowClass
{
public:
    QWidget *centralWidget;
    QPushButton *m_prevButton;
    QPushButton *m_nextButton;
    QPushButton *m_playButton;
    QSlider *m_volumeSlider;
    QPushButton *m_volumeButton;
    QLabel *m_volumeLabel;
    QLabel *m_albumCoverLabel;
    QSlider *m_timeSlider;
    QLabel *m_currentTimeLabel;
    QLabel *m_totalTimeLabel;
    QPushButton *m_sequentialButton;
    QPushButton *m_randomButton;
    QPushButton *m_loopOneButton;
    QPushButton *m_onlineButton;
    QPushButton *m_localButton;
    QStackedWidget *m_stackedWidget;
    QWidget *page;
    QLineEdit *m_pLineEdit;
    QPushButton *m_searchButton;
    QTableWidget *m_onlineTableWidget;
    QTextBrowser *m_lyricsBrowser;
    QWidget *page_2;
    QTableWidget *m_localTableWidget;
    QTextBrowser *m_localLyricsBrowser;
    QPushButton *m_addMusicButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *KMainWindowClass)
    {
        if (KMainWindowClass->objectName().isEmpty())
            KMainWindowClass->setObjectName(QStringLiteral("KMainWindowClass"));
        KMainWindowClass->resize(782, 535);
        centralWidget = new QWidget(KMainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        m_prevButton = new QPushButton(centralWidget);
        m_prevButton->setObjectName(QStringLiteral("m_prevButton"));
        m_prevButton->setGeometry(QRect(20, 350, 61, 28));
        m_nextButton = new QPushButton(centralWidget);
        m_nextButton->setObjectName(QStringLiteral("m_nextButton"));
        m_nextButton->setGeometry(QRect(160, 350, 61, 28));
        m_playButton = new QPushButton(centralWidget);
        m_playButton->setObjectName(QStringLiteral("m_playButton"));
        m_playButton->setGeometry(QRect(90, 350, 61, 28));
        m_playButton->setStyleSheet(QLatin1String("QPushButton {\n"
"    qproperty-icon: url(:/images/play.png);\n"
"    \n"
"}"));
        m_volumeSlider = new QSlider(centralWidget);
        m_volumeSlider->setObjectName(QStringLiteral("m_volumeSlider"));
        m_volumeSlider->setGeometry(QRect(310, 330, 22, 71));
        m_volumeSlider->setOrientation(Qt::Vertical);
        m_volumeButton = new QPushButton(centralWidget);
        m_volumeButton->setObjectName(QStringLiteral("m_volumeButton"));
        m_volumeButton->setGeometry(QRect(230, 350, 61, 28));
        m_volumeButton->setStyleSheet(QLatin1String("QPushButton {\n"
"    qproperty-icon: url(:/images/open.png);\n"
"    \n"
"}"));
        m_volumeLabel = new QLabel(centralWidget);
        m_volumeLabel->setObjectName(QStringLiteral("m_volumeLabel"));
        m_volumeLabel->setGeometry(QRect(340, 360, 21, 20));
        QFont font;
        font.setPointSize(12);
        m_volumeLabel->setFont(font);
        m_albumCoverLabel = new QLabel(centralWidget);
        m_albumCoverLabel->setObjectName(QStringLiteral("m_albumCoverLabel"));
        m_albumCoverLabel->setGeometry(QRect(30, 20, 280, 280));
        m_albumCoverLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    border-radius: 140px; /* \350\256\276\347\275\256\345\234\206\350\247\222\345\215\212\345\276\204\344\270\27250% */\n"
"    \n"
"    border: 2px solid black;\n"
"    background-image: url(:/images/disk.png);\n"
"    background-position: center;\n"
"    background-repeat: no-repeat;\n"
"    background-size: cover;\n"
"}\n"
"\n"
""));
        m_timeSlider = new QSlider(centralWidget);
        m_timeSlider->setObjectName(QStringLiteral("m_timeSlider"));
        m_timeSlider->setGeometry(QRect(50, 410, 231, 22));
        m_timeSlider->setOrientation(Qt::Horizontal);
        m_currentTimeLabel = new QLabel(centralWidget);
        m_currentTimeLabel->setObjectName(QStringLiteral("m_currentTimeLabel"));
        m_currentTimeLabel->setGeometry(QRect(50, 440, 51, 16));
        m_totalTimeLabel = new QLabel(centralWidget);
        m_totalTimeLabel->setObjectName(QStringLiteral("m_totalTimeLabel"));
        m_totalTimeLabel->setGeometry(QRect(240, 440, 51, 16));
        m_sequentialButton = new QPushButton(centralWidget);
        m_sequentialButton->setObjectName(QStringLiteral("m_sequentialButton"));
        m_sequentialButton->setGeometry(QRect(390, 350, 71, 28));
        m_sequentialButton->setStyleSheet(QString::fromUtf8("QPushButton:checked {\n"
"    background-color: lightblue; /* \350\256\276\347\275\256\351\253\230\344\272\256\346\227\266\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"    border: 2px solid #ADD8E6;\n"
"}"));
        m_sequentialButton->setCheckable(true);
        m_randomButton = new QPushButton(centralWidget);
        m_randomButton->setObjectName(QStringLiteral("m_randomButton"));
        m_randomButton->setGeometry(QRect(580, 350, 71, 28));
        m_randomButton->setStyleSheet(QString::fromUtf8("QPushButton:checked {\n"
"    background-color: lightblue; /* \350\256\276\347\275\256\351\253\230\344\272\256\346\227\266\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"    border: 2px solid #ADD8E6;\n"
"}"));
        m_randomButton->setCheckable(true);
        m_loopOneButton = new QPushButton(centralWidget);
        m_loopOneButton->setObjectName(QStringLiteral("m_loopOneButton"));
        m_loopOneButton->setGeometry(QRect(480, 350, 71, 28));
        m_loopOneButton->setStyleSheet(QString::fromUtf8("QPushButton:checked {\n"
"    background-color: lightblue; /* \350\256\276\347\275\256\351\253\230\344\272\256\346\227\266\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"    border: 2px solid #ADD8E6;\n"
"}"));
        m_loopOneButton->setCheckable(true);
        m_onlineButton = new QPushButton(centralWidget);
        m_onlineButton->setObjectName(QStringLiteral("m_onlineButton"));
        m_onlineButton->setGeometry(QRect(320, 50, 71, 28));
        m_onlineButton->setStyleSheet(QString::fromUtf8("QPushButton:checked {\n"
"    background-color: lightblue; /* \350\256\276\347\275\256\351\253\230\344\272\256\346\227\266\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"    border: 2px solid #ADD8E6;\n"
"}"));
        m_onlineButton->setCheckable(false);
        m_localButton = new QPushButton(centralWidget);
        m_localButton->setObjectName(QStringLiteral("m_localButton"));
        m_localButton->setGeometry(QRect(320, 90, 71, 28));
        m_localButton->setStyleSheet(QString::fromUtf8("QPushButton:checked {\n"
"    background-color: lightblue; /* \350\256\276\347\275\256\351\253\230\344\272\256\346\227\266\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"    border: 2px solid #ADD8E6;\n"
"}"));
        m_localButton->setCheckable(false);
        m_stackedWidget = new QStackedWidget(centralWidget);
        m_stackedWidget->setObjectName(QStringLiteral("m_stackedWidget"));
        m_stackedWidget->setGeometry(QRect(410, 0, 361, 321));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        m_pLineEdit = new QLineEdit(page);
        m_pLineEdit->setObjectName(QStringLiteral("m_pLineEdit"));
        m_pLineEdit->setGeometry(QRect(0, 10, 281, 31));
        m_searchButton = new QPushButton(page);
        m_searchButton->setObjectName(QStringLiteral("m_searchButton"));
        m_searchButton->setGeometry(QRect(290, 10, 61, 28));
        m_onlineTableWidget = new QTableWidget(page);
        if (m_onlineTableWidget->columnCount() < 3)
            m_onlineTableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        m_onlineTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        m_onlineTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        m_onlineTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        m_onlineTableWidget->setObjectName(QStringLiteral("m_onlineTableWidget"));
        m_onlineTableWidget->setGeometry(QRect(0, 50, 371, 192));
        m_onlineTableWidget->setStyleSheet(QStringLiteral(""));
        m_onlineTableWidget->setColumnCount(3);
        m_onlineTableWidget->horizontalHeader()->setDefaultSectionSize(115);
        m_lyricsBrowser = new QTextBrowser(page);
        m_lyricsBrowser->setObjectName(QStringLiteral("m_lyricsBrowser"));
        m_lyricsBrowser->setGeometry(QRect(0, 270, 371, 51));
        m_stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        m_localTableWidget = new QTableWidget(page_2);
        if (m_localTableWidget->columnCount() < 1)
            m_localTableWidget->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        m_localTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem3);
        m_localTableWidget->setObjectName(QStringLiteral("m_localTableWidget"));
        m_localTableWidget->setGeometry(QRect(0, 50, 371, 192));
        m_localTableWidget->setColumnCount(1);
        m_localTableWidget->horizontalHeader()->setDefaultSectionSize(120);
        m_localLyricsBrowser = new QTextBrowser(page_2);
        m_localLyricsBrowser->setObjectName(QStringLiteral("m_localLyricsBrowser"));
        m_localLyricsBrowser->setGeometry(QRect(0, 270, 371, 51));
        m_stackedWidget->addWidget(page_2);
        m_addMusicButton = new QPushButton(centralWidget);
        m_addMusicButton->setObjectName(QStringLiteral("m_addMusicButton"));
        m_addMusicButton->setGeometry(QRect(320, 210, 71, 28));
        m_addMusicButton->setStyleSheet(QString::fromUtf8("QPushButton:checked {\n"
"    background-color: lightblue; /* \350\256\276\347\275\256\351\253\230\344\272\256\346\227\266\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"    border: 2px solid #ADD8E6;\n"
"}"));
        m_addMusicButton->setCheckable(false);
        KMainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(KMainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 782, 26));
        KMainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(KMainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        KMainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(KMainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        KMainWindowClass->setStatusBar(statusBar);

        retranslateUi(KMainWindowClass);

        m_stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(KMainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *KMainWindowClass)
    {
        KMainWindowClass->setWindowTitle(QApplication::translate("KMainWindowClass", "KMainWindow", Q_NULLPTR));
        m_prevButton->setText(QApplication::translate("KMainWindowClass", "\344\270\212\344\270\200\351\246\226", Q_NULLPTR));
        m_nextButton->setText(QApplication::translate("KMainWindowClass", "\344\270\213\344\270\200\351\246\226", Q_NULLPTR));
        m_playButton->setText(QString());
        m_volumeButton->setText(QString());
        m_volumeLabel->setText(QApplication::translate("KMainWindowClass", "0", Q_NULLPTR));
        m_albumCoverLabel->setText(QString());
        m_currentTimeLabel->setText(QApplication::translate("KMainWindowClass", "00:00", Q_NULLPTR));
        m_totalTimeLabel->setText(QApplication::translate("KMainWindowClass", "00:00", Q_NULLPTR));
        m_sequentialButton->setText(QApplication::translate("KMainWindowClass", "\351\241\272\345\272\217\346\222\255\346\224\276", Q_NULLPTR));
        m_randomButton->setText(QApplication::translate("KMainWindowClass", "\351\232\217\346\234\272\346\222\255\346\224\276", Q_NULLPTR));
        m_loopOneButton->setText(QApplication::translate("KMainWindowClass", "\345\215\225\346\233\262\345\276\252\347\216\257", Q_NULLPTR));
        m_onlineButton->setText(QApplication::translate("KMainWindowClass", "\347\275\221\347\273\234\346\255\214\346\233\262", Q_NULLPTR));
        m_localButton->setText(QApplication::translate("KMainWindowClass", "\346\234\254\345\234\260\346\255\214\346\233\262", Q_NULLPTR));
        m_searchButton->setText(QApplication::translate("KMainWindowClass", "\346\220\234\347\264\242", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = m_onlineTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("KMainWindowClass", "\346\255\214\346\233\262", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = m_onlineTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("KMainWindowClass", "\344\270\223\350\276\221", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem2 = m_onlineTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("KMainWindowClass", "\346\227\266\351\225\277", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem3 = m_localTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("KMainWindowClass", "\346\255\214\346\233\262", Q_NULLPTR));
        m_addMusicButton->setText(QApplication::translate("KMainWindowClass", "\346\267\273\345\212\240\346\226\207\344\273\266", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class KMainWindowClass: public Ui_KMainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KMAINWINDOW_H
