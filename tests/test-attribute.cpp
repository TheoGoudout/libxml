#include <cppunit/extensions/HelperMacros.h>
#include "TestFixtureWithParameter.h"
#include "TestTools.h"

#include <tuple>


#include "attribute.h"

template <typename charT>
class test_attribute : public CppUnit::TestFixtureWithParameter
{
    CPPUNIT_TEST_SUITE( test_attribute );
    CPPUNIT_TEST_WITH_PARAMETER( test_read );
    CPPUNIT_TEST_SUITE_END();

public:
    typedef std::basic_istream<charT> istream_t;
    typedef xml::basic_readable<charT> readable_t;
    typedef xml::basic_attribute<charT> attribute_t;


    std::vector<std::tuple<std::string, bool, std::string, std::string, std::string> > test_read_data()
    {
        return std::vector<std::tuple<std::string, bool, std::string, std::string, std::string> > {
            std::make_tuple("foo='bar'",      true,  "foo", "bar",      ""),
            std::make_tuple("foo='&entity;'", true,  "foo", "&entity;", ""),
            std::make_tuple("foo='&#1337;'",  true,  "foo", "&#1337;",  ""),
            std::make_tuple("foo='&#x1EE7;'", true,  "foo", "&#x1EE7;", ""),

            std::make_tuple("foo='&bar'",     false, "",    "",         "Expected ''' character (at line 1 col 6)"),
            std::make_tuple("foo='&#BAR;'",   false, "",    "&#BAR;",   "Expected ''' character (at line 1 col 6)"),
            std::make_tuple("foo='bar<bar'",  false, "",    "",         "Expected ''' character (at line 1 col 9)"),
        };
    }

    void test_read(
        std::string input_string,
        bool isSuccessful,
        std::string expectedName,
        std::string expectedValue,
        std::string expectedError)
    {
        istream_t&& stream = create_input_stream<charT>(input_string);
        readable_t input(stream);
        attribute_t* attribute = attribute_t::read(input, false);

        if (isSuccessful)
        {
            CPPUNIT_ASSERT(nullptr != attribute);
            CPPUNIT_ASSERT_STRING_EQUAL(convert<charT>(expectedName).c_str(), attribute->name().c_str())
            CPPUNIT_ASSERT_STRING_EQUAL(convert<charT>(expectedValue).c_str(), attribute->value().c_str())

            delete attribute;
        }
        else
        {
            CPPUNIT_ASSERT(nullptr == attribute);
            CPPUNIT_ASSERT_STRING_EQUAL(input.get_last_error().what(), expectedError.c_str())
        }
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_attribute<char>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_attribute<char16_t>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_attribute<char32_t>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_attribute<wchar_t>);
