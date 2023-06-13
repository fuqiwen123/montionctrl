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

    ui->XCheckBox->setChecked(true);
    ui->YCheckBox->setChecked(true);
    ui->ZCheckBox->setChecked(true);

    QObject::connect(ui->XCheckBox, &QCheckBox::clicked, this, &MainWindow::CheckAxis);

    QObject::connect(ui->YCheckBox, &QCheckBox::clicked, this, &MainWindow::CheckAxis);

    QObject::connect(ui->ZCheckBox, &QCheckBox::clicked, this, &MainWindow::CheckAxis);


    if(!Axis::CreateAxis(m_pAxis, m_nCount, 1))
    {
        QMessageBox::critical(this, "错误", "创建轴对象失败");
        exit(0);
    }
    setFocusPolicy(Qt::StrongFocus);
    // setFocus();

    m_xTimer.setInterval(50);
    // m_xTimerId = m_xTimer.timerId();
    m_yTimer.setInterval(50);
    // m_yTimerId = m_yTimer.timerId();
    m_zTimer.setInterval(50);
    // m_zTimerId = m_zTimer.timerId();

    QObject::connect(&m_xTimer, &QTimer::timeout, this, [&](){
        if (!m_bXCheck)
            return;

        if (!m_pAxis[0].GetHomeFlag())
            return;

        SetXAxisLabel(Axis::GetPosition(&m_pAxis[0]));
    });

    QObject::connect(&m_yTimer, &QTimer::timeout, this, [&](){
        if (!m_bYCheck)
            return;

        if (!m_pAxis[1].GetHomeFlag())
            return;

        SetYAxisLabel(Axis::GetPosition(&m_pAxis[1]));
    });

    QObject::connect(&m_zTimer, &QTimer::timeout, this, [&](){
        if (!m_bZCheck)
            return;

        if (!m_pAxis[2].GetHomeFlag())
            return;

        SetZAxisLabel(Axis::GetPosition(&m_pAxis[2]));
    });
}

MainWindow::~MainWindow()
{
    Axis::RelaseAxis(m_pAxis);
    delete ui;
}

void MainWindow::CheckAxis(bool checked)
{
    QObject* p=sender();
    if(p==ui->XCheckBox)
        m_bXCheck = checked;
    else if(p==ui->YCheckBox)
        m_bYCheck = checked;
    else if(p==ui->ZCheckBox)
        m_bZCheck = checked;
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
    {
        if (m_bXCheck)
        {
            m_pAxis[0].KeyboardLeft();
            SetXAxisLabel(Axis::GetPosition(&m_pAxis[0]));
        }

        if (m_bYCheck)
        {
            d1000_set_pls_outmode(1, 1);
            m_pAxis[1].KeyboardLeft();
            SetYAxisLabel(Axis::GetPosition(&m_pAxis[1]));
        }

        if (m_bZCheck)
        {

            m_pAxis[2].KeyboardLeft();
            SetZAxisLabel(Axis::GetPosition(&m_pAxis[2]));
        }
        break;
    }
    case Qt::Key_Right:

        if (m_bXCheck)
        {
            m_pAxis[0].KeyboardRight(20000);
            SetXAxisLabel(Axis::GetPosition(&m_pAxis[0]));
        }

        if (m_bYCheck)
        {
            d1000_set_pls_outmode(1, 1);
            m_pAxis[1].KeyboardRight(10000);
            SetYAxisLabel(Axis::GetPosition(&m_pAxis[1]));
        }

        if (m_bZCheck)
        {
            m_pAxis[2].KeyboardRight(10000);
            SetZAxisLabel(Axis::GetPosition(&m_pAxis[2]));
        }
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
        if (m_bXCheck)
        {
            m_pAxis[0].ImmediateStop();
        }

        if (m_bYCheck)
        {
            m_pAxis[1].ImmediateStop();
        }

        if (m_bZCheck)
        {
            m_pAxis[2].ImmediateStop();
        }

        break;
    default:
        QMainWindow::keyReleaseEvent(event);
    }
}

void MainWindow::SetXAxisLabel(double dPos)
{
    ui->x_pos_label->setText(QString("%1 %2").arg("位置:").arg(dPos, 0, 'f', 2));
}


void MainWindow::SetYAxisLabel(double dPos)
{
    ui->y_pos_label->setText(QString("%1 %2").arg("位置:").arg(dPos, 0, 'f', 2));
}

void MainWindow::SetZAxisLabel(double dPos)
{
    ui->z_pos_label->setText(QString("%1 %2").arg("位置:").arg(dPos, 0, 'f', 2));
}

void MainWindow::on_m_immdateStop_clicked()
{
    if (m_bXCheck)
    {
        m_pAxis[0].ImmediateStop();
        m_xTimer.stop();
    }

    if (m_bYCheck)
    {
        m_pAxis[1].ImmediateStop();
        m_yTimer.stop();
    }

    if (m_bZCheck)
    {
        m_pAxis[2].ImmediateStop();
        m_zTimer.stop();
    }
}


void MainWindow::on_ORG_clicked()
{
    if (m_bXCheck)
    {
        m_pAxis[0].HomeMove(1, 12.7);
        m_xTimer.start(); // 啟動計時器
        m_xTimerId = m_xTimer.timerId();
    }

    if (m_bYCheck)
    {
        m_pAxis[1].HomeMove(1, 12.7);
        m_yTimer.start(); // 啟動計時器
        m_yTimerId = m_yTimer.timerId();
    }

    if (m_bZCheck)
    {
        m_pAxis[2].HomeMove(1, 12.7);
        m_zTimer.start(); // 啟動計時器
        m_zTimerId = m_zTimer.timerId();
    }
}


void MainWindow::on_ZeroPulse_clicked()
{
    if (m_bXCheck)
    {
        SetXAxisLabel(0.0);
        m_pAxis[0].ZeroPulse();
    }

    if (m_bYCheck)
    {
        SetYAxisLabel(0.0);
        m_pAxis[1].ZeroPulse();
    }

    if (m_bZCheck)
    {
        SetZAxisLabel(0.0);
        m_pAxis[2].ZeroPulse();
    }
}

