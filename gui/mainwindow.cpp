#include "../include/gui/mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QColor>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      originalScene_(new QGraphicsScene(this)),
      processedScene_(new QGraphicsScene(this))
{
    ui->setupUi(this);
    basicGroup_ = new QButtonGroup(this);

    basicGroup_->addButton(ui->radioOriginal, 0);
    basicGroup_->addButton(ui->radioBW, 1);
    basicGroup_->addButton(ui->radioNegative, 2);
    basicGroup_->addButton(ui->radioKeepColor, 3);

    convGroup_ = new QButtonGroup(this);

    connect(convGroup_,
        &QButtonGroup::idClicked,
        this,
        [this](int id)
        {
            bool custom = (id == 7);

            ui->tableWidgetKernel->setVisible(custom);
            ui->spinBoxKernel->setEnabled(custom);
        });

    convGroup_->addButton(ui->radioConvNone, 0);
    convGroup_->addButton(ui->radioEmboss, 1);
    convGroup_->addButton(ui->radioSharpen, 2);
    convGroup_->addButton(ui->radioBoxBlur, 3);
    convGroup_->addButton(ui->radioGaussian, 4);
    convGroup_->addButton(ui->radioLaplacian, 5);
    convGroup_->addButton(ui->radioPrewitt, 6);
    convGroup_->addButton(ui->radioCustomKernel, 7);

    resize(600, 800);

    ui->graphicsViewOriginal->setScene(originalScene_);
    ui->graphicsViewProcessed->setScene(processedScene_);

    ui->spinBoxKernel->setMinimum(3);
    ui->spinBoxKernel->setMaximum(16);
    ui->spinBoxKernel->setSingleStep(2);
    ui->spinBoxKernel->setValue(3);

    ui->tableWidgetKernel->hide();

    connect(ui->actionOpen,
            &QAction::triggered,
            this,
            &MainWindow::openImage);

    connect(ui->actionSave,
            &QAction::triggered,
            this,
            &MainWindow::saveImage);

    connect(ui->pushButtonApply,
            &QPushButton::clicked,
            this,
            &MainWindow::applyFilter);

    connect(ui->spinBoxKernel,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &MainWindow::kernelChanged);

    kernelChanged(3);

    connect(ui->pushButtonColor,
        &QPushButton::clicked,
        this,
        &MainWindow::chooseColor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openImage()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Open image"),
        "",
        tr("Bitmap (*.bmp)")
    );

    if (filename.isEmpty())
        return;

    try
    {
        controller_.loadFile(filename.toStdString());

        updateOriginalImage();
        updateProcessedImage();
    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              e.what());
    }
}

void MainWindow::saveImage()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        tr("Save image"),
        "",
        tr("Bitmap (*.bmp)")
    );

    if (filename.isEmpty())
        return;

    try
    {
        controller_.saveFile(filename.toStdString());
    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              e.what());
    }
}

void MainWindow::kernelChanged(int size)
{
    controller_.setKernelSize(size);

    ui->tableWidgetKernel->clear();
    ui->tableWidgetKernel->setRowCount(size);
    ui->tableWidgetKernel->setColumnCount(size);

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            auto *item = new QTableWidgetItem("0");
            item->setTextAlignment(Qt::AlignCenter);

            ui->tableWidgetKernel->setItem(i, j, item);
        }
    }
}

void MainWindow::updateOriginalImage()
{
    originalScene_->clear();

    QImage image = bmpToQImage(controller_.originalImage());

    originalScene_->addPixmap(
        QPixmap::fromImage(image));

    ui->graphicsViewOriginal->fitInView(
        originalScene_->itemsBoundingRect(),
        Qt::KeepAspectRatio);
}

void MainWindow::updateProcessedImage()
{
    processedScene_->clear();

    QImage image = bmpToQImage(controller_.processedImage());

    processedScene_->addPixmap(
        QPixmap::fromImage(image));

    ui->graphicsViewProcessed->fitInView(
        processedScene_->itemsBoundingRect(),
        Qt::KeepAspectRatio);
}

QImage MainWindow::bmpToQImage(const Bmp<Rgba>& bmp) const
{
    QImage image(
        bmp.getWidth(),
        bmp.getHeight(),
        QImage::Format_ARGB32);

    for (uint32_t y = 0; y < bmp.getHeight(); ++y)
    {
        QRgb* line =
            reinterpret_cast<QRgb*>(image.scanLine(y));

        for (uint32_t x = 0; x < bmp.getWidth(); ++x)
        {
            const Rgba& pixel = bmp(y, x);

            line[x] = qRgba(
                pixel.red,
                pixel.green,
                pixel.blue,
                pixel.alpha);
        }
    }

    return image;
}

std::vector<float> MainWindow::readKernel() const
{
    int size = ui->tableWidgetKernel->rowCount();

    std::vector<float> kernel;
    kernel.reserve(size * size);

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            auto *item = ui->tableWidgetKernel->item(i, j);

            if (item)
                kernel.push_back(item->text().toFloat());
            else
                kernel.push_back(0.f);
        }
    }

    return kernel;
}

void MainWindow::chooseColor()
{
    QColor color =
        QColorDialog::getColor(
            selectedColor,
            this);

    if (!color.isValid())
        return;

    selectedColor = color;

    ui->frameColor->setStyleSheet(
        QString("background:%1;")
            .arg(color.name()));
}

void MainWindow::applyFilter()
{
    controller_.clearImage();

    switch (basicGroup_->checkedId())
    {
    case 0:
        controller_.clearImage();
        break;
    case 1:
        controller_.makeBWFunction();
        break;

    case 2:
        controller_.makeNegativeFunction();
        break;

    case 3:
    {
        Rgba color{
            static_cast<uint8_t>(selectedColor.blue()),
            static_cast<uint8_t>(selectedColor.green()),
            static_cast<uint8_t>(selectedColor.red()),
            static_cast<uint8_t>(selectedColor.alpha())
        };

        controller_.keepCertainChannelFunction(color);
        break;
    }

    default:
        break;
    }

    if (convGroup_->checkedId())
        controller_.setFilter(convGroup_->checkedId());
    
    if (convGroup_->checkedId() == 7)
    {
        controller_.setKernel(readKernel());
    }
    else if (convGroup_->checkedId() != 0)
    {
        controller_.setFilter(
            static_cast<CONVOLUTION_FILTER>(convGroup_->checkedId()));
    }

    controller_.processImage();
    updateProcessedImage();
}