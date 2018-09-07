#include <ip/async_write_buffer.h>

namespace klicen
{
	namespace asio
	{
		namespace ip
		{
			bool AsyncWriteBuffer::AsyncWrite(shared_ptr <WriteData> in_write_data, shared_ptr <WriteData>& out_write_data)
			{
				lock_guard<mutex> lock(mutex_);
				if (is_async_write_)
				{
					list_wirte_data_.push_back(in_write_data);
					return false;
				}
				out_write_data = in_write_data;
				is_async_write_ = true;

				return true;
			}

			bool AsyncWriteBuffer::HandleWrite(shared_ptr <WriteData>& out_write_data)
			{
				lock_guard<mutex> lock(mutex_);
				do 
				{
					is_async_write_ = true;
					if (!list_wirte_data_.size())
					{
						is_async_write_ = false;
						break;
					}
					out_write_data = list_wirte_data_.front();
					list_wirte_data_.pop_front();
				} while (false);

				return is_async_write_;
			}

			void AsyncWriteBuffer::Clear()
			{
				lock_guard<mutex> lock(mutex_);
				list_wirte_data_.clear();
				is_async_write_ = false;
			}

			void AsyncWriteBuffer::HandleWriteFailed(shared_ptr <WriteData> in_write_data)
			{
				lock_guard<mutex> lock(mutex_);
				list_wirte_data_.push_front(in_write_data);
				is_async_write_ = false;
			}
		}
	}
}
