#pragma once
#include <mutex>
#include <unordered_map>
#include <list>
using namespace std;

namespace klicen
{
	namespace asio
	{
		namespace ip
		{
			template <class TKey, class TValue>
			class UnorderedMapSafe
			{
			public:
				bool Upsert(const TKey& key, const TValue& value_in, TValue *value_out = nullptr)
				{
					lock_guard<mutex> lock(mutex_);
					bool is_update = false;
					auto it = unordered_map_.find(key);
					if (it == unordered_map_.end())
					{
						unordered_map_[key] = value_in;
					}
					else
					{
						is_update = true;
						if (value_out != nullptr)
						{
							*value_out = it->second;
						}
						it->second = value_in;
					}
					return is_update;
				}

				void Erase(const TKey& key)
				{
					lock_guard<mutex> lock(mutex_);
					unordered_map_.erase(key);
				}

				bool EraseByKeyAndValue(const TKey& key, const TValue& value)
				{
					lock_guard<mutex> lock(mutex_);
					auto it = unordered_map_.find(key);
					if (it != unordered_map_.end() && value == it->second)
					{
						unordered_map_.erase(it);

						return true;
					}
					return false;
				}

				void EraseByValue(const TValue& value)
				{
					lock_guard<mutex> lock(mutex_);
					list <TValue> values_erase;
					for (auto pair : unordered_map_)
					{
						if (value == pair.second)
						{
							values_erase.push_back(pair.first);
						}
					}
					for (auto value_erase : values_erase)
					{
						unordered_map_.erase(value_erase);
					}
				}

				bool Erase(const TKey& key, TValue& value)
				{
					lock_guard<mutex> lock(mutex_);
					auto it = unordered_map_.find(key);
					if (unordered_map_.end() == it)
					{
						return false;
					}
					value = it->second;
					unordered_map_.erase(it);

					return true;
				}

				void ForEach(const function<void(const TKey&, const TValue&)>& _Func)
				{
					lock_guard<mutex> lock(mutex_);
					for (auto pair : unordered_map_)
					{
						_Func(pair.first, pair.second);
					}
				};

				bool ForOne(const TKey& key, const function<void(const TKey&, TValue&)>& _Func)
				{
					lock_guard<mutex> lock(mutex_);
					auto it = unordered_map_.find(key);
					if (unordered_map_.end() == it)
					{
						return false;
					}
					_Func(it->first, it->second);

					return true;
				}

				bool ForOneAndDelete(const TKey& key, const function<void(TValue&, bool& is_delete)>& _Func)
				{
					lock_guard<mutex> lock(mutex_);
					auto it = unordered_map_.find(key);
					if (unordered_map_.end() == it)
					{
						return false;
					}
					bool is_delete = false;
					_Func(it->second, is_delete);
					if (is_delete)
					{
						unordered_map_.erase(it);
					}
					return true;
				}

				bool GetValue(const TKey& key, TValue& value)
				{
					lock_guard<mutex> lock(mutex_);
					auto it = unordered_map_.find(key);
					if (unordered_map_.end() == it)
					{
						return false;
					}
					value = it->second;

					return true;
				}

				bool IsExists(const TKey& key)
				{
					lock_guard<mutex> lock(mutex_);
					auto it = unordered_map_.find(key);
					if (unordered_map_.end() == it)
					{
						return false;
					}
					return true;
				}

				void Clear()
				{
					lock_guard<mutex> lock(mutex_);
					unordered_map_.clear();
				}

				int Size()
				{
					lock_guard<mutex> lock(mutex_);
					return unordered_map_.size();
				}
				void ForEachErase(const function<bool(TValue&)>& _Func)
				{
					lock_guard<mutex> lock(mutex_);
					for (auto iter = unordered_map_.begin(); iter != unordered_map_.end();)
					{
						if (_Func(iter->second))unordered_map_.erase(iter++);
						else iter++;
					}
				}
				void ForEachChange(const function<void(const TKey&, TValue&)>& _Func)
				{
					lock_guard<mutex> lock(mutex_);
					for (auto &pair : unordered_map_)
					{
						_Func(pair.first, pair.second);
					}
				};

			private:
				mutex mutex_;
				unordered_map <TKey, TValue> unordered_map_;
			};
		}
	}
}
