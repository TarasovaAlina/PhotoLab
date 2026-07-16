#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QImage>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QTableWidget>
#include <QButtonGroup>


#include "../controller/controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openImage();
    void saveImage();

    void applyFilter();

    void kernelChanged(int size);

private:
    void updateOriginalImage();
    void updateProcessedImage();

    QImage bmpToQImage(const Bmp<Rgba>& bmp) const;
    std::vector<float> readKernel() const;

    Ui::MainWindow *ui;

    QGraphicsScene *originalScene_;
    QGraphicsScene *processedScene_;

    QColor selectedColor = Qt::red;

    QButtonGroup *basicGroup_;
    QButtonGroup *convGroup_;

    Controller controller_;
private slots:

    void chooseColor();
};

#endif