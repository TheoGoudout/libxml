#include <cppunit/extensions/HelperMacros.h>
#include "TestFixtureWithParameter.h"
#include "TestTools.h"

#include <tuple>


#include "encoding.h"

template <typename charT>
class test_encoding : public CppUnit::TestFixtureWithParameter
{
    CPPUNIT_TEST_SUITE( test_encoding );
    CPPUNIT_TEST_WITH_PARAMETER( test_read );
    CPPUNIT_TEST_SUITE_END();

public:
    typedef std::basic_istream<charT> istream_t;
    typedef xml::basic_readable<charT> readable_t;
    typedef xml::basic_encoding<charT> encoding_t;


    std::vector<std::tuple<std::string, bool, std::string, std::string> > test_read_data()
    {
        return std::vector<std::tuple<std::string, bool, std::string, std::string> > {
            std::make_tuple("encoding = 'UTF-8'",  true,  "UTF-8",  ""),
            std::make_tuple("encoding = 'UTF-16'", true,  "UTF-16", ""),
            std::make_tuple("encoding = 'UTF-32'", true,  "UTF-32", ""),

            std::make_tuple("encoding = '42'",     false, "",       "Expected valid XML encoding name (at line 1 col 13)"),
            std::make_tuple("foo = 'UTF-8'",       false, "",       "Expected name to be 'encoding' (at line 1 col 1)"),
        };
    }

    void test_read(std::string input_string, bool isSuccessful, std::string expectedEncoding, std::string expectedError)
    {
        istream_t&& stream = create_input_stream<charT>(input_string);
        readable_t input(stream);
        encoding_t* encoding = encoding_t::read(input, false);

        if (isSuccessful)
        {
            CPPUNIT_ASSERT(nullptr != encoding);
            CPPUNIT_ASSERT_STRING_EQUAL(convert<charT>(expectedEncoding).c_str(), encoding->encoding().c_str())

            delete encoding;
        }
        else
        {
            CPPUNIT_ASSERT(nullptr == encoding);
            CPPUNIT_ASSERT_STRING_EQUAL(input.get_last_error().what(), expectedError.c_str())
        }
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_encoding<char>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_encoding<char16_t>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_encoding<char32_t>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_encoding<wchar_t>);
