#include <cppunit/TestFixture.h>

#include "TestCallerWithParameter.h"

#define CPPUNIT_TEST_WITH_PARAMETER( test_name )         \
    auto fixture = context.makeFixture();                \
    auto data = fixture->test_name ## _data();           \
    for (auto it = data.begin(); it != data.end(); ++it) \
    {                                                    \
        context.addTest(                                 \
            createTestWithParameter(                     \
                context.getTestNameFor( #test_name ),    \
                &TestFixtureType::test_name,             \
                *it,                                     \
                context.makeFixture()                    \
            )                                            \
        );                                               \
    }                                                    \
    delete fixture;


namespace CPPUNIT_NS
{
    class TestFixtureWithParameter : public TestFixture
    {
    protected:
        template <typename TestFixtureType, typename funcT, typename ... Args>
        static CPPUNIT_NS::Test* createTestWithParameter(
            std::string name,
            funcT func,
            std::tuple<Args ...>& tuple,
            TestFixtureType* fixture)
        {
            return new CPPUNIT_NS::TestCallerWithParameter<TestFixtureType, Args ...>(
                name,
                func,
                tuple,
                fixture
            );
        }
    };
}
