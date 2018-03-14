#ifndef CFISH_H
#define CFISH_H

#include <QtGlobal>
#include <QVector>
#include <QVector2D>
#include <QElapsedTimer>
#include <QMutex>

struct SFishColor
{
    float r=0.6f;
    float g=0.6f;
    float b=1.0f;
};

class CSwarmForceValue
{
public:
    CSwarmForceValue();
    CSwarmForceValue(float fRangeFL, float fFaLLOffGamma, float fForce);
    void setForceValues(float fRangeFL, float fFaLLOffGamma, float fForce);
    void setRange(float fRangeFL);
    float fRangeFL;
    float fRange;
    float fFaLLOffGamma;
    float fForce;
    const static float fFishSize;
};


class CFishPos
{
public:
    CFishPos();
    CFishPos(int id, float x, float y);
    void set(int id, float x, float y);
    int id=0;
    float x=0.0f;
    float y=0.0f;
};


class CVec2
{
public:
    inline CVec2():x(0.0f),y(0.0f){}
    inline CVec2(float x, float y):x(x),y(y){}
    float x,y;
};

inline bool operator< (const CFishPos& lhs, const CFishPos& rhs){ return lhs.x<rhs.x; }
inline bool operator> (const CFishPos& lhs, const CFishPos& rhs){ return rhs < lhs; }
inline bool operator<=(const CFishPos& lhs, const CFishPos& rhs){ return !(lhs > rhs); }
inline bool operator>=(const CFishPos& lhs, const CFishPos& rhs){ return !(lhs < rhs); }

class CFish
{
public:
    CFish();
    CFish(float x, float y, float vx, float vy);
    float x,y,vx,vy;
    void move( float dT, CVec2 a = CVec2());
    static const float fTankSize;
    static const float fHalfTanksSize;
    static float fScale;
    static int iFishCount;
    static QVector<CFish> fishList;
    static QVector<CFishPos> posList;
    static QVector<CVec2> fishForces;
    static QVector<QVector<int>> fishNeighbourhoods;
    static QVector<QVector<int>> fishProxyhoods;
    static QVector<SFishColor> fishColor;
    static void moveFishes(float dT);
    static void sortFishPos();
    static void calcForces();
    static void findNeighbourhoods();
    inline static void checkforNeighbour(const CFishPos &, const CFishPos &,QVector<int> &, QVector<int> &, float);
    static CSwarmForceValue alignment;
    static CSwarmForceValue coherence;
    static CSwarmForceValue repulsion;
    static int iRecallNeighbourhoodDelay;
    static int iRecalcForcesDelay;
};



#endif // CFISH_H
