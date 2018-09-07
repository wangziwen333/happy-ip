#pragma once
#include <ip/server/unordered_map_safe.h>
#include <ip/message_factory.h>
#include <ip/server/session.h>

namespace klicen
{
	namespace asio
	{
		namespace ip
		{
#define CLOSE_HANDLE [&](const string& session_id){ DeleteSession(session_id); }
			class Server
			{
			public:
				Server(const int session_timeout)
					: session_timeout_(session_timeout)
				{}

				void SetMessageFactory(shared_ptr <klicen::asio::ip::MessageFactory> message_factory)
				{
					message_factory_ = message_factory;
				}

				void PublishAll(const string& buffer)
				{
					if (buffer.length())
					{
						sessions_.ForEach([&](const string& session_id, const shared_ptr<Session>& session)
						{
							session->AsyncWrite(buffer);
						});
					}
				}

				void PublishOne(const string& session_id, const string& buffer,const string &tag="")
				{
					if (buffer.length())
					{
						sessions_.ForOne(session_id, [&](const string& session_id, const shared_ptr<Session>& session)
						{
							session->AsyncWrite(buffer,tag);
						});
					}
				}

				void AllSend(const function<string(const string& session_id, const shared_ptr<Session>& session)>& _Func)
 				{
					sessions_.ForEach([&](const string& session_id, const shared_ptr<Session>& session)
					{
						auto temp = _Func(session_id, session);

						if (!temp.empty())
						{
							session->AsyncWrite(temp);
						}
					});
 				}

				void AllHandle(const function<string(const string& session_id, const shared_ptr<Session>& session)>& _Func)
				{
					sessions_.ForEach([&](const string& session_id, const shared_ptr<Session>& session)
					{
						_Func(session_id, session);
					});
				}

			protected:
				virtual shared_ptr <klicen::asio::ip::MessageFactory> GetMessageFactory(const string& session_id)
				{
					return message_factory_->Create(session_id);
				}

				// true: is update
				bool UpsertSession(const string& session_id, const shared_ptr<Session>& session_in, shared_ptr<Session>* session_out = nullptr)
				{
					return sessions_.Upsert(session_id, session_in, session_out);
				}

				shared_ptr<Session> FindSession(const string& session_id)
				{
					shared_ptr<Session> value;
					bool b = sessions_.GetValue(session_id, value);
					if (b)
						return value;
					else
						return nullptr;
				}

				virtual void DeleteSession(const string& session_id)
				{
					sessions_.Erase(session_id);
				}

				const int session_timeout_;
			private:
				UnorderedMapSafe <string, shared_ptr<Session>> sessions_;
				shared_ptr <klicen::asio::ip::MessageFactory> message_factory_{ new klicen::asio::ip::MessageFactory };
			};
		}
	}
}