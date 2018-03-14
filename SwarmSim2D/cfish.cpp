#include "cfish.h"
#include<QDebug>
#include<algorithm>
#include<math.h>

template<typename T> T sqDist(T diff1,T diff2)
{
    return diff1*diff1+diff2*diff2;
}


float CFish::fScale=0.015f;
const float CFish::fTankSize = 2.0f+2.0f*CFish::fScale;
const float CFish::fHalfTanksSize = fTankSize/2.0;
int CFish::iFishCount=5000;
QVector<CFish> CFish::fishList;
QVector<CFishPos> CFish::posList;
QVector<CVec2> CFish::fishForces;
QVector<QVector<int>> CFish::fishNeighbourhoods;
QVector<QVector<int>> CFish::fishProxyhoods;
QVector<SFishColor> CFish::fishColor;
const float CSwarmForceValue::fFishSize = CFish::fScale*2.0f;
CSwarmForceValue CFish::alignment = CSwarmForceValue(10,1.0,0.5);
CSwarmForceValue CFish::coherence = CSwarmForceValue(10,1.0,0.5);
CSwarmForceValue CFish::repulsion = CSwarmForceValue(2,1.0,0.5);
int CFish::iRecalcForcesDelay = 50;
int CFish::iRecallNeighbourhoodDelay= 100;
float fRepusliondoubleRange;
float fMaxDist;


CFishPos::CFishPos()
    :id(0),x(0.0f){}

CFishPos::CFishPos(int id,float x ,float y)
    :id(id),x(x),y(y){}

void CFishPos::set(int id, float x, float y)
{
    this->id=id;
    this->x=x;
    this->y=y;
}

CFish::CFish()
    :x(0.0f),y(0.0f),vx(0.0f),vy(0.0f)
{}

CFish::CFish(float x, float y, float vx, float vy)
    :x(x),y(y),vx(vx),vy(vy)
{}



void CFish::move(float dT, CVec2 a)
{
    vx+=a.x*dT;
    vy+=a.y*dT;
    x+=vx*dT;
    y+=vy*dT;
    if (x>fHalfTanksSize)
        x-=fTankSize;
    if (x<-fHalfTanksSize)
        x+=fTankSize;
    if (y>fHalfTanksSize)
        y-=fTankSize;
    if (y<-fHalfTanksSize)
        y+=fTankSize;
}

void CFish::moveFishes(float dT)
{
    for(int i=0;i<fishList.size();i++){
        fishList[i].move(dT,fishForces[i]);
    }
}

void CFish::sortFishPos()
{
    std::sort(posList.begin(),posList.end());
}

void CFish::calcForces()
{

}


inline void CFish::checkforNeighbour(const CFishPos &curr, const CFishPos &neigh,QVector<int> &hood,QVector<int> &prox, float xdiff)
{

    float ydiff;
    if (neigh.y>curr.y)
        ydiff=neigh.y-curr.y;
    else
        ydiff=curr.y-neigh.y;
    if (ydiff>fHalfTanksSize)
        ydiff=fTankSize-ydiff;
    float dist=ydiff+xdiff;
    if (dist<fMaxDist)
    {
        hood.append(neigh.id);
        if (dist<fRepusliondoubleRange)
            prox.append(neigh.id);
    }
}


void CFish::findNeighbourhoods()
{

    fMaxDist=std::max(alignment.fRange,std::max(coherence.fRange,repulsion.fRange));
    fRepusliondoubleRange = CFish::repulsion.fRange*2.0f;
    int iReserveSize = fishList.size() / std::max(1,(int) (fTankSize/(fMaxDist*3)));
    int iReserveProxSize=fishList.size() / std::max(1,(int) (fTankSize/(repulsion.fRange*3)));
    for(int i=0; i<posList.size();i++)
    {
        int iOverWrapMaxLowerIndex=-1;
        const CFishPos &curr=posList[i];
        QVector<int> &hood=fishNeighbourhoods[curr.id];
        QVector<int> &proxy=fishProxyhoods[curr.id];
        hood.clear();
        hood.reserve(iReserveSize);
        proxy.clear();
        proxy.reserve(iReserveProxSize);
        int j=i+1;
        for(;j<posList.size();j++)
        {
            const CFishPos &neigh=posList[j];
            float xdiff=neigh.x-curr.x;
            if (xdiff>fMaxDist)
                break;
            checkforNeighbour(curr,neigh,hood,proxy,xdiff);
        }
        int iFirstSearchEndedAt=j;
        if (j==posList.size())
        {
            float xpos=curr.x-fTankSize;
            for(j=0;j<i;j++)
            {
                const CFishPos &neigh=posList[j];
                float xdiff=neigh.x-xpos;
                if (xdiff>fMaxDist)
                    break;
                checkforNeighbour(curr,neigh,hood,proxy,xdiff);
            }
            iOverWrapMaxLowerIndex=j;
        }

        for(j=i-1;j>iOverWrapMaxLowerIndex;j--)
        {
            const CFishPos &neigh=posList[j];
            float xdiff=curr.x-neigh.x;
            if (xdiff>fMaxDist)
                break;
            checkforNeighbour(curr,neigh,hood,proxy,xdiff);

        }
        if (j<0)
        {
            for(j=posList.size()-1;j>=iFirstSearchEndedAt;j--)
            {
                float xpos=curr.x+fTankSize;
                const CFishPos &neigh=posList[j];
                float xdiff=xpos-neigh.x;
                if (xdiff>fMaxDist)
                    break;
                checkforNeighbour(curr,neigh,hood,proxy,xdiff);
            }
        }
    }
}



CSwarmForceValue::CSwarmForceValue()
    :fRangeFL(10.0f),fRange(10.0f*CSwarmForceValue::fFishSize),fFaLLOffGamma(1.0f),fForce(1.0f){}

CSwarmForceValue::CSwarmForceValue(float fRangeFL, float fFaLLOffGamma, float fForce)
    :fRangeFL(fRangeFL),fRange(fRangeFL*fFishSize),fFaLLOffGamma(fFaLLOffGamma),fForce(fForce){}


void CSwarmForceValue::setForceValues(float fRangeFL, float fFaLLOffGamma, float fForce)
{
    this->fRangeFL=fRangeFL;
    this->fRange=fRangeFL*CSwarmForceValue::fFishSize;
    this->fFaLLOffGamma=fFaLLOffGamma;
    this->fForce=fForce;
}

void CSwarmForceValue::setRange(float fRangeFL)
{
    this->fRangeFL=fRangeFL;
    this->fRange=fRangeFL*fFishSize;
}
