#include <cppunit/extensions/HelperMacros.h>
#include "TestFixtureWithParameter.h"
#include "TestTools.h"

#include <cstring>

#include "exception.h"

class test_exception : public CppUnit::TestFixtureWithParameter
{
    CPPUNIT_TEST_SUITE( test_exception );
    CPPUNIT_TEST( test_parsing_exception );
    CPPUNIT_TEST_SUITE_END();

public:
    typedef xml::exception exception_t;
    typedef xml::parsing_exception parsing_exception_t;

    void test_parsing_exception()
    {
        {
            const char* input = "Unknown error";
            exception_t* e = parsing_exception_t::create_parsing_exception(input);

            const char* expected = "Unknown error";
            CPPUNIT_ASSERT(strcmp(e->what(), expected) == 0);

            delete e;
        }

        {
            const char* input = "Expected : %s; Actual : %s";
            exception_t* e = parsing_exception_t::create_parsing_exception(input, "foo", "bar");

            const char* expected = "Expected : foo; Actual : bar";
            CPPUNIT_ASSERT(strcmp(e->what(), expected) == 0);

            delete e;
        }
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_exception);
