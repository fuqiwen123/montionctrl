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
    void SetXAxisLabel(double dPos);
    void SetYAxisLabel(double dPos);
    void SetZAxisLabel(double dPos);

private slots:
    void on_m_immdateStop_clicked();

    void on_ORG_clicked();

    void on_ZeroPulse_clicked();
    void CheckAxis(bool checked);

private:
    Ui::MainWindow *ui;


private:
    int m_nCount = 0;
    Axis* m_pAxis = NULL;
    int m_nDir = 0;

    QTimer m_xTimer;
    int m_xTimerId = 0;

    QTimer m_yTimer;
    int m_yTimerId = 0;

    QTimer m_zTimer;
    int m_zTimerId = 0;

    bool m_bXCheck = true;
    bool m_bYCheck = true;
    bool m_bZCheck = true;
};
#endif // MAINWINDOW_H
