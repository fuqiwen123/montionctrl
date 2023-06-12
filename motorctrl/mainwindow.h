#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "axis.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int nCount, QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void SetStringInLabelCtrl(double dPos);

private slots:
    void on_m_immdateStop_clicked();

    void on_ORG_clicked();

    void on_ZeroPulse_clicked();

private:
    Ui::MainWindow *ui;

private:
    int m_nCount = 0;
    Axis* m_pAxis = NULL;
    int m_nDir = 0;
    QTimer m_timer;
    int m_timerId = 0;
};
#endif // MAINWINDOW_H
