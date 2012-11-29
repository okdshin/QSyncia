#include "optiondialog.h"
#include "ui_optiondialog.h"
#include <QFileDialog>

OptionDialog::OptionDialog(const QString& current_download_path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
    this->ui->download_directory_path_edit->setText(current_download_path);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

auto OptionDialog::ChooseDownloadDirectoryPath() -> void {

    const auto dir_path_str = QFileDialog::getExistingDirectory(this, "Select Directory", "/home", QFileDialog::ShowDirsOnly);
    if(dir_path_str == ""){
        std::cout << "canceled." << std::endl;
        return;
    }
    this->ui->download_directory_path_edit->setText(dir_path_str);
    this->download_directory_path = syncia::FileSystemPath(dir_path_str.toStdString());

}

auto OptionDialog::GetDownloadDirectoryPath() -> syncia::FileSystemPath {
    return this->download_directory_path;
}

auto OptionDialog::AddNewLinkNodeId() -> void {
    if(this->ui->lineEdit->text() == ""){
        return;
    }
    this->ui->listWidget->addItem(this->ui->lineEdit->text());
}

auto OptionDialog::RemoveLinkNodeId() -> void {
    //this->ui->listWidget->removeItemWidget(this->ui->listWidget->indexFromItem(this->ui->listWidget->selectedItems()[0]);
}
