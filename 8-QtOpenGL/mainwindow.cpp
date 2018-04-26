#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	connect(ui->setBtn, &QAbstractButton::clicked, this, &MainWindow::onSetClicked);
}

void MainWindow::onSetClicked(bool check) {
	QImage image("decoration1.png");
	QImage srcImage = image.convertToFormat(QImage::Format_RGB888).mirrored(); 
	srcImage = srcImage.rgbSwapped(); 
	ui->openGLWidget->setFrameData(QOpenGLTexture::BGR, QOpenGLTexture::UInt8, srcImage.bits(), srcImage.width(), srcImage.height());
	ui->statusBar->showMessage("set");
}

MainWindow::~MainWindow()
{
    delete ui;
}
