/* This is a simple template map with a string key and easy accessors
 */


#ifndef IMAP_H
#define IMAP_H

#include <map>
#include <string>
#include "LogFunctions.h"
#include "MyLogger.h"

template <class T>
class IMap
{
	typedef std::map<std::string, T> MapObj;
	MapObj obj;
public:
	inline const T& at(const std::string&) const;
	inline T& at(const std::string&);
	
	inline const T& operator()(const std::string&) const;
	inline T& operator()(const std::string&);
	
	inline void AddEntry(const std::string& key, const T& value);
	inline T& operator[](const std::string& key);
	inline void RemoveEntry(const std::string& key);
	
	inline bool IsAvailable(const std::string&) const;
		
	inline typename MapObj::const_iterator begin() const;
	inline typename MapObj::iterator begin();
	
	inline typename MapObj::const_iterator end() const;
	inline typename MapObj::iterator end();
	
	template <class U>
	friend inline MyLogger& operator<<(MyLogger&, const U&);
};

template <class T>
const T & IMap<T>::at(const std::string& key) const
{
	return obj.at(key);
}

template <class T>
T & IMap<T>::at(const std::string& key)
{
	return obj.at(key);
}

template <class T>
const T & IMap<T>::operator()(const std::string& key) const
{
	return obj.at(key);
}

template <class T>
T & IMap<T>::operator()(const std::string& key)
{
	return obj.at(key);
}

template<class T> 
T& IMap<T>::operator[](const std::string& key)
{
	return obj[key];
}

template<class T> 
void IMap<T>::AddEntry(const std::string& key, const T& value)
{
	obj[key] = value;
}

template<class T> 
void IMap<T>::RemoveEntry(const std::string& key)
{
	typename MapObj::iterator itr = obj.find(key);
	if (itr == obj.end())
		LogFunctions::Error("SimulationParameters::RemoveEntry", std::string("Parameter \'") + key + "\' not found");
	
	obj.erase(itr);
}

template<class T> 
typename IMap<T>::MapObj::const_iterator IMap<T>::begin() const
{
	return obj.begin();
}

template<class T> 
typename IMap<T>::MapObj::iterator IMap<T>::begin()
{
	return obj.begin();
}

template<class T> 
typename IMap<T>::MapObj::const_iterator IMap<T>::end() const
{
	return obj.end();
}

template<class T> 
typename IMap<T>::MapObj::iterator IMap<T>::end()
{
	return obj.end();
}

template<class T> 
bool IMap<T>::IsAvailable(const std::string& key) const
{
	bool isAvailable = true;
	
	try
	{
		at(key);
	}
	catch (std::out_of_range&)
	{
		isAvailable = false;
	}
	
	return isAvailable;
}




template <class U>
inline MyLogger& operator<<(MyLogger& os, const U& obj)
{
	os << "Map entries: " << os.EndL();
    os << "{" << os.EndL(MyLogger::INC);
    
	for (auto itr = obj.begin(); itr != obj.end(); itr++)
		os << itr->first << "\t" << itr->second << os.EndL();
	
	os << "}" << os.EndL(MyLogger::DEC);

    return os;
}



#endif
