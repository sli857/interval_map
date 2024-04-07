#include <map>
#include <iostream>

using namespace std;

template <typename K, typename V>
class interval_map
{
	friend void IntervalMapTest();
	V m_valBegin;

public:
	map<K, V> m_map;
	// constructor associates whole range of K with val
	interval_map(V const &val)
		: m_valBegin(val)
	{
	}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign(K const &keyBegin, K const &keyEnd, V const &val)
	{
		if (!(keyBegin < keyEnd))
			return;

		using iterator_t = typename std::map<K, V>::iterator;
		iterator_t itLow = m_map.lower_bound(keyBegin);
		iterator_t itHigh = m_map.lower_bound(keyEnd);

		if (itLow != m_map.begin() && std::prev(itLow)->second == val)
		{
			--itLow;
		}
		else
		{
			if (itLow == itHigh && keyEnd < itLow->first)
			{
				if (itHigh == m_map.begin())
				{
					m_map.insert({keyEnd, m_valBegin});
				}
				else
				{
					m_map.insert({keyEnd, std::prev(itHigh)->second});
				}
			}
			itLow = m_map.insert(itLow, {keyBegin, val});
		}

		if (itHigh != m_map.end() && itHigh->second == val)
		{
			++itHigh;
		}
		else
		{
			if (itLow == m_map.begin() && itHigh == m_map.end())
			{
				itHigh = m_map.insert(itHigh, {keyEnd, (--m_map.end())->second});
			}
			else
			{
				if (itHigh == m_map.end())
				{
					m_map.insert({keyEnd, std::prev(itLow)->second});
				}
				else
				{
					m_map.insert({keyEnd, std::prev(itHigh)->second});
				}
			}
			if (!(keyEnd < itHigh->first))
			{
				++itHigh;
			}
		}
		itLow = std::next(itLow);
		itHigh = std::prev(itHigh);
		if (!(itLow->first < itHigh->first))
		{
			return;
		}
		m_map.erase(itLow, itHigh);
	}

	// look-up of the value associated with key
	V const &operator[](K const &key) const
	{
		auto it = m_map.upper_bound(key);
		if (it == m_map.begin())
		{
			return m_valBegin;
		}
		else
		{
			return (--it)->second;
		}
	}
};

void printM(typename std::map<int, char>::iterator itBegin, typename std::map<int, char>::iterator itEnd)
{

	for (itBegin; itBegin != itEnd; ++itBegin)
	{
		cout << "key = " << itBegin->first << " value = " << itBegin->second << endl;
	}
}

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.
int main()
{
	interval_map<int, char> demo('?');
	demo.m_map = {};
	demo.m_map = {{4, 'a'}, {8, 'b'}, {12, 'c'}};
	demo.assign(0, 2, '!');
	printM(demo.m_map.begin(), demo.m_map.end());
	std::cout << "size: " << demo.m_map.size() << "\n";
	for (int i = 0; i < 15; i++)
	{
		cout << i << ": " << demo[i] << endl;
	}

	cout << endl
		 << "try 2" << endl;
	demo.assign(6, 10, 'a');
	printM(demo.m_map.begin(), demo.m_map.end());
	for (int i = 0; i < 15; i++)
	{
		cout << i << ": " << demo[i] << endl;
	}

	cout << "try 3" << endl
		 << endl;
	demo.assign(14, 16, 'e');
	printM(demo.m_map.begin(), demo.m_map.end());
	for (int i = 0; i < 20; i++)
	{
		cout << i << ": " << demo[i] << endl;
	}

	cout << "try 4" << endl
		 << endl;
	demo.assign(9, 16, '|');
	printM(demo.m_map.begin(), demo.m_map.end());
	for (int i = 0; i < 20; i++)
	{
		cout << i << ": " << demo[i] << endl;
	}

	interval_map<int, char> demo1('?');
	cout << endl
		 << endl
		 << "new one" << endl;
	demo1.m_map = {};
	demo1.m_map = {{4, 'a'}, {8, 'b'}, {12, 'c'}, {16, 'd'}};
	demo1.assign(2, 18, '!');
	// printM(demo.m_map.begin(), demo1.m_map.end());
	std::cout << "size: " << demo1.m_map.size() << "\n";
	for (int i = 0; i < 20; i++)
	{
		cout << i << ": " << demo1[i] << endl;
	}
}