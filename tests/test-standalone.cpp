#include <cppunit/extensions/HelperMacros.h>
#include "TestFixtureWithParameter.h"
#include "TestTools.h"

#include <tuple>


#include "standalone.h"

template <typename charT>
class test_standalone : public CppUnit::TestFixtureWithParameter
{
    CPPUNIT_TEST_SUITE( test_standalone );
    CPPUNIT_TEST_WITH_PARAMETER( test_read );
    CPPUNIT_TEST_SUITE_END();

public:
    typedef std::basic_istream<charT> istream_t;
    typedef xml::basic_readable<charT> readable_t;
    typedef xml::basic_standalone<charT> standalone_t;


    std::vector<std::tuple<std::string, bool, bool, std::string> > test_read_data()
    {
        return std::vector<std::tuple<std::string, bool, bool, std::string> > {
            std::make_tuple("standalone = 'yes'", true,  true,  ""),
            std::make_tuple("standalone = 'no'",  true,  false, ""),

            std::make_tuple("standalone = 'foo'", false, false, "Expected 'yes' or 'no' value (at line 1 col 15)"),
            std::make_tuple("foo = 'yes'",        false, false, "Expected name to be 'standalone' (at line 1 col 1)"),
        };
    }

    void test_read(std::string input_string, bool isSuccessful, bool expectedStandalone, std::string expectedError)
    {
        istream_t&& stream = create_input_stream<charT>(input_string);
        readable_t input(stream);
        standalone_t* standalone = standalone_t::read(input, false);

        if (isSuccessful)
        {
            CPPUNIT_ASSERT(nullptr != standalone);
            CPPUNIT_ASSERT_EQUAL(expectedStandalone, standalone->standalone());

            delete standalone;
        }
        else
        {
            CPPUNIT_ASSERT(nullptr == standalone);
            CPPUNIT_ASSERT_STRING_EQUAL(input.get_last_error().what(), expectedError.c_str())
        }
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_standalone<char>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_standalone<char16_t>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_standalone<char32_t>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_standalone<wchar_t>);
