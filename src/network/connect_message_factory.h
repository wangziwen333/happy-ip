#pragma once
#include <network/util_message_factory.h>

namespace happy
{
	namespace utils
	{
		namespace network
		{
			class ConnectMessageFactory : public UtilMessageFactory
			{
			public:
				ConnectMessageFactory();
			private:
				virtual void HandleCreate(string& response) override final;
				virtual bool HandleRead(const uint8_t* buffer, const size_t& length, string& response) override final;
				virtual shared_ptr <Message> Produce(const shared_ptr <News> news) override final;
				protected:
					virtual void ShowTips(string& request) = 0;
			};
		}
	}
}

