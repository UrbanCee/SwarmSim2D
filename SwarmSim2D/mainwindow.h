#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <myglwidget.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_sliderAlignmentRange_sliderMoved(int position);
    void on_sliderCoherenceRange_sliderMoved(int position);
    void on_sliderRepulsionRange_sliderMoved(int position);

    void on_sliderAlignmentForce_sliderMoved(int position);

    void on_sliderAlignmentGamma_sliderMoved(int position);

    void on_sliderCoherenceForce_sliderMoved(int position);

    void on_sliderRepulsionForce_sliderMoved(int position);

    void on_sliderCoherenceGamma_sliderMoved(int position);

    void on_sliderRepulsionGamma_sliderMoved(int position);

    void on_editFishCount_textChanged(const QString &arg1);

    void on_spinBoxForceDelay_valueChanged(int arg1);

    void on_spinBoxNeightborrhoodDelay_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
