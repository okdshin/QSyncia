#pragma once
//SynciaEnvironment:20121003
#include <iostream>
#include "syncia/SynciaCore.h"

namespace syncia
{
class SynciaEnvironment{
public:
    SynciaEnvironment(
			int max_key_hash_count, int spread_key_hash_max_count, 
			int max_hop_count, int local_port, int buffer_size, 
			const neuria::network::NodeId& node_id,
			neuria::network::SessionPool::Pointer upper_pool,
			neuria::network::SessionPool::Pointer lower_pool,
			syncia::database::FileKeyHashDb::Pointer file_db,
			syncia::database::FileKeyHashDb::Pointer searched_file_db,
			syncia::LinkAction::OnFailedCreateLinkFunc on_failed_create_link_func,
			syncia::RequestFileAction::OnRepliedFileFunc on_replied_file_func)
		:service_ptr(new boost::asio::io_service), 
		work_ptr(new boost::asio::io_service::work(*(this->service_ptr))), 
		thread_ptr(new boost::thread(boost::bind(&boost::asio::io_service::run, service_ptr))),
		client(neuria::network::SocketClient::Create(*service_ptr, buffer_size, std::cout)),
		server(neuria::network::SocketServer::Create(
			*service_ptr, local_port, buffer_size, std::cout)),
		dispatcher(syncia::BehaviorDispatcher::Create(*service_ptr, std::cout)),
		multiple_timer(neuria::timer::MultipleTimer::Create(*service_ptr)),
		syncia(syncia::SynciaCore::Create(
			max_key_hash_count, spread_key_hash_max_count, 
			max_hop_count, buffer_size, upper_pool, lower_pool, 
			file_db, searched_file_db, node_id, 
			on_failed_create_link_func, on_replied_file_func, std::cout)){
	
		SetOnReceiveFuncOnly(this->server, this->dispatcher->GetOnReceiveFunc());
	}

	auto Initialize() -> void {
		this->syncia->Bind(this->dispatcher);	
		this->syncia->Bind(this->client);	
		this->syncia->Bind(this->multiple_timer);
	}

	auto GetSocketSyncia() -> syncia::SynciaCore::Pointer {
		return this->syncia;	
	}

	auto StartAccept() -> void {
		this->server->StartAccept();	
	}

	auto Join() -> void {
		this->thread_ptr->join();	
	}

private:
	boost::shared_ptr<boost::asio::io_service> service_ptr;
	boost::shared_ptr<boost::asio::io_service::work> work_ptr;
	boost::shared_ptr<boost::thread> thread_ptr;
	neuria::network::SessionPool::Pointer upper_pool, lower_pool;
	neuria::network::Client::Pointer client;
	neuria::network::Server::Pointer server;
	syncia::BehaviorDispatcher::Pointer dispatcher;
	neuria::timer::MultipleTimer::Pointer multiple_timer;
	syncia::SynciaCore::Pointer syncia;

};
}

