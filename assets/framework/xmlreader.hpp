#ifndef __XMLREADER_H__
#define __XMLREADER_H__

#pragma once

#ifdef WINDOWS
#   include <windows.h>
#   pragma warning(disable: 4244)
#   pragma warning(disable: 4250)
#endif

#include <expat.h>

#include <SAX/helpers/CatchErrorHandler.hpp>
#include <DOM/SAX2DOM/SAX2DOM.hpp>
#include <DOM/io/Stream.hpp>
#include <XPath/XPath.hpp>

#ifdef WINDOWS
#   pragma warning(default: 4244)
#   pragma warning(default: 4250)
#endif

class xmlreader
{
    private:
        typedef wchar_t wchar;        
        typedef std::wstring string_t;

        typedef Arabica::SAX2DOM::Parser<string_t> xml_dom_parser_t;
        typedef Arabica::DOM::Document<string_t> xml_document_t;
        typedef Arabica::XPath::XPath<string_t> xpath_parser_t;
        typedef Arabica::XPath::XPathExpressionPtr<string_t> xpath_t;
        typedef Arabica::SAX::CatchErrorHandler<string_t> error_handler_t;
        typedef Arabica::SAX::InputSource<string_t> input_source_t;
        typedef Arabica::XPath::XPathValuePtr<string_t> xpath_result_t;

        typedef std::auto_ptr<xml_dom_parser_t> xml_dom_parser_ptr_t;

    public:
        typedef Arabica::DOM::NamedNodeMap<string_t> attributes_t;
        typedef Arabica::DOM::Node<string_t> node_t;
        typedef Arabica::XPath::NodeSet<string_t> node_set_t;

    private:
        xml_dom_parser_ptr_t    my_xml_dom_parser;
        xpath_parser_t          my_xpath_parser;

    public:
        xmlreader()
            : my_xml_dom_parser(new Arabica::SAX2DOM::Parser<string_t>())
        {
        }

       ~xmlreader()
        {
            try
            {
                my_xml_dom_parser.release(); //??!!! broken arabica, need this statement
            }
            catch(const std::exception&)
            {
            }
        }

        void load(const std::string& xi_file_name)
        {
            string_t file_name(xmlreader::string_to_wstring(xi_file_name));

            load(file_name);
        }

        void load(const string_t& xi_file_name)
        {
            try
            {
                input_source_t input_source(xi_file_name);

                error_handler_t error_handler;

                (*my_xml_dom_parser).setErrorHandler(error_handler);

                bool rc = (*my_xml_dom_parser).parse(input_source);

				if (!(rc && !error_handler.errorsReported()))
				{						
					std::string error = "Invalid file: " + xmlreader::wstring_to_string(xi_file_name) + ". " + error_handler.errors();
					throw std::exception(error.c_str());
				}
            }
            catch(const std::exception& ex)
            {
                throw ex;
            }
        }

    public:
        template <typename T> T get_value(const string_t& xi_query, T xi_result) const
        {
            return T;
        }

