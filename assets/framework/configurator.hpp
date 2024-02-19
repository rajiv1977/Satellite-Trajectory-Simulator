#ifndef __CONFIGURATOR_H__
#define __CONFIGURATOR_H__

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

#include <framework/text.hpp>
#include <framework/singleton.hpp>
#include <framework/converter.hpp>

#ifdef WINDOWS
#   pragma warning(default: 4244)
#   pragma warning(default: 4250)
#endif

UISOFT_BEGIN_NAMESPACE(backend)

class configurator : public singleton_shared_memory<configurator, 'cfgr'>
{
    private:
        typedef Arabica::SAX2DOM::Parser<string_t> xml_dom_parser_t;
        typedef Arabica::DOM::Document<string_t> xml_document_t;
        typedef Arabica::DOM::Element<string_t> xml_element_t;
        typedef Arabica::DOM::Comment<string_t> xml_comment_t;
        typedef Arabica::XPath::XPath<string_t> xpath_parser_t;
        typedef Arabica::XPath::XPathExpressionPtr<string_t> xpath_t;
        typedef Arabica::SAX::CatchErrorHandler<string_t> error_handler_t;
        typedef Arabica::SAX::InputSource<string_t> input_source_t;
        typedef Arabica::XPath::XPathValuePtr<string_t> xpath_result_t;
        typedef Arabica::XPath::XPathValuePtr<string_t> xpath_result_t;

        typedef std::auto_ptr<xml_dom_parser_t> xml_dom_parser_ptr_t;

        typedef tthread::recursive_mutex mutex_t;
        typedef tthread::lock_guard<mutex_t> guard_t;

    public:
        typedef Arabica::DOM::NamedNodeMap<string_t> attributes_t;
        typedef Arabica::DOM::Node<string_t> node_t;
        typedef Arabica::XPath::NodeSet<string_t> node_set_t;

    private:
        xml_dom_parser_ptr_t    my_xml_dom_parser;
        xpath_parser_t          my_xpath_parser;

        mutable mutex_t         my_mutex;

        bool                    my_loaded;

    public:
        configurator()
            : my_xml_dom_parser(new Arabica::SAX2DOM::Parser<string_t>()),
              my_loaded(false)
        {
        }

