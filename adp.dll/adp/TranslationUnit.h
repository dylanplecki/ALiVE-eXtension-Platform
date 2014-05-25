#pragma once

#include <stdint.h>

#include <queue>
#include <unordered_map>

#include "Package.h"

namespace ADP
{
	class TranslationUnit
	{
	private:
		std::queue<ADP::Package*> export_queue;
		std::unordered_map<uint32_t, ADP::Package*> waiting_exports;

		// Functions
		void CallFunction(ADP::Package* a_package);

		// IO Functions
		bool Import(const char* &data);
		bool Export(const size_t &buffer_size, char* &buffer);
		bool Export(const uint32_t &a_package_id, const size_t &buffer_size, char* &buffer);
		bool QueueForExport(ADP::Package* a_package);

	public:
		TranslationUnit();
		~TranslationUnit();

		// Functions
		void Receive(const char* &data, const size_t &buffer_size, char* &output_buffer);

		// SQF Translation Functions
		// TODO: Static?
	};
}
