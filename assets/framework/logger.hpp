#ifndef __LOGGER_H__
#define __LOGGER_H__

#pragma once

#include <tinythread.h>

#include <framework/text.hpp>

#include <framework/iobservable.hpp>
#include <framework/singleton.hpp>

#if DEBUG || _DEBUG || DBG || _DBG
#   define DEFAULT_LOG_SEVERITY_LEVEL uisoft::etflib::backend::logger::edebug
#else
#   define DEFAULT_LOG_SEVERITY_LEVEL uisoft::etflib::backend::logger::ecritical
#endif

UISOFT_BEGIN_NAMESPACE(backend)

class logger : public singleton_shared_memory<logger, 'lggr'>, public iobservable<logger>
{
    typedef iobserver<logger> observer_t;
    typedef std::vector<observer_t*> observers_t;

    typedef tthread::mutex mutex_t;
    typedef tthread::lock_guard<mutex_t> guard_t;

    public:
        enum elogseverity
        {
            einfo = 0,
            ewarning,
            eerror,
            ecritical,
            edebug
        };

        struct logstate
        {
            wchar* message;
            elogseverity severity;
        };

    private:
        std::wofstream  my_stream;
        elogseverity    my_severity_level;
        bool            my_show_location;

        observers_t     my_observers;

        mutex_t         my_mutex;

    private:
        void format_time(int xi_buffer_size, wchar* xo_buffer)
        {
            time_t current_time;

            time(&current_time);

            tm new_time;

            localtime_s(&new_time, &current_time);

            UISNWPRINTF(xo_buffer, xi_buffer_size, CUT_SHORT, L"%02d%02d%04d %02d:%02d:%02d", new_time.tm_mday, new_time.tm_mon+1, new_time.tm_year+1900, new_time.tm_hour, new_time.tm_min, new_time.tm_sec);
        }

    public:
        logger() : my_show_location(false)
        {
        }

       ~logger()
        {
            try
            {
                uninitialize();
            }
            catch(const std::exception&)
            {
            }
        }

    public:
        void initialize(const string_t& xi_file_name)
        {
            if(my_stream.is_open())
            {
                my_stream.flush();
                my_stream.close();
            }

            my_stream.open(xi_file_name, std::ios::out|std::ios::app);

            xassert(my_stream.is_open() && !(my_stream.bad() || my_stream.fail()), x::einvalidarg, L"Unable initialize logger: %s.", xi_file_name.c_str());
        }

        void uninitialize()
        {
            try
            {
                if(my_stream.is_open())
                {
                    my_stream.flush();
                    my_stream.close();
                }
            }
            catch(const std::exception&)
            {
            }
        }

        void set_severity_level(elogseverity xi_severity_level)
        {
            my_severity_level = xi_severity_level;
        }

        void set_severity_level(const string_t& xi_severity_level)
        {
            if(xi_severity_level == L"DEBUG" || xi_severity_level == L"debug")
            {
                set_severity_level(logger::edebug);
            }
            else if(xi_severity_level == L"INFO" || xi_severity_level == L"info")
            {
                set_severity_level(logger::einfo);
            }
            else if(xi_severity_level == L"WARNING" || xi_severity_level == L"warning")
            {
                set_severity_level(logger::ewarning);
            }
            else if(xi_severity_level == L"ERROR" || xi_severity_level == L"error")
            {
                set_severity_level(logger::eerror);
            }
            else if(xi_severity_level == L"CRITICAL" || xi_severity_level == L"critical")
            {
                set_severity_level(logger::ecritical);
            }
            else // default
            {
                set_severity_level(DEFAULT_LOG_SEVERITY_LEVEL);
            }
        }

        void set_show_location(bool xi_show)
        {
            my_show_location = xi_show;
        }

        void log(const wchar* xi_file_name, int xi_line, const wchar* xi_function_name, elogseverity xi_severity_level, const wchar* xi_template, ...)
        {
            // [filename(line):function]:[datetime]:[level] message
            // datetime - ISO(day month year) hh:mm:ss.ms
            if(my_stream.is_open() && !(my_stream.bad() || my_stream.fail()))
            {
                if(xi_severity_level <= my_severity_level)
                {
                    guard_t guard(my_mutex);

                    wchar time_buffer[32];

                    format_time(ARRAY_SIZE(time_buffer), time_buffer);

                    wchar* severity_level_name;

                    switch(xi_severity_level)
                    {
                        case logger::edebug:
                            severity_level_name = L"DEBUG";
                            break;
                        case logger::einfo:
                            severity_level_name = L"INFO";
                            break;
                        case logger::ewarning:
                            severity_level_name = L"WARNING";
                            break;
                        case logger::eerror:
                            severity_level_name = L"ERROR";
                            break;
                        case logger::ecritical:
                            severity_level_name = L"CRITICAL";
                            break;
                        default:
                            severity_level_name = L"UNKNOWN";
                            break;
                    }

                    wchar buffer[4096];

                    if(my_show_location)
                    {
                        UISNWPRINTF(buffer, ARRAY_SIZE(buffer), CUT_SHORT, L"[%s]:[%s(%ld):%s][%s]  ", time_buffer, xi_file_name, xi_line, xi_function_name, severity_level_name);
                    }
                    else
                    {
                        UISNWPRINTF(buffer, ARRAY_SIZE(buffer), CUT_SHORT, L"[%s]:[%s]  ", time_buffer, severity_level_name);
                    }

                    my_stream << buffer;

                    va_list arguments;

                    va_start(arguments, xi_template);

                    UIVSNWPRINTF(buffer, ARRAY_SIZE(buffer), CUT_SHORT, xi_template, arguments);

                    va_end(arguments);

                    my_stream << buffer << std::endl;

                    my_stream.flush();

                    logstate state;

                    state.message = buffer;
                    state.severity = xi_severity_level;

                    notify_observers(&state);
                }

                my_stream.clear();
            }
        }

