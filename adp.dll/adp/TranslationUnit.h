#pragma once

#include <stdint.h>

#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_map>

#include "Package.h"
#include "protocol_def.h"

namespace ADP
{
	class TranslationUnit
	{
	private:
		std::mutex export_queue_lock_;
		std::queue<ADP::Package*> export_queue_;
		std::unordered_map<uint32_t, ADP::Package*> waiting_exports_;

		// Functions
		void CallFunction(ADP::Package* a_package); // TODO
		bool PackReturn(const char &status, const char &func, const boost::string_ref &message, std::stringstream &output_stream);
		bool PackReturn(const char &status, const char &func, Package* &a_package, const size_t &buffer_size, std::stringstream &output_stream);
		bool ReturnError(const boost::string_ref &message, std::stringstream &output_stream);
		void ReturnPackage(ADP::Package* a_package, const size_t &buffer_size, std::stringstream &output_stream);

		// IO Functions
		bool Import(const char* &data);
		bool Export(const size_t &buffer_size, char* &buffer);
		bool Export(const uint32_t &a_package_id, const size_t &buffer_size, char* &buffer);
		bool QueueForExport(ADP::Package* a_package);

	public:
		TranslationUnit();
		~TranslationUnit();

		// Functions
		void Receive(const char* &data, size_t buffer_size, char* &output_buffer);

		// SQF Translation Functions
		// TODO: Static?
	};
}
