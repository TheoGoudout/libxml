#include <cppunit/extensions/HelperMacros.h>

#include "parent-node-stub.h"
#include "child-node-stub.h"

template <typename charT>
class test_parent_node : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( test_parent_node );
    CPPUNIT_TEST( test_constructor );
    CPPUNIT_TEST( test_destructor );
    CPPUNIT_TEST( test_push_front );
    CPPUNIT_TEST( test_push_back );
    CPPUNIT_TEST_SUITE_END();

public:
    typedef parent_node_stub<charT> parent_node_t;
    typedef child_node_stub<charT>  child_node_t;

    void test_constructor()
    {
        // Default constructor
        {
            parent_node_t parent;

            CPPUNIT_ASSERT(parent.first() == nullptr);
            CPPUNIT_ASSERT(parent.last()  == nullptr);
        }
    }

    void test_destructor()
    {
        // Default destructor
        {
            parent_node_t parent;

            CPPUNIT_ASSERT(child_node_t::objectNumber() == 0);
            parent.first() = parent.last() = new child_node_t(&parent);
            CPPUNIT_ASSERT(child_node_t::objectNumber() == 1);
        }
        CPPUNIT_ASSERT(child_node_t::objectNumber() == 0);

    }

    void test_push_front()
    {
        parent_node_t parent;
        child_node_t child1;
        child_node_t child2;
        child_node_t child3;

        // Add one element
        {
            parent.push_front(child1);

            child_node_t* first = static_cast<child_node_t*>(parent.first());
            child_node_t* last  = static_cast<child_node_t*>(parent.last());

            CPPUNIT_ASSERT(first != nullptr);
            CPPUNIT_ASSERT(first != &child1);
            CPPUNIT_ASSERT(last  != nullptr);
            CPPUNIT_ASSERT(last  != &child1);

            CPPUNIT_ASSERT_EQUAL(first->id(), child1.id());
            CPPUNIT_ASSERT_EQUAL(last->id() , child1.id());
        }

        // Add two elements
        {
            parent.push_front(child2);

            child_node_t* first = static_cast<child_node_t*>(parent.first());
            child_node_t* last  = static_cast<child_node_t*>(parent.last());

            CPPUNIT_ASSERT(first != nullptr);
            CPPUNIT_ASSERT(first != &child2);
            CPPUNIT_ASSERT(last  != nullptr);
            CPPUNIT_ASSERT(last  != &child1);

            CPPUNIT_ASSERT_EQUAL(first->id(), child2.id());
            CPPUNIT_ASSERT_EQUAL(last->id() , child1.id());
        }

        // Add three elements
        {
            parent.push_front(child3);

            child_node_t* first  = static_cast<child_node_t*>(parent.first());
            child_node_t* second = static_cast<child_node_t*>(first->next());
            child_node_t* last   = static_cast<child_node_t*>(parent.last());

            CPPUNIT_ASSERT(first  != nullptr);
            CPPUNIT_ASSERT(first  != &child3);
            CPPUNIT_ASSERT(second != nullptr);
            CPPUNIT_ASSERT(second != &child2);
            CPPUNIT_ASSERT(last   != nullptr);
            CPPUNIT_ASSERT(last   != &child1);

            CPPUNIT_ASSERT_EQUAL(first->id() , child3.id());
            CPPUNIT_ASSERT_EQUAL(second->id(), child2.id());
            CPPUNIT_ASSERT_EQUAL(last->id()  , child1.id());
        }
    }

    void test_push_back()
    {
        parent_node_t parent;
        child_node_t child1;
        child_node_t child2;
        child_node_t child3;

        // Add one element
        {
            parent.push_back(child1);

            child_node_t* first = static_cast<child_node_t*>(parent.first());
            child_node_t* last  = static_cast<child_node_t*>(parent.last());

            CPPUNIT_ASSERT(first != nullptr);
            CPPUNIT_ASSERT(first != &child1);
            CPPUNIT_ASSERT(last  != nullptr);
            CPPUNIT_ASSERT(last  != &child1);

            CPPUNIT_ASSERT_EQUAL(first->id(), child1.id());
            CPPUNIT_ASSERT_EQUAL(last->id() , child1.id());
        }

        // Add two elements
        {
            parent.push_back(child2);

            child_node_t* first = static_cast<child_node_t*>(parent.first());
            child_node_t* last  = static_cast<child_node_t*>(parent.last());

            CPPUNIT_ASSERT(first != nullptr);
            CPPUNIT_ASSERT(first != &child1);
            CPPUNIT_ASSERT(last  != nullptr);
            CPPUNIT_ASSERT(last  != &child2);

            CPPUNIT_ASSERT_EQUAL(first->id(), child1.id());
            CPPUNIT_ASSERT_EQUAL(last->id() , child2.id());
        }

        // Add three elements
        {
            parent.push_back(child3);

            child_node_t* first = static_cast<child_node_t*>(parent.first());
            child_node_t* second = static_cast<child_node_t*>(first->next());
            child_node_t* last  = static_cast<child_node_t*>(parent.last());

            CPPUNIT_ASSERT(first  != nullptr);
            CPPUNIT_ASSERT(first  != &child1);
            CPPUNIT_ASSERT(second != nullptr);
            CPPUNIT_ASSERT(second != &child2);
            CPPUNIT_ASSERT(last   != nullptr);
            CPPUNIT_ASSERT(last   != &child3);

            CPPUNIT_ASSERT_EQUAL(first->id() , child1.id());
            CPPUNIT_ASSERT_EQUAL(second->id(), child2.id());
            CPPUNIT_ASSERT_EQUAL(last->id()  , child3.id());
        }
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_parent_node<char>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_parent_node<char16_t>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_parent_node<char32_t>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_parent_node<wchar_t>);