       ~configurator()
        {
            try
            {
                my_xml_dom_parser.release(); //??!!! broken arabica, need this statement
            }
            catch(const std::exception&)
            {
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

                xassert(rc && !error_handler.errorsReported(), x::einvalidarg, L"Invalid arguments: %s => %s.", xi_file_name.c_str(), converter::instance().convet_to_wstring(error_handler.errors().c_str()).c_str());

                my_loaded = true;
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void merge(const string_t& xi_query, const string_t& xi_file_name)
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            try
            {
                guard_t guard(my_mutex);

                node_t configurator_document(get_node(xi_query));

                xassert(configurator_document != 0, x::einvalidarg, L"Invalid arguments: %s.", xi_query.c_str());

                input_source_t input_source(xi_file_name);

                error_handler_t error_handler;

                xml_dom_parser_ptr_t xml_dom_parser(new Arabica::SAX2DOM::Parser<string_t>());

                (*xml_dom_parser).setErrorHandler(error_handler);

                bool rc = (*xml_dom_parser).parse(input_source);

                xassert(rc && !error_handler.errorsReported(), x::einvalidarg, L"Invalid arguments: %s => %s.", xi_file_name.c_str(), converter::instance().convet_to_wstring(error_handler.errors().c_str()).c_str());

                xpath_parser_t xpath_parser;

                xpath_t xpath(xpath_parser.compile_expr(L"/"));

                xml_document_t junction_document((*xml_dom_parser).getDocument());

                configurator_document.appendChild((*my_xml_dom_parser).getDocument().importNode(junction_document.getDocumentElement(), true));
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void save(const string_t& xi_file_name)
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            try
            {
                guard_t guard(my_mutex);

                xml_document_t document((*my_xml_dom_parser).getDocument());

                document.normalize();

                std::wofstream file_stream;

                file_stream.open(xi_file_name, std::ios::out|std::ios::trunc);

                xassert(file_stream.is_open() && !(file_stream.bad() || file_stream.fail()), x::einvalidarg, xi_file_name.c_str());

                file_stream << document;

                file_stream.flush();
                file_stream.close();
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void save_node(const string_t& xi_query, const string_t& xi_file_name)
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            try
            {
                guard_t guard(my_mutex);

                node_t document(get_node(xi_query));

                xassert(document != 0, x::einvalidarg, xi_query.c_str());

                document.normalize();

                std::wofstream file_stream;

                file_stream.open(xi_file_name, std::ios::out|std::ios::trunc);

                xassert(file_stream.is_open() && !(file_stream.bad() || file_stream.fail()), x::einvalidarg, xi_file_name.c_str());

                file_stream << document;

                file_stream.flush();
                file_stream.close();
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

    public:
        template <typename T> T get_value(const string_t& xi_query, T xi_result) const
        {
            xassert(false, x::enotimplemented, L"Invalid type.");
            return T;
        }

        template <> int get_value<int>(const string_t& xi_query, int xi_result) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            xassert(!xi_query.empty(), x::einvalidarg, L"Query is empty.");

            int result = xi_result;

            try
            {
                guard_t guard(my_mutex);

                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                string_t str(xpath_result->asString());

                if(!str.empty())
                {
                    result = converter::instance().parse_from_wstring<int>(str);
                }
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        template <> float get_value<float>(const string_t& xi_query, float xi_result) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            xassert(!xi_query.empty(), x::einvalidarg, L"Query is empty.");

            float result = xi_result;

            try
            {
                guard_t guard(my_mutex);

                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                string_t str(xpath_result->asString());

                if(!str.empty())
                {
                    result = converter::instance().parse_from_wstring<float>(str);
                }
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        template <> double get_value<double>(const string_t& xi_query, double xi_result) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            xassert(!xi_query.empty(), x::einvalidarg, L"Query is empty.");

            double result = xi_result;

            try
            {
                guard_t guard(my_mutex);

                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                string_t str(xpath_result->asString());

                if(!str.empty())
                {
                    result = converter::instance().parse_from_wstring<double>(str);
                }
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        template <> string_t get_value<string_t>(const string_t& xi_query, string_t xi_result) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            xassert(!xi_query.empty(), x::einvalidarg, L"Query is empty.");

            string_t result = xi_result;

            try
            {
                guard_t guard(my_mutex);

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

        template <> std::string get_value<std::string>(const string_t& xi_query, std::string xi_result) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            xassert(!xi_query.empty(), x::einvalidarg, L"Query is empty.");

            std::string result = xi_result;

            try
            {
                guard_t guard(my_mutex);

                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                string_t str(xpath_result->asString());

                if(!str.empty())
                {
                    result = text::wstring_to_string(str);
                }
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        template <> text_t get_value<text_t>(const string_t& xi_query, text_t xi_result) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            xassert(!xi_query.empty(), x::einvalidarg, L"Query is empty.");

            text_t result = xi_result;

            try
            {
                guard_t guard(my_mutex);

                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                string_t str(xpath_result->asString());

                if(!str.empty())
                {
                    result = str.c_str();
                }
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        template <> bool get_value<bool>(const string_t& xi_query, bool xi_result) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            xassert(!xi_query.empty(), x::einvalidarg, L"Query is empty.");

            bool result = xi_result;

            try
            {
                guard_t guard(my_mutex);

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
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            return get_value<T>(xi_query.c_str());
        }

        template <typename T> T get_value(const wchar* xi_query) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            xassert(xi_query != 0, x::einvalidarg, L"Query is empty.");

            T result = T();

            try
            {
                guard_t guard(my_mutex);

                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                result = converter::instance().parse_from_wstring<T>(xpath_result->asString());
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        template <> string_t get_value<string_t>(const string_t& xi_query) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            return get_value<string_t>(xi_query.c_str());
        }

        template <> string_t get_value<string_t>(const wchar* xi_query) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            xassert(xi_query != 0, x::einvalidarg, L"Query is empty.");

            string_t result;

            try
            {
                guard_t guard(my_mutex);

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

        template <> text_t get_value<text_t>(const string_t& xi_query) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            return get_value<text_t>(xi_query.c_str());
        }

        template <> text_t get_value<text_t>(const wchar* xi_query) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            xassert(xi_query != 0, x::einvalidarg, L"Query is empty.");

            text_t result;

            try
            {
                guard_t guard(my_mutex);

                xpath_t xpath(my_xpath_parser.compile_expr(xi_query));

                xml_document_t document((*my_xml_dom_parser).getDocument());

                xpath_result_t xpath_result(xpath->evaluate(document));

                string_t aux_result(xpath_result->asString());

                result = aux_result.c_str();
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        template <> bool get_value<bool>(const string_t& xi_query) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            return get_value<bool>(xi_query.c_str());
        }

        template <> bool get_value<bool>(const wchar* xi_query) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            xassert(xi_query != 0, x::einvalidarg, L"Query is empty.");

            bool result = false;

            try
            {
                guard_t guard(my_mutex);

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
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            return get_node(xi_query.c_str());
        }

        node_t get_node(const wchar* xi_query) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            xassert(xi_query != 0, x::einvalidarg, L"Query is empty.");

            node_t result;

            try
            {
                guard_t guard(my_mutex);

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

        void add_comment(const string_t& xi_comment)
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            try
            {
                guard_t guard(my_mutex);

                xml_comment_t comment_node((*my_xml_dom_parser).getDocument().createComment(xi_comment));

                (*my_xml_dom_parser).getDocument().appendChild(comment_node);
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void create_root_document(const std::string& xi_root_name)
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            try
            {
                guard_t guard(my_mutex);

                std::stringstream stream("<?xml version=\"1.0\" encoding=\"UTF-8\"?><"+xi_root_name+"/>");

                input_source_t input_source(stream);

                error_handler_t error_handler;

                (*my_xml_dom_parser).setErrorHandler(error_handler);

                bool rc = (*my_xml_dom_parser).parse(input_source);

                xassert(rc && !error_handler.errorsReported(), x::einvalidarg, converter::instance().convet_to_wstring(error_handler.errors().c_str()).c_str());
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void create_node(const string_t& xi_parent_node, const string_t& xi_node_name)
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            try
            {
                guard_t guard(my_mutex);

                node_t parent_node(get_node(xi_parent_node));

                if(parent_node != 0)
                {
                    xml_element_t new_node((*my_xml_dom_parser).getDocument().createElement(xi_node_name));

                    parent_node.appendChild(new_node);
                }
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void add_node(const string_t& xi_parent_node, const string_t& xi_node_name, const string_t& xi_value)
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            try
            {
                guard_t guard(my_mutex);

                node_t parent_node(get_node(xi_parent_node));

                if(parent_node != 0)
                {
                    xml_element_t new_node((*my_xml_dom_parser).getDocument().createElement(xi_node_name));

                    new_node.setNodeValue(xi_value);

                    parent_node.appendChild(new_node);
                }
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void add_node(const string_t& xi_parent_node, const string_t& xi_node_name, const string_t& xi_attribute_name, const string_t& xi_value)
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            try
            {
                guard_t guard(my_mutex);

                node_t parent_node(get_node(xi_parent_node));

                if(parent_node != 0)
                {
                    xml_element_t new_node((*my_xml_dom_parser).getDocument().createElement(xi_node_name));

                    new_node.setAttribute(xi_attribute_name, xi_value);

                    parent_node.appendChild(new_node);
                }
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void add_node(const string_t& xi_parent_node, const string_t& xi_node_name, const std::vector<std::pair<string_t, string_t> >& xi_attributes)
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            try
            {
                guard_t guard(my_mutex);

                node_t parent_node(get_node(xi_parent_node));

                if(parent_node != 0)
                {
                    xml_element_t new_node((*my_xml_dom_parser).getDocument().createElement(xi_node_name));

                    parent_node.appendChild(new_node);

                    for(std::vector<std::pair<string_t, string_t> >::const_iterator it(xi_attributes.begin()), it_end(xi_attributes.end()); it != it_end; ++it)
                    {
                        const string_t& attribute_name((*it).first);
                        const string_t& attribute_value((*it).second);

                        new_node.setAttribute(attribute_name, attribute_value);
                    }
                }
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void remove_nodes(const string_t& xi_parent, const string_t& xi_child)
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            try
            {
                guard_t guard(my_mutex);

                node_t parent_node(get_node(xi_parent));
                node_t node_to_remove;

                while(parent_node != 0 && (node_to_remove = get_child(parent_node, xi_child)) != 0)
                {
                    parent_node.purgeChild(node_to_remove);
                }
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void delete_nodes(const string_t& xi_parent)
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            try
            {
                guard_t guard(my_mutex);

                node_t parent_node(get_node(xi_parent));
                node_t node_to_remove;

                while(parent_node != 0 && (node_to_remove = parent_node.getFirstChild()) != 0)
                {
                    parent_node.purgeChild(node_to_remove);
                }
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void delete_node(const string_t& xi_query)
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            try
            {
                guard_t guard(my_mutex);

                node_t node_to_remove(get_node(xi_query));

				if(node_to_remove != 0 && node_to_remove.getParentNode() != 0)
                {
                    node_to_remove.getParentNode().purgeChild(node_to_remove);
                }
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        node_set_t get_values(const string_t& xi_query) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            return get_values(xi_query.c_str());
        }

        node_set_t get_values(const wchar* xi_query) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            xassert(xi_query != 0, x::einvalidarg, L"Query is empty.");

            node_set_t result;

            try
            {
                guard_t guard(my_mutex);

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

        static void create_directory(const string_t& xi_path)
        {
#ifdef WINDOWS
            string_t delimiter(L"\\");
#else
            string_t delimiter(L"/");
#endif
            string_t path(xi_path);

            std::vector<string_t> branches;
#ifdef WINDOWS
            text::replace(string_t(L"/"), delimiter, path);
#else
            text::replace(string_t(L"\\"), delimiter, path);
#endif
            text::split<string_t>(path, delimiter, branches);

            string_t sub_path;

            for(int i = 0, n = (int)branches.size(); i < n; i++)
            {
                sub_path += branches[i]+delimiter;
#ifdef WINDOWS
                ::CreateDirectory(sub_path.c_str(), 0);
#else
#endif
            }
        }

        node_t get_node_by_name(const node_t& xi_node, const string_t& xi_name) const
        {
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

            xassert(xi_node != 0, x::einvalidarg, L"Invalid argument.");

            node_t result;

            for(configurator::node_t node(xi_node.getFirstChild()); node != 0; node = node.getNextSibling())
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
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

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
            xassert(my_loaded, x::einvalidoperation, L"Configurator is not initialized");

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
};

UISOFT_END_NAMESPACE

#endif // __CONFIGURATOR_H__
