#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openGLWidget,SIGNAL(statusBarMessage(QString,int)),ui->statusBar,SLOT(showMessage(QString,int)));
    connect(ui->openGLWidget,SIGNAL(showFPS(int)),ui->lcdNumberFPS,SLOT(display(int)));
    ui->sliderAlignmentRange->setValue((int)CFish::alignment.fRangeFL);
    ui->sliderAlignmentForce->setValue((int)(CFish::alignment.fForce*100.0f));
    ui->sliderAlignmentGamma->setValue((int)(CFish::alignment.fFaLLOffGamma*10.0f));
    ui->sliderCoherenceRange->setValue((int)CFish::coherence.fRangeFL);
    ui->sliderCoherenceForce->setValue((int)(CFish::coherence.fForce*100.0f));
    ui->sliderCoherenceGamma->setValue((int)(CFish::coherence.fFaLLOffGamma*10.0f));
    ui->sliderRepulsionRange->setValue((int)CFish::repulsion.fRangeFL);
    ui->sliderRepulsionForce->setValue((int)(CFish::repulsion.fForce*100.0f));
    ui->sliderRepulsionGamma->setValue((int)(CFish::repulsion.fFaLLOffGamma*10.0f));
    ui->labelAlignmentRange->setText(QString("%1FL").arg((int)CFish::alignment.fRangeFL));
    ui->labelCoherenceRange->setText(QString("%1FL").arg((int)CFish::coherence.fRangeFL));
    ui->labelRepulsionRange->setText(QString("%1FL").arg((int)CFish::repulsion.fRangeFL));
    ui->labelAlignmentForce->setText(QString("%1").arg((double)CFish::alignment.fForce,6,'f',2));
    ui->labelCoherenceForce->setText(QString("%1").arg((double)CFish::coherence.fForce,6,'f',2));
    ui->labelRepulsionForce->setText(QString("%1").arg((double)CFish::repulsion.fForce,6,'f',2));
    ui->labelAlignmentGamma->setText(QString("%1").arg((double)CFish::alignment.fFaLLOffGamma,6,'f',2));
    ui->labelCoherenceGamma->setText(QString("%1").arg((double)CFish::coherence.fFaLLOffGamma,6,'f',2));
    ui->labelRepulsionGamma->setText(QString("%1").arg((double)CFish::repulsion.fFaLLOffGamma,6,'f',2));
    ui->editFishCount->setText(QString("%1").arg(CFish::iFishCount));
    ui->editFishCount->setValidator( new QIntValidator(10,10000,ui->editFishCount));
    ui->spinBoxForceDelay->setValue(CFish::iRecalcForcesDelay);
    ui->spinBoxNeightborrhoodDelay->setValue(CFish::iRecallNeighbourhoodDelay);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sliderAlignmentRange_sliderMoved(int position)
{   CFish::alignment.setRange((float)position);
    ui->labelAlignmentRange->setText(QString("%1FL").arg((int)CFish::alignment.fRangeFL));}
void MainWindow::on_sliderCoherenceRange_sliderMoved(int position)
{   CFish::coherence.setRange((float)position);
    ui->labelCoherenceRange->setText(QString("%1FL").arg((int)CFish::coherence.fRangeFL));}
void MainWindow::on_sliderRepulsionRange_sliderMoved(int position)
{   CFish::repulsion.setRange((float)position);
    ui->labelRepulsionRange->setText(QString("%1FL").arg((int)CFish::repulsion.fRangeFL));}

void MainWindow::on_sliderAlignmentForce_sliderMoved(int position)
{   CFish::alignment.fForce=(float)position/100.0f;
    ui->labelAlignmentForce->setText(QString("%1").arg((double)CFish::alignment.fForce,6,'f',2));}
void MainWindow::on_sliderCoherenceForce_sliderMoved(int position)
{   CFish::coherence.fForce=(float)position/100.0f;
    ui->labelCoherenceForce->setText(QString("%1").arg((double)CFish::coherence.fForce,6,'f',2));}
void MainWindow::on_sliderRepulsionForce_sliderMoved(int position)
{   CFish::repulsion.fForce=(float)position/100.0f;
    ui->labelRepulsionForce->setText(QString("%1").arg((double)CFish::repulsion.fForce,6,'f',2));}

void MainWindow::on_sliderAlignmentGamma_sliderMoved(int position)
{   CFish::alignment.fFaLLOffGamma=(float)position/10.0f;
    ui->labelAlignmentGamma->setText(QString("%1").arg((double)CFish::alignment.fFaLLOffGamma,6,'f',2));}
void MainWindow::on_sliderCoherenceGamma_sliderMoved(int position)
{   CFish::coherence.fFaLLOffGamma=(float)position/10.0f;
    ui->labelCoherenceGamma->setText(QString("%1").arg((double)CFish::coherence.fFaLLOffGamma,6,'f',2));}
void MainWindow::on_sliderRepulsionGamma_sliderMoved(int position)
{   CFish::repulsion.fFaLLOffGamma=(float)position/10.0f;
    ui->labelRepulsionGamma->setText(QString("%1").arg((double)CFish::repulsion.fFaLLOffGamma,6,'f',2));}

void MainWindow::on_editFishCount_textChanged(const QString &arg1)
{
    bool bOk=true;
    arg1.toInt(&bOk);
    if (bOk)
    {
        CFish::iFishCount=arg1.toInt(&bOk);
        ui->openGLWidget->createFishes();
    }
}

void MainWindow::on_spinBoxForceDelay_valueChanged(int arg1)
{
    CFish::iRecalcForcesDelay=arg1;
}

void MainWindow::on_spinBoxNeightborrhoodDelay_valueChanged(int arg1)
{
    CFish::iRecallNeighbourhoodDelay=arg1;
}
