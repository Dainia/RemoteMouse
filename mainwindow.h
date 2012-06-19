#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui/qevent.h>
#include <QtGui/QSystemTrayIcon>
#include <QtNetwork>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void LButtonClick();
    void LButtonDBClick();
    void RButtonClick();
    void RButtonDBClick();


protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);
    void keyPressEvent(QKeyEvent *e);


private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayicon;
    QMenu *trayiconMenu;
    QTcpServer *m_tcpServer;
    QTcpSocket *m_tcpSocket;
    float cur_pos_x;
    float cur_pos_y;

private slots:
    void on_actionQuit_triggered();
    void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void newConnect();
    void readMessage();
};

#endif // MAINWINDOW_H
