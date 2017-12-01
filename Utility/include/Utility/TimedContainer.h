// Container to store elements throughout simulation time
// Usage: TimedContainer t; t(i) returns the value of the element i positions before present time; t.TimeAt(i) returns the time of the element i positions before present

#ifndef TIMED_MAP_H
#define TIMED_MAP_H

#include <vector>
#include <utility>
#include <algorithm>

template<class T>
class TimedContainer
{
	// TimePair is a pair ordered in time (first element of pair)
	class TimePair
	{
		typename std::pair<double, T> timePair;
	public:
		TimePair(const double& time, const T& value)
		{
			timePair = std::make_pair(time, value);
		}
	
		bool operator<(const TimePair& t2)
		{
			// More recent times go first
			return (timePair.first > t2.timePair.first);
		}
		
		const double& time() const {return timePair.first;}
		const T& value() const {return timePair.second;}
		
	};

	
	typedef typename std::vector<TimePair> timedObject;
	timedObject obj;


	class iterator
	{
		friend class TimedContainer<T>;
		typename timedObject::iterator itr;
	public:
		iterator(typename timedObject::iterator i) : itr(i) {}
		const double& time() const {return itr->time();}
		const T& value() const {return itr->value();}
		iterator operator++() {return iterator(++itr);}
		iterator operator--() {return iterator(--itr);}
		iterator operator++(int) {return iterator(itr++);}
		iterator operator--(int) {return iterator(itr--);}
		bool operator !=(const iterator& i) {return (itr != i.itr);}
	};

	
public:
	TimedContainer() {}
	void insert(const double& time, const T& element);
	void erase(iterator itr) {obj.erase(itr.itr);}
	void clear();
	iterator begin() {return iterator(obj.begin());}
	iterator begin() const {return iterator(obj.begin());}
	iterator last() {return iterator(--(obj.end()));}
	iterator last() const {return iterator(--(obj.end()));}
	iterator end() {return iterator(obj.end());}
	iterator end() const {return iterator(obj.end());}
	int size() const {return obj.size();}
};

template<class T>
void TimedContainer<T>::insert(const double& time, const T& element)
{
	bool needsSorting = false;
	
	// If you insert a newer element (time-wise), no need to sort (normal case)
	if (obj.size() > 0)
		if (time < obj.front().time())
			needsSorting = true;
	
	obj.insert(obj.begin(), TimePair(time, element));
	
	if (needsSorting)
		std::sort(obj.begin(), obj.end());
}

template<class T>
void TimedContainer<T>::clear()
{
	if (size() == 0)
		return;
	else if (size() == 1)
		erase(begin());
	else
	{
		for (auto itr = begin(); itr != end(); itr++)
		{
			erase(itr);
			itr--;
		}
	}
}


#endif
