#include "myglwidget.h"
#include "QDebug"
#include <QtMath>
#include <QElapsedTimer>

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent), QOpenGLFunctions_4_0_Core(),
    vScale(1.0f,1.0f)
{

}


void MyGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    connect(&updateTimer,SIGNAL(timeout()),this,SLOT(updateFishies()));
    connect(&recalcForcesTimer,SIGNAL(timeout()),this,SLOT(recalcForces()));
    connect(&recalcNeighbourhoodTimer,SIGNAL(timeout()),this,SLOT(recalcNeighbourhoods()));

        m_program = new QOpenGLShaderProgram(this);
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/Fish.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/Fish.frag");
        m_program->link();
        qDebug() << "Fish Shader Log: " << m_program->log();

        createFishes();
        updateBuffers();
}

void MyGLWidget::paintGL()
{
    iFrameCounter++;
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->bind();
    glBindVertexArray(VAOs[FishBody]);

    m_program->setUniformValue("scale",vScale);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glBindBuffer(GL_ARRAY_BUFFER,Buffers[FishDataBuffer]);
    glBufferData(GL_ARRAY_BUFFER,CFish::fishList.size()*sizeof(CFish),&(CFish::fishList.first().x), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(vFishPosition);
    glVertexAttribPointer(vFishPosition,2,GL_FLOAT,GL_FALSE,4*sizeof(float),BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vFishVelocity);
    glVertexAttribPointer(vFishVelocity,2,GL_FLOAT,GL_FALSE,4*sizeof(float),BUFFER_OFFSET(2*sizeof(float)));
    glVertexAttribDivisor(vFishPosition,1);
    glVertexAttribDivisor(vFishVelocity,1);


    glDrawElementsInstanced(GL_TRIANGLES,iFishIndices,GL_UNSIGNED_SHORT,BUFFER_OFFSET(0),CFish::fishList.size());

    glBindVertexArray(0);
 }

void MyGLWidget::resizeGL(int w, int h)
{
    if (w>h)
    {
        vScale=QVector2D(1.0f,static_cast<float>(w)/static_cast<float>(h));
    }else{
        vScale=QVector2D(static_cast<float>(h)/static_cast<float>(w),1.0f);
    }
    emit statusBarMessage(QString("New Resolution: %1x%2").arg(w).arg(h),2000);

}

void MyGLWidget::updateFishies()
{
    updateTimer.start(10);
    qreal dT=0.0;
    if (timeSinceLastUpdate.elapsed()>0)
    {
        dT=static_cast<qreal>(timeSinceLastUpdate.elapsed())/1000.0;
        timeSinceLastUpdate.restart();
    }
    else
        return;
    CFish::moveFishes(dT);
    update();
    if (timeSinceLastFPSDisplay.elapsed()>1000)
    {
        qreal dTime=1000/static_cast<qreal>(timeSinceLastFPSDisplay.elapsed());
        int iFPS = static_cast<int>(iFrameCounter*dTime);
        emit statusBarMessage(QString("FPS: %1").arg(iFPS),500);
        emit showFPS(iFPS);
        timeSinceLastFPSDisplay.restart();
        iFrameCounter=0;
    }
}

void MyGLWidget::recalcForces()
{
    CFish::calcForces();
}

void MyGLWidget::recalcNeighbourhoods()
{
    recalcNeighbourhoodTimer.start(CFish::iRecallNeighbourhoodDelay);
    QElapsedTimer timer;
    timer.start();
    static int i=0;
    for (int i=0;i<CFish::iFishCount;i++)
    {
        const CFish &currFish=CFish::fishList[i];
        CFish::posList[i].set(i,currFish.x,currFish.y);
    }
    CFish::sortFishPos();
    CFish::findNeighbourhoods();
    CFish::fishColor.fill(SFishColor());
    CFish::fishColor[0].g=1.0;
    CFish::fishColor[0].b=0.6;
    for(int i=0;i<CFish::fishNeighbourhoods[0].size();i++)
    {
        int id=CFish::fishNeighbourhoods[0].at(i);
        CFish::fishColor[id].r=1.0;
        CFish::fishColor[id].b=0.6;
    }
    for(int i=0;i<CFish::fishProxyhoods[0].size();i++)
    {
        int id=CFish::fishProxyhoods[0].at(i);
        CFish::fishColor[id].r=1.0;
        CFish::fishColor[id].g=1.0;
        CFish::fishColor[id].b=0.3;
    }
    glBindVertexArray(VAOs[FishBody]);
    glBindBuffer(GL_ARRAY_BUFFER,Buffers[FishColorBuffer]);
    glBufferData(GL_ARRAY_BUFFER,CFish::fishList.size()*sizeof(SFishColor),&(CFish::fishColor.first().r), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(vFishColor);
    glVertexAttribPointer(vFishColor,3,GL_FLOAT,GL_FALSE,sizeof(SFishColor),BUFFER_OFFSET(0));
    glVertexAttribDivisor(vFishColor,1);
    glBindVertexArray(0);

    if (i%10==0)
    {
        qDebug()<< i << "th Iteration" <<timer.elapsed();
    }
    i++;



}

void MyGLWidget::updateBuffers()
{
    if (bBuffersCreated){
        glDeleteVertexArrays(NumVAOs,VAOs);
        glDeleteBuffers(NumBuffers,Buffers);
    }

    glGenVertexArrays(NumVAOs,VAOs);
    glBindVertexArray(VAOs[FishBody]);


 /*   GLfloat vertexCoords[iFishVertices][2]={
    {0.5f*CFish::fScale,0.0f},
    {-0.3f*CFish::fScale,-0.3f*CFish::fScale},
    {-0.3f*CFish::fScale,0.3f*CFish::fScale}
    };*/

    GLfloat vertexCoords[iFishVertices][2]={
        {1.0f*CFish::fScale,0.0f},
        {0.8f*CFish::fScale,0.3f*CFish::fScale},
        {0.8f*CFish::fScale,-0.3f*CFish::fScale},
        {0.0f,0.5f*CFish::fScale},
        {0.0f,-0.5f*CFish::fScale},
        {-0.6f*CFish::fScale,0.0f},
        {-1.0f*CFish::fScale,0.45f*CFish::fScale},
        {-1.0f*CFish::fScale,-0.45f*CFish::fScale},
    };

    GLushort fishIndicies[iFishIndices]={0,1,2, 1,3,4, 2,1,4, 3,5,4, 5,6,7};

    glGenBuffers(NumBuffers,Buffers);

    glBindBuffer(GL_ARRAY_BUFFER,Buffers[FishArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertexCoords),vertexCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vVertexPosition);
    glVertexAttribPointer(vVertexPosition,2,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,Buffers[FishIndexBuffer]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(fishIndicies),fishIndicies, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,Buffers[FishColorBuffer]);
    glBufferData(GL_ARRAY_BUFFER,CFish::fishList.size()*sizeof(SFishColor),&(CFish::fishColor.first().r), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(vFishColor);
    glVertexAttribPointer(vFishColor,3,GL_FLOAT,GL_FALSE,sizeof(SFishColor),BUFFER_OFFSET(0));
    glVertexAttribDivisor(vFishColor,1);



    glBindVertexArray(0);
}

void MyGLWidget::createFishes()
{
    if (CFish::iFishCount==0)
        CFish::iFishCount=1;
    if (!CFish::fishList.isEmpty())
    {
        CFish::fishList.clear();
    }
    if (!CFish::posList.isEmpty())
    {
        CFish::posList.clear();
    }
    if (!CFish::fishForces.isEmpty())
    {
        CFish::fishForces.clear();
    }
    if (!CFish::fishNeighbourhoods.isEmpty())
    {
        CFish::fishNeighbourhoods.clear();
    }
    if (!CFish::fishProxyhoods.isEmpty())
    {
        CFish::fishProxyhoods.clear();
    }
    if (!CFish::fishColor.isEmpty())
    {
        CFish::fishColor.clear();
    }
    CFish::fishList.reserve(CFish::iFishCount);
    CFish::posList.reserve(CFish::iFishCount);
    CFish::fishForces.fill(CVec2(),CFish::iFishCount);
    CFish::fishNeighbourhoods.fill(QVector<int>(),CFish::iFishCount);
    CFish::fishProxyhoods.fill(QVector<int>(),CFish::iFishCount);
    CFish::fishColor.fill(SFishColor(),CFish::iFishCount);

    for (int i=0;i<CFish::iFishCount;i++)
    {
        qreal x = static_cast<float> (qrand()) / static_cast<float> (RAND_MAX)*CFish::fTankSize - CFish::fHalfTanksSize;
        qreal y = static_cast<float> (qrand()) / static_cast<float> (RAND_MAX)*CFish::fTankSize - CFish::fHalfTanksSize;
        qreal v = static_cast<float> (qrand()) / static_cast<float> (RAND_MAX)*0.05+0.05;
        qreal phi = static_cast<float> (qrand()) / static_cast<float> (RAND_MAX)*2*M_PI;
        qreal vx=static_cast<float>(qFastCos(phi))*v;
        qreal vy=static_cast<float>(qFastSin(phi))*v;

        CFish::fishList.append(CFish(x,y,vx,vy));
        CFish::posList.append(CFishPos(i,x,y));
    }
    updateTimer.start(10);
    recalcForcesTimer.start(CFish::iRecalcForcesDelay);
    recalcNeighbourhoodTimer.start(CFish::iRecallNeighbourhoodDelay);
    timeSinceLastUpdate.restart();
    timeSinceLastFPSDisplay.restart();
}
