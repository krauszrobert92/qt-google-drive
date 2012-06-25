#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include "AppRegData.h"
#include <QKeyEvent>

Ui::MainWindow* UiInstance::ui = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    UiInstance::Instance()->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete UiInstance::ui;
}

void MainWindow::init(void)
{
    qDebug() << "MainWindow::init";

    if(!CheckReg()) return;

    driveEngine.reset(new DriveEngine(this));
    driveEngine->init();
    driveEngine->slotCheckWorkDir(false);

    setConnections();

    UiInstance::ui->foldersView->installEventFilter(this);
    UiInstance::ui->filesView->installEventFilter(this);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
}

void MainWindow::setConnections(void)
{
    connect(UiInstance::ui->actionMenuLogin, SIGNAL(triggered()), driveEngine.data(), SLOT(slotStartLoginFromMenu()));
    connect(UiInstance::ui->actionMenuQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(UiInstance::ui->actionMenuDownload, SIGNAL(triggered()), driveEngine.data(), SLOT(slotDownload()));
    connect(UiInstance::ui->actionDownload, SIGNAL(triggered()), driveEngine.data(), SLOT(slotDownload()));
    connect(UiInstance::ui->actionMenuUpload, SIGNAL(triggered()), driveEngine.data(), SLOT(slotUpload()));
    connect(UiInstance::ui->actionUpload, SIGNAL(triggered()), driveEngine.data(), SLOT(slotUpload()));
    connect(UiInstance::ui->actionMenuSettings, SIGNAL(triggered()), driveEngine.data(), SLOT(slotCheckWorkDir()));
    connect(UiInstance::ui->actionSettings, SIGNAL(triggered()), driveEngine.data(), SLOT(slotCheckWorkDir()));
    connect(driveEngine->getOAuth2(), SIGNAL(loginDone()), this, SLOT(slotloginDone()));
    connect(driveEngine->getFoldersManager(), SIGNAL(signalAccessTokenRequired()), driveEngine.data(), SLOT(slotStartLogin()));
    connect(this, SIGNAL(signalDel(QObject*)), driveEngine.data(), SLOT(slotDel(QObject*)));
    connect(UiInstance::ui->actionMenuDelete, SIGNAL(triggered()), driveEngine.data(), SLOT(slotTriggeredDel()));
    connect(UiInstance::ui->actionDelete, SIGNAL(triggered()), driveEngine.data(), SLOT(slotTriggeredDel()));
    connect(UiInstance::ui->actionMenuCreateFolder, SIGNAL(triggered()), driveEngine.data(), SLOT(slotCreateFolder()));
    connect(UiInstance::ui->actionCreateFolder, SIGNAL(triggered()), driveEngine.data(), SLOT(slotCreateFolder()));
}

Ui::MainWindow* UiInstance::Instance()
{
    if(!UiInstance::ui) UiInstance::ui = new Ui::MainWindow;

    return UiInstance::ui;
}

void MainWindow::slotloginDone(void)
{
    qDebug() << "MainWindow::slotloginDone";
    init();
}

void MainWindow::slotUpdateFileList()
{
    qDebug() << "MainWindow::slotUpdateFileList";
    driveEngine->showFiles();
}

bool MainWindow::CheckReg(void)
{
    bool regState = true;

    if(CLIENT_ID == "YOUR_CLIENT_ID_HERE" || REDIRECT_URI == "YOUR_REDIRECT_URI_HERE" || CLIENT_SECRET == "YOUR_CLIENT_SECRET")
    {
        regState = false;

        // TODO: change link to wiki page
        QMessageBox::warning(this, "Warning",
                             "To work with application you need to register your own application in <b>Google</b>.\n"
                             "Learn more from <a href='http://code.google.com/p/qt-google-drive/'>here</a>");
    }

    return regState;
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->key() == Qt::Key_Delete) {
            emit signalDel(object);
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}
