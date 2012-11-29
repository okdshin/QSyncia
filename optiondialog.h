#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include "syncia/Syncia.h"

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT
    
public:
    OptionDialog(const QString& current_download_path, QWidget *parent=0);
    ~OptionDialog();

public slots:
    void ChooseDownloadDirectoryPath();
    syncia::FileSystemPath GetDownloadDirectoryPath();
    void AddNewLinkNodeId();
    void RemoveLinkNodeId();
    
private:
    Ui::OptionDialog *ui;
    syncia::FileSystemPath download_directory_path;
};

#endif // OPTIONDIALOG_H
