#pragma once
#include <network/util_message_factory.h>
using namespace happy::asio::ip;

namespace happy
{
	namespace utils
	{
		namespace network
		{
			class ClientMessageFactory : public UtilMessageFactory
			{
			public:
				ClientMessageFactory(const string& closed_print = "", const bool is_read_print = false);
			private:
				const string closed_print_;
			private:
				virtual void HandleClose(const ClosedType closed_type) override;
			};
		}
	}
}

