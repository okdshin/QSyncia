#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include "optiondialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->key_hash_model = new KeyHashModel(this);
    this->ui->key_hash_view->setModel(this->key_hash_model);

    std::ifstream config_file("./config.txt");
    assert("open file error" && config_file);
    boost::property_tree::ptree config_tree;
    boost::property_tree::info_parser::read_info(config_file, config_tree);
    const unsigned int buffer_size = config_tree.get("buffer_size", 126);
    const unsigned int max_key_hash_count = config_tree.get("search_key_count", 10);
    const unsigned int spread_key_hash_max_count = config_tree.get("spread_key_count", 10);
    const unsigned int max_hop_count = config_tree.get("hop_count", 6);
    const unsigned int thread_num = config_tree.get("thread_num", 5);
    const std::string local_hostname = config_tree.get("node_id.local_hostname", "localhost");
    const unsigned int local_port = config_tree.get("node_id.local_port", 577716);
    const bool is_debug_mode = config_tree.get("is_debug_mode", false);
    std::stringstream no_output;
    std::ostream& log_os = is_debug_mode ? std::cout : no_output;
    std::cout << "LocalHostname:" << local_hostname << std::endl;
    std::cout << "LocalPort:" << local_port << std::endl;

    const auto node_id = neuria::network::CreateSocketNodeId(local_hostname.c_str(), local_port);

    this->upper_pool = neuria::network::SessionPool::Create();
    this->lower_pool = neuria::network::SessionPool::Create();
    this->file_db = syncia::database::FileKeyHashDb::Create(0.3, buffer_size, log_os);
    this->searched_file_db = syncia::database::FileKeyHashDb::Create(0.3, buffer_size, log_os);

    this->syncia_env = syncia::SynciaEnvironment::Create(
        max_key_hash_count, spread_key_hash_max_count,
        max_hop_count, local_port, buffer_size, thread_num, node_id,
        this->upper_pool, this->lower_pool,
        this->file_db, this->searched_file_db
    );

    this->syncia = syncia_env->GetSocketSyncia();
    syncia_env->Initialize();
    syncia_env->StartAccept();

    {//create initial link
        std::ifstream link_file("links.txt");
        std::string node_id_str;
        while(link_file && std::getline(link_file, node_id_str)){
            std::cout << "attempt to connect " << node_id_str << "..." << std::endl;
            this->syncia->CreateSearchLink(neuria::network::NodeId(node_id_str));
        }
    }

    this->current_download_directory_path = syncia::FileSystemPath("./");

}

MainWindow::~MainWindow()
{
    delete ui;
    this->syncia_env.reset();
}

auto MainWindow::Join() -> void {
    this->syncia_env->Join();
}

auto MainWindow::AddUploadDirectory() -> void
{
    const auto dir_path_str = QFileDialog::getExistingDirectory(this, "Select Directory", "/home", QFileDialog::ShowDirsOnly);
    if(dir_path_str == ""){
        std::cout << "canceled." << std::endl;
        return;
    }
    this->syncia->AddUploadDirectory(syncia::FileSystemPath(dir_path_str.toStdString()));
}


auto MainWindow::SearchKeyHash() -> void
{
    this->key_hash_model->Clear();
    auto keyword_list_str = this->ui->search_edit->text().toStdString();
    auto keyword_list = syncia::database::KeywordList::WrappedType();
    boost::split(keyword_list, keyword_list_str, boost::is_any_of(" "));
    this->syncia->SearchKeyHash(syncia::database::KeywordList(keyword_list));
    const auto key_hash_list = this->searched_file_db->GetNewer(20);
    for(const auto& key_hash : key_hash_list){
        this->key_hash_model->Add(key_hash);
    }
}

auto MainWindow::SetDownloadDirectoryPath() -> void {
    const auto dir_path_str = QFileDialog::getExistingDirectory(this, "Select Directory", "/home", QFileDialog::ShowDirsOnly);
    if(dir_path_str == ""){
        std::cout << "canceled." << std::endl;
        return;
    }
    this->syncia->SetDownloadDirectoryPath(syncia::FileSystemPath(dir_path_str.toStdString()));
}

auto MainWindow::DownloadFile(const QModelIndex& index) -> void
{
    const auto key_hash = this->key_hash_model->GetKeyHash(index);
    this->syncia->RequestFile(key_hash, [this](const syncia::database::FileKeyHash& key_hash){
        //QMessageBox::information(this, "QSyncia", "File Downloaded.", QMessageBox::Ok);
    });
}

auto MainWindow::ShowVersionInfo() -> void {
    QMessageBox::information(this, "QSyncia", "QSyncia version 0.5", QMessageBox::Ok);
}

auto MainWindow::OpenOptionDialog() -> void {
    OptionDialog od(QString(this->current_download_directory_path.string().c_str()), this);
    od.exec();
    this->current_download_directory_path = od.GetDownloadDirectoryPath();
    this->syncia->SetDownloadDirectoryPath(this->current_download_directory_path);
}
