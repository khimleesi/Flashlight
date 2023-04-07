#pragma once

/*******************************************************************************************************************
	Singleton.h, Singleton.cpp
	Created by Kim Kane
	Last updated: 06/01/2018
	Class finalized: 18/01/1018

	A thread-safe singleton class that generates one instance of a template object on the heap.
	
	I use a Windows defined CRITICAL_SECTION for the following reasons:
	A critical section object provides synchronization similar to that provided by a mutex object,
	except that a critical section can be used only by the threads of a single process. 
	Critical section objects cannot be shared across processes.

	Event, mutex, and semaphore objects can also be used in a single-process application,
	but critical section objects provide a slightly faster, more efficient mechanism for mutual-exclusion synchronization 
	(a processor-specific test and set instruction). Like a mutex object, a critical section object can be owned by only 
	one thread at a time, which makes it useful for protecting a shared resource from simultaneous access. 

	Using a critical section, I am able to generate a lock and unlock mechanism.
	This means that in the event of using multiple threads, if both threads try to access the Instance() function
	at the same time, only one instance will ever be created due to the lock and unlock procedures.

	Effectively, the first thread to reach the lock will create the instance. The other thread will 'wait'
	for the lock to be unlocked, and then as the instance has already been created, will just return that instance.

	I am also using an unique_ptr* to eliminate memory leaks, as we create the singleton object on the heap.

	*std::unique_ptr is responsible for managing dynamically allocated memory and automatically 
	 calls delete to free the dynamic memory when the unique_ptr is destroyed or goes out of scope.

*******************************************************************************************************************/
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>

class CriticalSection : public CRITICAL_SECTION {

public:
	CriticalSection()	{ InitializeCriticalSection(this); }
	~CriticalSection()	{ DeleteCriticalSection(this); }

private:
	CriticalSection(CriticalSection const&)				= delete;
	CriticalSection& operator=(CriticalSection const&)	= delete;
};

template <class T>
class Singleton {

public:
	static T* Instance();

private:
	Singleton()								= default;
	~Singleton()							= default;
	Singleton(Singleton const&)				= delete;
	Singleton& operator=(Singleton const&)	= delete;

	static CriticalSection s_singletonLock;
};


/*******************************************************************************************************************
	Create a static lock object, which is initialized before main() is called
*******************************************************************************************************************/
template <class T> CriticalSection Singleton<T>::s_singletonLock;


/*******************************************************************************************************************
	Lock the procedure, check to see if we need to create a new instance, then unlock procedure
*******************************************************************************************************************/
template <class T> T* Singleton<T>::Instance() {

	EnterCriticalSection(&s_singletonLock);
		static std::unique_ptr<T> s_singletonObject(new T());
	LeaveCriticalSection(&s_singletonLock);

	return s_singletonObject.get();
}