#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QElapsedTimer>

#include "cfish.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))


class MyGLWidget : public QOpenGLWidget, QOpenGLFunctions_4_0_Core
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = nullptr);



protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);


signals:
    void statusBarMessage(const QString &, int);
    void showFPS(int);
public slots:

    void updateFishies();
    void recalcForces();
    void recalcNeighbourhoods();
    void createFishes();

private:
    enum VAO_IDs { FishBody, NumVAOs };
    enum Buffer_IDs { FishArrayBuffer, FishDataBuffer, FishIndexBuffer, FishColorBuffer, NumBuffers };
    enum Attrib_IDs { vVertexPosition = 0 , vFishPosition = 1, vFishVelocity = 2, vFishColor = 3 };

    void updateBuffers();

    GLuint VAOs[NumVAOs];
    GLuint Buffers[NumBuffers];
    int iFishVertices = 8;
    int iFishIndices = 15;
    bool bBuffersCreated = false;
    QVector2D vScale;

    QOpenGLShaderProgram *m_program;

    QTimer updateTimer;
    QTimer recalcForcesTimer;
    QTimer recalcNeighbourhoodTimer;
    QElapsedTimer timeSinceLastUpdate;
    QElapsedTimer timeSinceLastFPSDisplay;
    int iFrameCounter=0;
};

#endif // MYGLWIDGET_H
