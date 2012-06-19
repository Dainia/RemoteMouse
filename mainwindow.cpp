#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>
#include <winuser.h>
#include <winable.h>

//从这里开始
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_tcpSocket = NULL;

    trayicon = new QSystemTrayIcon(this);
    QIcon icon(":/images/images/remote_mouse.png");
    trayiconMenu = new QMenu(this);
    trayiconMenu->addAction(ui->actionQuit);
    trayiconMenu->addSeparator();
    trayicon->setIcon(icon);
    trayicon->show();
    trayicon->setToolTip(tr("退出"));
    trayicon->setContextMenu(trayiconMenu);
    connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason)));

    m_tcpServer = new QTcpServer(this);
    m_tcpServer->listen(QHostAddress::Any,51706);
    connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnect()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Left)
    {
        POINT pt;
        GetCursorPos(&pt);
        SetCursorPos(pt.x-10,pt.y);
    }
    else if(e->key()==Qt::Key_Up)
    {

        LButtonClick();
    }
}

void MainWindow::onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        this->show();
        break;
    }
}

void MainWindow::on_actionQuit_triggered()
{
    trayicon->hide();
    this->close();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(trayicon->isVisible())
    {
        this->hide();
        e->ignore();
    }
}

void MainWindow::LButtonClick()
{
    mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
    mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
}

//void MainWindow::LButtonDBClick()
//{
//    mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
//    mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);

//    mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
//    mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
//}

void MainWindow::RButtonClick()
{
    mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
    mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
}

void MainWindow::RButtonDBClick()
{
    mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
    mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);

    mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
    mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
}

void MainWindow::newConnect()
{
    qDebug()<<"rrrr";
    if(m_tcpSocket==NULL)
    {
        qDebug()<<"bbbbbb";
        m_tcpSocket = m_tcpServer->nextPendingConnection();
        connect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    }
    qDebug()<<"eeeee";
}

void MainWindow::readMessage()
{
    if(m_tcpSocket->bytesAvailable()>0)
    {
       QByteArray qba;
       qba.resize(m_tcpSocket->bytesAvailable());
       m_tcpSocket->read(qba.data(),qba.size());
       QString msg = qba.data();
       QStringList list = msg.split(";");
       if(list[0] == "a")
       {
           LButtonClick();
       }
       if(list[0] == "b")
       {
           RButtonClick();
       }
       if(list[0] == "c")
       {
           cur_pos_x = atof(list[1].toAscii().constData());
           cur_pos_y = atof(list[2].toAscii().constData());
           //           POINT pt;
//           GetCursorPos(&pt);
//           SetCursorPos(pt.x-4.5*atoi(list[1].toAscii().constData()),pt.y-4.5*atoi(list[2].toAscii().constData()));
       }
       if(list[0] == "d")
       {
           float dlatx = atof(list[1].toAscii().constData())-cur_pos_x;
           float dlaty = atof(list[2].toAscii().constData())-cur_pos_y;
           POINT pt;
           GetCursorPos(&pt);           
           ::SendInput(NULL,NULL,NULL);
           INPUT input_event;
           input_event.type = INPUT_MOUSE;
           input_event.mi.dx = pt.x+dlatx;
           input_event.mi.dy = pt.y+dlaty;
           input_event.mi.dwFlags = MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE;
           input_event.mi.mouseData = NULL;
           input_event.mi.dwExtraInfo = NULL;
           input_event.mi.time = NULL;
           SendInput(1,&input_event,sizeof(INPUT));
           //SetCursorPos(pt.x+dlatx,pt.y+dlaty);
//           float k = dlaty/dlatx;
//           if(dlatx>=0){
//               for(int i=0;i<dlatx;i++)
//               {
//                    SetCursorPos(pt.x+i,pt.y+i*k);
//                    Sleep(1);
//               }
//           }else{
//               for(int i=-1;i>dlatx;i--)
//               {
//                    SetCursorPos(pt.x+i,pt.y+i*k);
//                    Sleep(1);
//               }
//           }
           cur_pos_x = atof(list[1].toAscii().constData());
           cur_pos_y = atof(list[2].toAscii().constData());
//           qDebug()<<(int)(pt.x+dlatx);
//           qDebug()<<(int)(pt.y+dlaty);
       }
    }

}
