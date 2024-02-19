#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#pragma once

#define BOOST_DATE_TIME_NO_LIB
#include <boost/interprocess/managed_shared_memory.hpp>

#include <tinythread.h>

UISOFT_BEGIN_NAMESPACE(backend)

template<typename T>
class singletonmanager : public T
{
    public:
        static bool& destroyed()
        {
            static bool destroyed = false;
            return destroyed;
        }

       ~singletonmanager()
        {
            try
            {
                singletonmanager::destroyed() = true;
            }
            catch(const std::exception&)
            {
            }
        }
};

class singletonguard : public boost::noncopyable
{
    private:
        static bool& get_lock()
        {
            static bool lock = false;
            return lock;
        }

    public:
        static void lock()
        {
            get_lock() = true;
        }

        static void unlock()
        {
            get_lock() = false;
        }

        static bool is_locked()
        {
            return get_lock();
        }
};

template <typename T>
class singleton : public singletonguard
{
    private:
        static T& our_instance;

    private:
        static void touch(const T&)
        {
            // triggers instantiation at pre-execution time (before main)
        }

    public:
        static T& instance()
        {
            xassert(!singletonmanager<T>::destroyed(), x::einvalidcontext, L"Singleton has been destroyed.");
            xassert(!singletonguard::is_locked(), x::einvalidoperation, L"Unable to get instance of locked singleton(s).");

            static singletonmanager<T> instance;

            touch(singleton<T>::our_instance);

            return static_cast<T&>(instance);
        }

        static bool is_destroyed()
        {
            return singletonmanager<T>::destroyed();
        }
};

template<typename T> T& singleton<T>::our_instance = singleton<T>::instance();

//
#define SHARED_MEMORY_NAME      ("multivis.sm:%x")
#define SHARED_MEMORY_COUNT     (32)
#define SHARED_MEMORY_SIZE      (SHARED_MEMORY_COUNT*128)
#define SHARED_MEMORY_TYPE_NAME ("multivis.sm.type")

template <typename T, unsigned long Id>
class singleton_shared_memory : public boost::noncopyable
{
    typedef tthread::recursive_mutex mutex_t;
    typedef tthread::lock_guard<mutex_t> guard_t;

    typedef unsigned long tag_t;

    typedef boost::shared_ptr<T> instance_t;

    struct singleton_entry
    {
        tag_t tag;
        instance_t instance;

        singleton_entry() : tag(-1)
        {
        }
    };

    typedef singleton_entry singleton_entry_t;

    private:
        static mutex_t  our_mutex;
        static T*       our_instance;

    public:
        static T& instance()
        {
            if(our_instance == 0)
            {
                guard_t guard(our_mutex);

                if(our_instance == 0)
                {
                    char buffer[SHARED_MEMORY_SIZE];

                    DWORD process = GetCurrentProcessId();

                    UISNPRINTF(buffer, ARRAY_SIZE(buffer), CUT_SHORT, SHARED_MEMORY_NAME, process);

                    boost::interprocess::managed_shared_memory shared_memory(boost::interprocess::open_or_create, buffer, SHARED_MEMORY_SIZE);

                    singleton_entry* entries(shared_memory.find_or_construct<singleton_entry>(SHARED_MEMORY_TYPE_NAME, std::nothrow)[SHARED_MEMORY_COUNT]());

                    if(entries != 0)
                    {
                        int i;

                        for(i = 0; i < SHARED_MEMORY_COUNT; i++)
                        {
                            const singleton_entry& entry(entries[i]);

                            if(entry.tag == -1)
                            {
                                break;
                            }

                            if(entry.tag == Id)
                            {
                                our_instance = entry.instance.get();
                                break;
                            }
                        }

                        if(our_instance == 0)
                        {
                            singleton_entry& entry(entries[i]);

                            entry.tag = Id;

                            entry.instance.reset(new T());

                            our_instance = entry.instance.get();
                        }
                    }
                }
            }

            return *our_instance;
        }

        static void cleanup()
        {
            char buffer[SHARED_MEMORY_SIZE];

            DWORD process = GetCurrentProcessId();

            UISNPRINTF(buffer, ARRAY_SIZE(buffer), CUT_SHORT, SHARED_MEMORY_NAME, process);

            boost::interprocess::managed_shared_memory shared_memory(boost::interprocess::open_or_create, buffer, SHARED_MEMORY_SIZE);

            std::pair<singleton_entry_t*, std::size_t> entries(shared_memory.find<singleton_entry_t>(SHARED_MEMORY_TYPE_NAME));

            if(entries.first != 0)
            {
                shared_memory.destroy_ptr(entries.first);
            }

            boost::interprocess::shared_memory_object::remove(buffer);
        }
};

template<typename T, unsigned long Id> typename singleton_shared_memory<T, Id>::mutex_t singleton_shared_memory<T, Id>::our_mutex;
template<typename T, unsigned long Id> T* singleton_shared_memory<T, Id>::our_instance(0);

class singleton_shared_memory_manager : public singleton<singleton_shared_memory_manager>
{
    class cleaner : public singleton_shared_memory<cleaner, 'clnr'>
    {
    };

    public:
        singleton_shared_memory_manager()
        {
        }

       ~singleton_shared_memory_manager()
        {
            cleaner::instance().cleanup();
        }
};

UISOFT_END_NAMESPACE

#endif // __SINGLETON_H__
