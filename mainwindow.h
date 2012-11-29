#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "KeyHashModel.h"
#include "syncia/SynciaEnvironment.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void AddUploadDirectory();
    void SearchKeyHash();
    void SetDownloadDirectoryPath();
    void DownloadFile(const QModelIndex& index);

    void Join();

    void OpenOptionDialog();

    void ShowVersionInfo();

private:
    Ui::MainWindow* ui;
    KeyHashModel* key_hash_model;
    syncia::SynciaEnvironment::Pointer syncia_env;
    syncia::SynciaCore::Pointer syncia;
    neuria::network::SessionPool::Pointer upper_pool;
    neuria::network::SessionPool::Pointer lower_pool;
    syncia::database::FileKeyHashDb::Pointer file_db;
    syncia::database::FileKeyHashDb::Pointer searched_file_db;
    syncia::FileSystemPath current_download_directory_path;
};

#endif // MAINWINDOW_H
