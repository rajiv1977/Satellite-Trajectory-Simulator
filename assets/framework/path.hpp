#ifndef __PATH_H__
#define __PATH_H__

#pragma once

UISOFT_BEGIN_NAMESPACE(backend)

class path : private boost::noncopyable
{
    public:
        template <typename T, typename F>
        static void enumerate(const T& xi_root, F xi_filter, bool xi_recursive, std::vector<T>& xo_result)
        {
#ifdef WINDOWS
            WIN32_FIND_DATA file_information;

            T root(xi_root);

            HANDLE hfile = ::FindFirstFile(T(root+_TEXT("\\*.*")).c_str(), &file_information);

            if(hfile != INVALID_HANDLE_VALUE)
            {
                do
                {
                    if(file_information.cFileName[0] != _TEXT('.'))
                    {
                        T path(root+_TEXT("\\")+file_information.cFileName);

                        if(file_information.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                        {
                            if(xi_recursive)
                            {
                                enumerate(path, xi_filter, xi_recursive, xo_result);
                            }
                        }
                        else
                        {
                            if(xi_filter(file_information.cFileName))
                            {
                                xo_result.push_back(path);
                            }
                        }
                    }

                } while(::FindNextFile(hfile, &file_information) == TRUE);
            }

            ::FindClose(hfile);
#else
#endif
        }

        template <typename T>
        static T absolute(const T& xi_relative_path)
        {
            string_t result;

#ifdef WINDOWS
            wchar buffer[MAX_PATH+1];

            if(::GetFullPathName(xi_relative_path.c_str(), ARRAY_SIZE(buffer), buffer, 0) > 0)
            {
                result = buffer;
            }
#else
#endif
            return result;
        }

        template <typename T>
        static T get_current_directory()
        {
            string_t result;

#ifdef WINDOWS
            wchar buffer[MAX_PATH+1];

            if(::GetCurrentDirectory(ARRAY_SIZE(buffer), buffer) > 0)
            {
                result = buffer;
                result += L"\\";
            }
#else
#endif
            return result;
        }

        template <typename T>
        static void set_current_directory(const T& xi_directory)
        {
#ifdef WINDOWS
            ::SetCurrentDirectory(xi_directory.c_str());
#else
#endif
        }
};

UISOFT_END_NAMESPACE

#endif // __PATH_H__
