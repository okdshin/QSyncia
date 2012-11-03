#include "mainwindow.h"
#include <QApplication>

#include "SynciaEnvironment.h"

int main(int argc, char *argv[])
{
    const int buffer_size = 128;
    const unsigned int max_key_hash_count = 5;
    const unsigned int spread_key_hash_max_count = 2;
    const unsigned int max_hop_count = 3;
    int local_port = 54321;

    std::cout << "LocalPort:" << local_port << std::endl;

    const auto node_id = neuria::network::CreateSocketNodeId("localhost", local_port);

    auto upper_pool = neuria::network::SessionPool::Create();
    auto lower_pool = neuria::network::SessionPool::Create();
    auto file_db = syncia::database::FileKeyHashDb::Create(0.3, buffer_size, std::cout);
    auto searched_file_db = syncia::database::FileKeyHashDb::Create(0.3, buffer_size, std::cout);

    syncia::SynciaEnvironment syncia_env(
        max_key_hash_count, spread_key_hash_max_count,
        max_hop_count, local_port, buffer_size, node_id,
        upper_pool, lower_pool, file_db, searched_file_db,
        [](const neuria::network::ErrorCode& error_code){
            std::cout << "failed create link : " << error_code << std::endl;
        },
        [](const syncia::FileSystemPath& file_path){
            std::cout << "replied!! : " << file_path << std::endl;
        }
    );

    syncia::SynciaCore* syncia_raw_ptr = syncia_env.GetSocketSyncia().get();
    syncia_env.Initialize();
    syncia_env.StartAccept();

    QApplication a(argc, argv);
    MainWindow w(syncia_raw_ptr);
    w.show();

    auto exec_code = a.exec();
    syncia_env.Join();
    return exec_code;
}