        template <> int get_value<int>(const string_t& xi_query, int xi_result) const
        {
            int result = xi_result;

            try
            {
                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                string_t str(xpath_result->asString());

                if(!str.empty())
                {
                    result = parse_from_wstring<int>(str);
                }
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        template <> float get_value<float>(const string_t& xi_query, float xi_result) const
        {
            float result = xi_result;

            try
            {
                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                string_t str(xpath_result->asString());

                if(!str.empty())
                {
                    result = parse_from_wstring<float>(str);
                }
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        template <> double get_value<double>(const string_t& xi_query, double xi_result) const
        {
            double result = xi_result;

            try
            {
                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                string_t str(xpath_result->asString());

                if(!str.empty())
                {
                    result = parse_from_wstring<double>(str);
                }
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        template <> string_t get_value<string_t>(const string_t& xi_query, string_t xi_result) const
        {
            string_t result = xi_result;

            try
            {
                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                string_t str(xpath_result->asString());

                if(!str.empty())
                {
                    result = str;
                }
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        template <> bool get_value<bool>(const string_t& xi_query, bool xi_result) const
        {
            bool result = xi_result;

            try
            {
                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                string_t str(xpath_result->asString());

                if(!str.empty())
                {
                    result = str.compare(L"true") == 0 || str.compare(L"1") == 0 || str.compare(L"on") == 0;
                }
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        template <typename T> T get_value(const string_t& xi_query) const
        {
            return get_value<T>(xi_query.c_str());
        }

        template <typename T> T get_value(const wchar* xi_query) const
        {
            T result = T();

            try
            {
                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                result = parse_from_wstring<T>(xpath_result->asString());
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        template <> string_t get_value<string_t>(const string_t& xi_query) const
        {
            return get_value<string_t>(xi_query.c_str());
        }

        template <> string_t get_value<string_t>(const wchar* xi_query) const
        {
            string_t result;

            try
            {
                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                result = xpath_result->asString();
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        template <> bool get_value<bool>(const string_t& xi_query) const
        {
            return get_value<bool>(xi_query.c_str());
        }

        template <> bool get_value<bool>(const wchar* xi_query) const
        {
            bool result = false;

            try
            {
                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                result = xpath_result->asString().compare(L"true") == 0 || xpath_result->asString().compare(L"1") == 0 || xpath_result->asString().compare(L"on") == 0;
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        node_t get_node(const string_t& xi_query) const
        {
            return get_node(xi_query.c_str());
        }

        node_t get_node(const wchar* xi_query) const
        {
            node_t result;

            try
            {
                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                node_set_t node_set(xpath_result->asNodeSet());

                if(!node_set.empty())
                {
                    result = xpath_result->asNodeSet()[0];
                }
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        void delete_node(const string_t& xi_query)
        {
            try
            {
                node_t parent_node(get_node(L"/configuration"));
                node_t node_to_remove(get_node(xi_query));

                if(parent_node != 0 && node_to_remove != 0)
                {
                    parent_node.purgeChild(node_to_remove);
                }
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        node_set_t get_values(const string_t& xi_query) const
        {
            return get_values(xi_query.c_str());
        }

        node_set_t get_values(const wchar* xi_query) const
        {
            node_set_t result;

            try
            {
                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                result = xpath_result->asNodeSet();
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        node_t get_node_by_name(const node_t& xi_node, const string_t& xi_name) const
        {
            node_t result;

            for(xmlreader::node_t node(xi_node.getFirstChild()); node != 0; node = node.getNextSibling())
            {
                if(node.getNodeName().compare(xi_name) == 0)
                {
                    result = node;
                    break;
                }
            }

            return result;
        }

        node_t get_child(const node_t& xi_parent, const string_t& xi_name) const
        {
            node_t result(0);

            if(xi_parent != 0)
            {
                for(node_t node = xi_parent.getFirstChild(); node != 0; node = node.getNextSibling()) 
                {
                    if(xi_name.compare(node.getNodeName()) == 0)
                    {
                        result = node;
                        break;
                    }
                }
            }

            return result;
        }

        node_set_t get_children(const node_t& xi_parent, const string_t& xi_name) const
        {
            node_set_t result(0);

            if(xi_parent != 0)
            {
                for(node_t node = xi_parent.getFirstChild(); node != 0; node = node.getNextSibling()) 
                {
                    if(xi_name.compare(node.getNodeName()) == 0)
                    {
                        result.push_back(node);
                    }
                }
            }

            return result;
        }

        string_t get_attribute(const node_t& xi_parent, const string_t& xi_name) const
        {
            string_t result;
            attributes_t attributes(xi_parent.getAttributes());

            for(int k = 0, n = (int)attributes.getLength(); k < n; k++)
            {
                const node_t& attribute(attributes.item(k));

                string_t value = attribute.getNodeValue();

                if(attribute.getNodeName().compare(xi_name) == 0)
                {
                    result = value;
                    break;
                }
            }

            return result;
        }

        template <typename T> std::string convet_to_string(const T& xi_value) const
        {
            std::ostringstream result;

            result << xi_value

            return result.str();
        }

        template <typename T> string_t convet_to_wstring(const T& xi_value) const
        {
            std::wostringstream result;

            result << xi_value

            return result.str();
        }

        template <typename T> T parse_from_string(const std::string& xi_value) const
        {
            typedef T value_t;

            value_t result = value_t();

            char leftovers;

            std::istringstream input(xi_value);

            input >> result;

            return (value_t)result;
        }

        template <typename T> T parse_from_wstring(const string_t& xi_value) const
        {
            typedef T value_t;

            value_t result = value_t();

            std::wistringstream input(xi_value);

            input >> result;

            return (value_t)result;
        }

        template <typename T> bool try_parse_from_string(const std::string& xi_value, T& xo_result) const
        {
            typedef T value_t;

            value_t result;

            std::istringstream input(xi_value);

            input >> result;

            xo_result = (value_t)result;

            return true;
        }

        template <typename T> bool try_parse_from_wstring(const string_t& xi_value, T& xo_result) const
        {
            typedef T value_t;

            value_t result;

            std::wistringstream input(xi_value);

            input >> result;

            xo_result = (value_t)result;

            return true;
        }

        #define STRING_CONVERSION_BUFFER_SIZE (512)

        static std::wstring string_to_wstring(const std::string& xi_s)
        {
#ifdef WINDOWS
            std::wstring result;

            int count = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, xi_s.c_str(), -1, 0, 0);

            if(count > 0)
            {
                if(count < STRING_CONVERSION_BUFFER_SIZE/8)
                {
                    wchar buffer[STRING_CONVERSION_BUFFER_SIZE];

                    count = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, xi_s.c_str(), -1, buffer, count);

                    if(count > 0)
                    {
                        result.assign(buffer);
                    }
                }
                else
                {
                    wchar p[MAX_PATH];

                    count = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, xi_s.c_str(), -1, p, count);

                    if(count > 0)
                    {
                        result = p;
                    }
                }
            }

            return result;
#else
            std::wstring aux(xi_s.length(), L' ');

            aux.assign(xi_s.begin(), xi_s.end());

            return aux;
#endif
        }

        static std::string wstring_to_string(const std::wstring& xi_s)
        {
#ifdef WINDOWS
            std::string result;

            int count = ::WideCharToMultiByte(CP_ACP, 0, xi_s.c_str(), -1, 0, 0, 0, 0);

            if(count > 0)
            {
                if(count < STRING_CONVERSION_BUFFER_SIZE/8)
                {
                    char buffer[STRING_CONVERSION_BUFFER_SIZE];

                    count = ::WideCharToMultiByte(CP_ACP, 0, xi_s.c_str(), -1, buffer, count, 0, 0);

                    if(count > 0)
                    {
                        result.assign(buffer);
                    }
                }
                else
                {
                    char p[MAX_PATH];

                    count = ::WideCharToMultiByte(CP_ACP, 0, xi_s.c_str(), -1, p, count, 0, 0);

                    if(count > 0)
                    {
                        result = p;
                    }
                }
            }

            return result;
#else
            std::string aux(xi_s.length(), L' ');

            aux.assign(xi_s.begin(), xi_s.end());

            return aux;
#endif
        }
};

#endif // __XMLREADER_H__
