#include <cppunit/extensions/HelperMacros.h>

#include "parent-node-stub.h"
#include "child-node-stub.h"

template <typename charT>
class test_parent_node : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( test_parent_node );
    CPPUNIT_TEST( test_constructor );
    CPPUNIT_TEST( test_destructor );
    CPPUNIT_TEST( test_insert );
    CPPUNIT_TEST( test_push_front );
    CPPUNIT_TEST( test_push_back );
    CPPUNIT_TEST_SUITE_END();

public:
    typedef parent_node_stub<charT> parent_t;
    typedef child_node_stub<charT>  child_t;

    void test_constructor()
    {
        // Default constructor
        {
            parent_t parent;

            CPPUNIT_ASSERT(parent.first() == nullptr);
            CPPUNIT_ASSERT(parent.last()  == nullptr);
        }
    }

    void test_destructor()
    {
        // Default destructor
        {
            parent_t parent;

            CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
            parent.first() = parent.last() = new child_t(&parent);
            CPPUNIT_ASSERT_EQUAL(1, child_t::objectNumber());
        }
        CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());

    }

    void test_insert()
    {
        // Test basic_parent_node::insert(iterator, child_t&)
        {
            parent_t parent;
            child_t child1;
            child_t child2;
            child_t child3;

            {
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                parent.insert(parent.begin(), child1);
                CPPUNIT_ASSERT_EQUAL(4, child_t::objectNumber());

                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());

                CPPUNIT_ASSERT(first != nullptr);
                CPPUNIT_ASSERT(first != &child1);
                CPPUNIT_ASSERT(last  != nullptr);
                CPPUNIT_ASSERT(last  != &child1);

                CPPUNIT_ASSERT_EQUAL(first->id(), child1.id());
                CPPUNIT_ASSERT_EQUAL(last->id() , child1.id());
            }

            {
                CPPUNIT_ASSERT_EQUAL(4, child_t::objectNumber());
                parent.insert(parent.end(), child2);
                CPPUNIT_ASSERT_EQUAL(5, child_t::objectNumber());

                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());

                CPPUNIT_ASSERT(first != nullptr);
                CPPUNIT_ASSERT(first != &child1);
                CPPUNIT_ASSERT(last  != nullptr);
                CPPUNIT_ASSERT(last  != &child2);

                CPPUNIT_ASSERT_EQUAL(first->id(), child1.id());
                CPPUNIT_ASSERT_EQUAL(last->id() , child2.id());
            }

            {
                CPPUNIT_ASSERT_EQUAL(5, child_t::objectNumber());
                parent.insert(++(parent.begin()), child3);
                CPPUNIT_ASSERT_EQUAL(6, child_t::objectNumber());

                child_t* first  = static_cast<child_t*>(parent.first());
                child_t* second = static_cast<child_t*>(first->next());
                child_t* last   = static_cast<child_t*>(parent.last());

                CPPUNIT_ASSERT(first  != nullptr);
                CPPUNIT_ASSERT(first  != &child1);
                CPPUNIT_ASSERT(second != nullptr);
                CPPUNIT_ASSERT(second != &child3);
                CPPUNIT_ASSERT(last   != nullptr);
                CPPUNIT_ASSERT(last   != &child2);

                CPPUNIT_ASSERT_EQUAL(first->id() , child1.id());
                CPPUNIT_ASSERT_EQUAL(second->id(), child3.id());
                CPPUNIT_ASSERT_EQUAL(last->id()  , child2.id());
            }
        }
        CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());

        // Test basic_parent_node::insert(iterator, size_t, child_t&)
        {
            parent_t parent;
            child_t child;

            {
                CPPUNIT_ASSERT_EQUAL(1, child_t::objectNumber());
                parent.insert(parent.begin(), 3, child);
                CPPUNIT_ASSERT_EQUAL(4, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 3);

                child_t* first  = static_cast<child_t*>(parent.first());
                child_t* second = static_cast<child_t*>(first->next());
                child_t* last   = static_cast<child_t*>(parent.last());

                CPPUNIT_ASSERT(first  != nullptr);
                CPPUNIT_ASSERT(first  != &child);
                CPPUNIT_ASSERT(second != nullptr);
                CPPUNIT_ASSERT(second != &child);
                CPPUNIT_ASSERT(last   != nullptr);
                CPPUNIT_ASSERT(last   != &child);

                CPPUNIT_ASSERT_EQUAL(first->id() , child.id());
                CPPUNIT_ASSERT_EQUAL(second->id(), child.id());
                CPPUNIT_ASSERT_EQUAL(last->id()  , child.id());
            }
        }
        CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());

        // Test basic_parent_node::insert (iterator, InputIterator, InputIterator)
        {
            child_t child;
            parent_t dummy;
            dummy.insert(dummy.begin(), 3, child);

            parent_t parent;

            {
                CPPUNIT_ASSERT_EQUAL(4, child_t::objectNumber());
                parent.insert(parent.begin(), dummy.begin(), dummy.end());
                CPPUNIT_ASSERT_EQUAL(7, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 3);

                child_t* first  = static_cast<child_t*>(parent.first());
                child_t* second = static_cast<child_t*>(first->next());
                child_t* last   = static_cast<child_t*>(parent.last());

                CPPUNIT_ASSERT(first  != nullptr);
                CPPUNIT_ASSERT(first  != &child);
                CPPUNIT_ASSERT(second != nullptr);
                CPPUNIT_ASSERT(second != &child);
                CPPUNIT_ASSERT(last   != nullptr);
                CPPUNIT_ASSERT(last   != &child);

                CPPUNIT_ASSERT_EQUAL(first->id() , child.id());
                CPPUNIT_ASSERT_EQUAL(second->id(), child.id());
                CPPUNIT_ASSERT_EQUAL(last->id()  , child.id());
            }
        }
    }

    void test_push_front()
    {
        parent_t parent;
        child_t child1;
        child_t child2;
        child_t child3;

        // Add one element
        {
            parent.push_front(child1);

            child_t* first = static_cast<child_t*>(parent.first());
            child_t* last  = static_cast<child_t*>(parent.last());

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

            child_t* first = static_cast<child_t*>(parent.first());
            child_t* last  = static_cast<child_t*>(parent.last());

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

            child_t* first  = static_cast<child_t*>(parent.first());
            child_t* second = static_cast<child_t*>(first->next());
            child_t* last   = static_cast<child_t*>(parent.last());

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
        parent_t parent;
        child_t child1;
        child_t child2;
        child_t child3;

        // Add one element
        {
            parent.push_back(child1);

            child_t* first = static_cast<child_t*>(parent.first());
            child_t* last  = static_cast<child_t*>(parent.last());

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

            child_t* first = static_cast<child_t*>(parent.first());
            child_t* last  = static_cast<child_t*>(parent.last());

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

            child_t* first = static_cast<child_t*>(parent.first());
            child_t* second = static_cast<child_t*>(first->next());
            child_t* last  = static_cast<child_t*>(parent.last());

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
