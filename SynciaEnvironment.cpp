#ifdef SYNCIAENVIRONMENT_UNIT_TEST
#include "SynciaEnvironment.h"
#include <iostream>

using namespace syncia;

int main(int argc, char* argv[])
{
	const int buffer_size = 128;
	const unsigned int max_key_hash_count = 5;
	const unsigned int spread_key_hash_max_count = 2;
	const unsigned int max_hop_count = 3;
	int local_port = 54321;
	if(argc == 2)
	{
		local_port = boost::lexical_cast<int>(argv[1]);
	}

	std::cout << "LocalPort:" << local_port << std::endl;

	const auto node_id = neuria::network::CreateSocketNodeId("localhost", local_port);

	auto upper_pool = neuria::network::SessionPool::Create();
	auto lower_pool = neuria::network::SessionPool::Create();
	auto file_db = syncia::database::FileKeyHashDb::Create(0.3, buffer_size, std::cout);
	auto searched_file_db = syncia::database::FileKeyHashDb::Create(0.3, buffer_size, std::cout);

	SynciaEnvironment syncia_env(
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

	auto syncia = syncia_env.GetSocketSyncia();
	syncia_env.Initialize();

	auto shell = neuria::test::CuiShell(std::cout);
	neuria::test::RegisterExitFunc(shell);
	
	shell.Register("upload", ": upload directory.", 
		[syncia](const neuria::test::CuiShell::ArgList& argument_list){
			syncia->AddUploadDirectory(syncia::FileSystemPath(argument_list.at(1)));
		});
	shell.Register("db", ": show uploaded files.", 
		[file_db](const neuria::test::CuiShell::ArgList& argument_list){
			std::cout << file_db << std::endl;
		});
	shell.Register("sdb", ": show uploaded files.", 
		[searched_file_db](const neuria::test::CuiShell::ArgList& argument_list){
			std::cout << searched_file_db << std::endl;
		});
	shell.Register("link", ": create new search link.", 
		[syncia](const neuria::test::CuiShell::ArgList& argument_list){
			syncia->CreateSearchLink(
				neuria::network::CreateSocketNodeId(
					argument_list.at(1), 
					boost::lexical_cast<int>(argument_list.at(2))
				)
			);
		});
	shell.Register("search", ": search key hash.", 
		[syncia](const neuria::test::CuiShell::ArgList& argument_list){
			auto keyward_only = neuria::test::CuiShell::ArgList();
			std::copy(argument_list.begin()+1, argument_list.end(), 
				std::back_inserter(keyward_only));
			syncia->SearchKeyHash(database::KeywordList(keyward_only));
		});
	
	shell.Register("spread", ": request spread key hash.", 
		[syncia](const neuria::test::CuiShell::ArgList& argument_list){
			syncia->RequestSpreadKeyHash();
		});
	
	shell.Register("request", ": request file.", 
		[syncia](const neuria::test::CuiShell::ArgList& argument_list){
			syncia->RequestFile(syncia::database::HashId(argument_list.at(3)),
				neuria::network::CreateSocketNodeId(
					argument_list.at(1), 
					boost::lexical_cast<int>(argument_list.at(2))),
				syncia::FileSystemPath(argument_list.at(4))
			);
		});
	
	shell.Register("upper", ": show upper linked sessions.", 
		[upper_pool](const neuria::test::CuiShell::ArgList& argument_list){
			std::cout << "upper session: " << upper_pool << std::endl;
		});
	shell.Register("lower", ": show lower linked sessions.", 
		[lower_pool](const neuria::test::CuiShell::ArgList& argument_list){
			std::cout << "lower session: " << lower_pool << std::endl;
		});
	
	syncia_env.StartAccept();
	shell.Start();
	
	syncia_env.Join();
    
	return 0;
}

#endif
