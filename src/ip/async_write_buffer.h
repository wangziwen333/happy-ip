#pragma once
#include <string>
#include <memory>
#include <list>
#include <mutex>
using namespace std;

namespace happy
{
	namespace asio
	{
		namespace ip
		{
			struct WriteData
			{
				WriteData(const string& in_data, const string& in_tag) : data(in_data), tag(in_tag) {}
				string data;
				string tag;
			};

			class AsyncWriteBuffer
			{
			public:
				bool AsyncWrite(shared_ptr <WriteData> in_write_data, shared_ptr <WriteData>& out_write_data);
				bool HandleWrite(shared_ptr <WriteData>& out_write_data);
				void Clear();
				void HandleWriteFailed(shared_ptr <WriteData> in_write_data);
				unsigned GetWriteBufferSize()
				{
					lock_guard<mutex> lock(mutex_);
					return list_wirte_data_.size();
				}
			private:
				bool is_async_write_ = false;
				list <shared_ptr<WriteData>> list_wirte_data_;
				mutex mutex_;
			};
		}
	}
}

