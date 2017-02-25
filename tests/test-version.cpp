#include <cppunit/extensions/HelperMacros.h>
#include "TestFixtureWithParameter.h"
#include "TestTools.h"

#include "version.h"

template <typename charT>
class test_version : public CppUnit::TestFixtureWithParameter
{
    CPPUNIT_TEST_SUITE( test_version );
    CPPUNIT_TEST_WITH_PARAMETER( test_read );
    CPPUNIT_TEST_SUITE_END();

public:
    typedef std::basic_istream<charT> istream_t;
    typedef xml::basic_readable<charT> readable_t;
    typedef xml::basic_version<charT> version_t;


    std::vector<std::tuple<std::string, bool, int, int, std::string> > test_read_data()
    {
        return std::vector<std::tuple<std::string, bool, int, int, std::string> > {
            std::make_tuple("version = '1.0'",     true,  1,   0,   ""),
            std::make_tuple("version = '1.1'",     true,  1,   1,   ""),
            std::make_tuple("version = '255.255'", true,  255, 255, ""),
            std::make_tuple("version = '0.0'",     true,  0,   0,   ""),

            std::make_tuple("version = '1.'",      false, 0,   0,   "Expected version (at line 1 col 12)"),
            std::make_tuple("version = '.0'",      false, 0,   0,   "Expected version (at line 1 col 12)"),
            std::make_tuple("foo = '1.0'",         false, 0,   0,   "Expected name to be 'version' (at line 1 col 1)"),
        };
    }

    void test_read(std::string input_string, bool isSuccessful, int expectedMajor, int expectedMinor, std::string expectedError)
    {
        istream_t&& stream = create_input_stream<charT>(input_string);
        readable_t input(stream);
        version_t* version = version_t::read(input, false);

        if (isSuccessful)
        {
            CPPUNIT_ASSERT(nullptr != version);
            CPPUNIT_ASSERT_EQUAL((uint8_t)expectedMajor, version->major());
            CPPUNIT_ASSERT_EQUAL((uint8_t)expectedMinor, version->minor());

            delete version;
        }
        else
        {
            CPPUNIT_ASSERT(nullptr == version);
            CPPUNIT_ASSERT_STRING_EQUAL(input.get_last_error().what(), expectedError.c_str())
        }
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_version<char>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_version<char16_t>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_version<char32_t>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_version<wchar_t>);
