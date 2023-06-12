#include "mainwindow.h"
#include <QMessageBox>
#include <QKeyEvent>
#include "Dmc1000.h"
#include "axis.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(int nCount, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_nCount(nCount)
{
    ui->setupUi(this);
    if(!Axis::CreateAxis(m_pAxis, m_nCount, 1))
    {
        QMessageBox::critical(this, "错误", "创建轴对象失败");
        exit(0);
    }
    setFocusPolicy(Qt::StrongFocus);
    d1000_set_pls_outmode(0,1);
    setFocus();

    m_timer.setInterval(100); // 設置計時器的時間間隔為 1000 毫秒
    m_timerId = m_timer.timerId();
    QObject::connect(&m_timer, &QTimer::timeout, this, [&](){
        if (!m_pAxis[0].GetHomeFlag())
            return;
        SetStringInLabelCtrl(Axis::GetPosition(&m_pAxis[0]));
    });
}

MainWindow::~MainWindow()
{
    QObject::killTimer(m_timerId);
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
    {
        m_pAxis[0].KeyboardLeft();
        SetStringInLabelCtrl(Axis::GetPosition(&m_pAxis[0]));
        break;
    }
    case Qt::Key_Right:
        m_pAxis[0].KeyboardRight();
        SetStringInLabelCtrl(Axis::GetPosition(&m_pAxis[0]));
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat() == true)  // 持续方向键按下(只处理一次)
    {
        QMainWindow::keyReleaseEvent(event);
        return;
    }

    switch (event->key())
    {
    case Qt::Key_Left:
    case Qt::Key_Right:
        m_pAxis[0].ImmediateStop();
        break;
    default:
        QMainWindow::keyReleaseEvent(event);
    }
}

void MainWindow::SetStringInLabelCtrl(double dPos)
{
    ui->pos_label->setText(QString("%1 %2").arg("位置:").arg(dPos, 0, 'f', 2));
}

void MainWindow::on_m_immdateStop_clicked()
{
    m_pAxis[0].ImmediateStop();
}


void MainWindow::on_ORG_clicked()
{
    m_pAxis[0].HomeMove(12.7);
    m_timer.start(); // 啟動計時器
}


void MainWindow::on_ZeroPulse_clicked()
{
    m_pAxis[0].ZeroPulse();
    m_timer.stop();  // 暂停定时器
    ui->pos_label->setText(QString("%1").arg(0.00, 0, 'f', 2));
}