    public:
        virtual void register_observer(observer_t* xi_observer)
        {
            guard_t guard(my_mutex);

            my_observers.push_back(xi_observer);
        }

        virtual void unregister_observer(observer_t* xi_observer)
        {
            guard_t guard(my_mutex);

            observers_t::iterator it(std::find(my_observers.begin(), my_observers.end(), xi_observer));

            if(it != my_observers.end())
            {
                my_observers.erase(it);
            }
        }

        virtual void notify_observers(void* xi_data)
        {
            observers_t::iterator it(my_observers.begin());
            observers_t::iterator it_end(my_observers.end());

            for(; it != it_end; ++it)
            {
                observer_t& observer(*(*it));
                observer.notify_observer(*this, (wchar*)xi_data);
            }
        }
};

#if DEBUG || _DEBUG
#   define logdebug(__template, ...)    { uisoft::etflib::backend::logger::instance().log(TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__), uisoft::etflib::backend::logger::edebug, __template, ##__VA_ARGS__); }
#else
#   define logdebug(__template, ...)
#endif
#define loginfo(__template, ...)     { uisoft::etflib::backend::logger::instance().log(TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__), uisoft::etflib::backend::logger::einfo, __template, ##__VA_ARGS__); }
#define logwarning(__template, ...)  { uisoft::etflib::backend::logger::instance().log(TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__), uisoft::etflib::backend::logger::ewarning, __template, ##__VA_ARGS__); }
#define logerror(__template, ...)    { uisoft::etflib::backend::logger::instance().log(TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__), uisoft::etflib::backend::logger::eerror, __template, ##__VA_ARGS__); }
#define logcritical(__template, ...) { uisoft::etflib::backend::logger::instance().log(TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__), uisoft::etflib::backend::logger::ecritical, __template, ##__VA_ARGS__); }

#define logstdexception(xi_ex, xi_text, ...)                                    \
{                                                                               \
    if(xi_text != 0)                                                            \
    {                                                                           \
        logcritical(xi_text, ##__VA_ARGS__);                                    \
    }                                                                           \
    if(xi_ex.what() != 0)                                                       \
    {                                                                           \
        logerror(text::string_to_wstring(ex.what()).c_str(), ##__VA_ARGS__);    \
    }                                                                           \
}
#define logstdcriticalexception(xi_ex, xi_text, ...)                            \
{                                                                               \
    if(xi_text != 0)                                                            \
    {                                                                           \
        logcritical(xi_text, ##__VA_ARGS__);                                    \
    }                                                                           \
    if(xi_ex.what() != 0)                                                       \
    {                                                                           \
        logcritical(text::string_to_wstring(ex.what()).c_str());                \
    }                                                                           \
}

#define logexception(xi_ex, xi_text, ...)                                       \
{                                                                               \
    if(xi_text != 0)                                                            \
    {                                                                           \
        logcritical(xi_text, ##__VA_ARGS__);                                    \
    }                                                                           \
    logcritical(const_cast<x&>(xi_ex).why());                                   \
    /*if(xi_ex.what() != 0)                                                       \
    {                                                                           \
        logerror(text::string_to_wstring(ex.what()).c_str());                   \
    }*/                                                                           \
}
#define logcriticalexception(xi_ex, xi_text, ...)                               \
{                                                                               \
    if(xi_text != 0)                                                            \
    {                                                                           \
        logcritical(xi_text, ##__VA_ARGS__);                                    \
    }                                                                           \
    logcritical(const_cast<x&>(xi_ex).why());                                   \
    /*if(xi_ex.what() != 0)                                                       \
    {                                                                           \
        logcritical(text::string_to_wstring(ex.what()).c_str());                \
    }*/                                                                           \
}

UISOFT_END_NAMESPACE

#endif // __LOGGER_H__
