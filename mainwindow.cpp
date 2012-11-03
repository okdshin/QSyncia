#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SynciaEnvironment.h"
#include <QFileDialog>

MainWindow::MainWindow(syncia::SynciaCore* syncia, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    syncia(syncia)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

auto MainWindow::AddUploadDirectory() -> void
{
    const auto dir_path_str = QFileDialog::getExistingDirectory(this, "Select Directory", "/home", QFileDialog::ShowDirsOnly);
    if(dir_path_str == ""){
        return;
    }
    this->syncia->AddUploadDirectory(syncia::FileSystemPath(dir_path_str.toStdString()));
}

auto MainWindow::SearchKeyHash() -> void
{

}

auto MainWindow::DownloadFile() -> void
{

}
