#include <cppunit/extensions/HelperMacros.h>

#include "parent-node-stub.h"
#include "child-node-stub.h"

template <typename charT>
class test_child_node : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( test_child_node );
    CPPUNIT_TEST( test_default_constructor );
    CPPUNIT_TEST( test_copy_constructor );
    CPPUNIT_TEST_SUITE_END();

public:
    typedef parent_node_stub<charT> parent_t;
    typedef child_node_stub<charT>  child_t;

    void test_default_constructor()
    {
        child_t child;

        CPPUNIT_ASSERT(child.parent()   == nullptr);
        CPPUNIT_ASSERT(child.previous() == nullptr);
        CPPUNIT_ASSERT(child.next()     == nullptr);
    }

    void test_copy_constructor()
    {
        parent_t parent;
        child_t child(&parent);

        CPPUNIT_ASSERT(child.parent()   == &parent);
        CPPUNIT_ASSERT(child.previous() == nullptr);
        CPPUNIT_ASSERT(child.next()     == nullptr);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_child_node<char>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_child_node<char16_t>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_child_node<char32_t>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_child_node<wchar_t>);
