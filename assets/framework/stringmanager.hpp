#ifndef __STRINGMANAGER_H__
#define __STRINGMANAGER_H__

#pragma once

#ifdef WINDOWS
#   pragma warning(disable: 4244)
#   pragma warning(disable: 4250)
#endif

#include <expat.h>

#include <SAX/helpers/CatchErrorHandler.hpp>
#include <DOM/SAX2DOM/SAX2DOM.hpp>
#include <DOM/io/Stream.hpp>
#include <XPath/XPath.hpp>

#include <tinythread.h>

#include <framework/singleton.hpp>
#include <framework/logger.hpp>

#ifdef WINDOWS
#   pragma warning(default: 4244)
#   pragma warning(default: 4250)
#endif

UISOFT_BEGIN_NAMESPACE(backend)

class stringmanager : public singleton_shared_memory<stringmanager, 'smgr'>
{
    private:
        typedef Arabica::SAX2DOM::Parser<string_t> xml_dom_parser_t;
        typedef Arabica::DOM::Document<string_t> xml_document_t;
        typedef Arabica::XPath::XPath<string_t> xpath_parser_t;
        typedef Arabica::XPath::XPathExpressionPtr<string_t> xpath_t;
        typedef Arabica::SAX::CatchErrorHandler<string_t> error_handler_t;
        typedef Arabica::SAX::InputSource<string_t> input_source_t;
        typedef Arabica::XPath::XPathValuePtr<string_t> xpath_result_t;

        typedef std::auto_ptr<xml_dom_parser_t> xml_dom_parser_ptr_t;

        typedef tthread::recursive_mutex mutex_t;
        typedef tthread::lock_guard<mutex_t> guard_t;

        typedef std::map<string_t, string_t> cache_t;

    public:
        typedef Arabica::DOM::NamedNodeMap<string_t> attributes_t;
        typedef Arabica::DOM::Node<string_t> node_t;
        typedef Arabica::XPath::NodeSet<string_t> node_set_t;

    private:
        xml_dom_parser_ptr_t    my_xml_dom_parser;
        xpath_parser_t          my_xpath_parser;

        cache_t                 my_cache;

        mutable mutex_t         my_mutex;

    public:
        stringmanager()
            : my_xml_dom_parser(new Arabica::SAX2DOM::Parser<string_t>())
        {
        }

       ~stringmanager()
        {
            try
            {
                my_xml_dom_parser.release(); //??!!! broken arabica, need this statement
            }
            catch(const std::exception& ex)
            {
                logstdcriticalexception(ex, L"dtor");
            }
        }

        void load(const string_t& xi_file_name)
        {
            try
            {
                guard_t guard(my_mutex);

                input_source_t input_source(xi_file_name);

                error_handler_t error_handler;

                (*my_xml_dom_parser).setErrorHandler(error_handler);

                bool rc = (*my_xml_dom_parser).parse(input_source);

                xassert(rc && !error_handler.errorsReported(), x::einvalidarg, L"Unable to initialize configurator: %s => %s.", xi_file_name.c_str(), error_handler.errors());
            }
            catch(const std::exception& ex)
            {
                logstdcriticalexception(ex, L"Unable to load stringmanager.");
                throw;
            }
        }

    public:
        string_t get_text(const string_t& xi_query)
        {
            return get_text(xi_query.c_str());
        }

        string_t get_text(const wchar* xi_query)
        {
            xassert(xi_query != 0, x::einvalidarg, L"Query is empty.");

            string_t result;

            try
            {
                guard_t guard(my_mutex);

                cache_t::const_iterator it(my_cache.find(xi_query));

                if(it != my_cache.end())
                {
                    result = (*it).second;
                }
                else
                {
                    xpath_parser_t xpath_parser;

                    xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                    xml_document_t document((*my_xml_dom_parser).getDocument());

                    xpath_result_t xpath_result(xpath->evaluate(document));

                    result = xpath_result->asString();

                    string_t query(xi_query);

                    my_cache.insert(cache_t::value_type(query, result));
                }
            }
            catch(const std::exception& ex)
            {
                std::string m(ex.what());
            }

            return result;
        }
};

UISOFT_END_NAMESPACE

#endif // __STRINGMANAGER_H__
