#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace syncia{
class SynciaCore;
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(syncia::SynciaCore* syncia, QWidget *parent = 0);
    ~MainWindow();

    auto AddUploadDirectory() -> void;
    auto SearchKeyHash() -> void;
    auto DownloadFile() -> void;

private:
    Ui::MainWindow *ui;
    syncia::SynciaCore* syncia;
};

#endif // MAINWINDOW_H
