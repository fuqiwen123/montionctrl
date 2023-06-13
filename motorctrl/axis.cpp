#include "axis.h"
#include "Dmc1000.h"
#include <math.h>
#include "QDebug.h"

Axis::Axis()
{
    m_bHome = false;
}

int Axis::SetPlsMode(int nId, int nMode)
{
    m_nId = nId;

    if (m_nId == 0 || m_nId == 2)
        return d1000_set_pls_outmode(m_nId, 1);

    return d1000_set_pls_outmode(m_nId, 0);
}

bool Axis::CreateAxis(Axis*& pAxis, int nCount, int nMode)
{
    if (pAxis)
        return false;

    int nRes;
    if (nCount == CARDS_COUNT)
        nRes = 0;
    else if (nCount > 0 && nCount < CARDS_COUNT)
        nRes = 1;
    else
        nRes = -1;

    if (nRes != 0)
        return false;

    pAxis = new Axis[nCount];
    if (!pAxis)
        return false;


    for(short i = 0; i < (nCount << 2); i++)
    {
        nRes = pAxis[i].SetPlsMode(i, nMode);
        if (nRes)
        {
            delete []pAxis;
            pAxis = nullptr;
            return false;
        }
    }

    return true;
}

void Axis::RelaseAxis(Axis*& pAxis)
{
    if (!pAxis)
        return;

    delete []pAxis;
    pAxis = nullptr;
}

double Axis::GetPosition(Axis* pAixs)
{
    return pAixs->GetPostion();
}

void Axis::Continuous(short nDir, double dMaxSpeed)
{
    if (m_nId == -1)
        return;

    m_bHome = false;
    if (d1000_check_done(m_nId))
        return;

    long nMaxVel = CONVERT_TO_PLUSE(dMaxSpeed);
    long nMinVel = nMaxVel / 10;
    m_nDir = nDir;
    d1000_start_sv_move(0, m_nDir * nMinVel, m_nDir * nMaxVel, 0.1);
}

void Axis::ImmediateStop()
{
    if (m_nId == -1)
        return;

    d1000_immediate_stop(m_nId);
}

void Axis::ZeroPulse()
{
    if (m_nId == -1)
        return;

    d1000_set_command_pos(m_nId, 0);
}

BYTE Axis::GetStatus()
{
    return static_cast<BYTE>(d1000_get_axis_status(m_nId));
}

double Axis::GetPostion()
{
    long nPos = d1000_get_command_pos(m_nId);

    return CONVERT_TO_LENGTH(nPos);
}

void Axis::KeyboardLeft()
{
    if (m_nId == -1)
        return;

    m_bHome = false;
    BYTE nRes = (BYTE)d1000_get_axis_status(m_nId);
    long nPos = d1000_get_command_pos(m_nId);

    m_nDir = -1;
    if (STATUS(nRes, ORG))  // 利用硬限(原点传感器)位停止
    {
        qDebug() << "nPos = " << nPos << Qt::endl;
        ImmediateStop();
        // d1000_set_command_pos(m_nId, 0);
    }
    else
    {
        d1000_start_sv_move(m_nId, -100, -1000, 0.1);
    }
}

/*
void Axis::KeyboardRight()
{
    if (m_nId == -1)
        return;

    m_bHome = false;
    long nPos = d1000_get_command_pos(m_nId);


    m_nDir = 1;
    if (nPos <= _HIGH)    // 软限位
    {
        d1000_start_sv_move(m_nId, 100, 1000, 0.1);
    }
    else
    {
        d1000_immediate_stop(m_nId);
    }
} */


void Axis::KeyboardRight(int nUp)
{
    if (m_nId == -1)
        return;

    m_bHome = false;
    long nPos = d1000_get_command_pos(m_nId);

    m_nDir = 1;
    if (nPos <= nUp)    // 软限位
    {
        d1000_start_sv_move(m_nId, 100, 1000, 0.1);
    }
    else
    {
        d1000_immediate_stop(m_nId);
    }
}

void Axis::HomeMove(short nDir, double dMaxSpeed)
{
    if (d1000_check_done(m_nId) == 0)
        return;

    long nSpeed = CONVERT_TO_PLUSE(dMaxSpeed);
    if (nSpeed > 0)
        nSpeed = -nSpeed;

    // m_nDir = nDir;
    long nMaxVel = nSpeed;
    long nMinVel = nMaxVel / 10;

    m_bHome = true;
    d1000_home_move(m_nId, nMinVel, nMaxVel, 0.1);
}
