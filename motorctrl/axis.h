#ifndef AXIS_H
#define AXIS_H
// 1PPS 对应 0.0127 mm   // Pulse equivalent

#define CARDS_COUNT   1
#define PULSE_EQUIVALENT  0.0127  // mm / pps
#define CONVERT_TO_PLUSE(length) (static_cast<int>(length / PULSE_EQUIVALENT))
#define CONVERT_TO_LENGTH(pulse) (static_cast<double>(pulse * PULSE_EQUIVALENT))
typedef unsigned char BYTE;

enum Status
{
    EL_MINUS = 0,    // 下限位
    EL_PLUS = 1,     // 上限位
    ORG = 2,         // 原点
    STP = 3,
    STA = 4,
    SD_MINUS = 5,
    SD_PLUS = 6,
    RESERVE = 7
};

#define STATUS(status, bit) (((status) & (1 << bit)) >> (bit))
#define _LOW 0
#define _HIGH 20000


class Axis
{   
public:
    Axis();
    int SetPlsMode(int nId, int nMode);
    static bool CreateAxis(Axis*& pAxis, int nCount, int nMode);
    static void RelaseAxis(Axis*& pAxis);
    static double GetPosition(Axis* pAixs);
    bool GetHomeFlag() { return m_bHome; }
    void Continuous(short nDir, double dMaxSpeed);
    void ImmediateStop();
    void ZeroPulse();

    BYTE GetStatus();
    double GetPostion();

    void KeyboardLeft();
    void KeyboardRight();
    void HomeMove(double dMaxSpeed);

private:
    short m_nDir = 0;      // 运动时轴移动的方向(-1, 1, 单位矢量)
    short m_nId = -1;      // 轴号索引
    double m_nDist = 0.0;  // 轴移动的距离
    bool m_bHome = false;
};

#endif // AXIS_H
