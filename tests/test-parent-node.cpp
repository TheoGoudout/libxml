#include <cppunit/extensions/HelperMacros.h>

#include "parent-node-stub.h"
#include "child-node-stub.h"

template <typename charT>
class test_parent_node : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( test_parent_node );
    CPPUNIT_TEST( test_constructor_default );
    CPPUNIT_TEST( test_destructor );
    CPPUNIT_TEST( test_insert_single_element );
    CPPUNIT_TEST( test_insert_fill );
    CPPUNIT_TEST( test_insert_range );
    CPPUNIT_TEST( test_insert_move );
    CPPUNIT_TEST( test_insert_initializer_list );
    CPPUNIT_TEST( test_push_front_single_element );
    CPPUNIT_TEST( test_push_front_move );
    CPPUNIT_TEST( test_push_back_single_element );
    CPPUNIT_TEST( test_push_back_move );
    CPPUNIT_TEST( test_emplace );
    CPPUNIT_TEST( test_emplace_front );
    CPPUNIT_TEST( test_emplace_back );
    CPPUNIT_TEST_SUITE_END();

public:
    typedef parent_node_stub<charT> parent_t;
    typedef child_node_stub<charT>  child_t;

    void test_constructor_default()
    {
        {
            parent_t parent;

            CPPUNIT_ASSERT(parent.first() == nullptr);
            CPPUNIT_ASSERT(parent.last()  == nullptr);
            CPPUNIT_ASSERT(parent.size() == 0);
        }
    }

    void test_destructor()
    {
        {
            parent_t parent;

            CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
            parent.push_front(std::move(child_t((parent_t*)0xdeadbeef)));
            CPPUNIT_ASSERT_EQUAL(1, child_t::objectNumber());
        }
        CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());

    }

    void test_insert_single_element()
    {
        {
            parent_t parent;
            child_t child1;
            child_t child2;
            child_t child3;

            {
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                parent.insert(parent.begin(), child1);
                CPPUNIT_ASSERT_EQUAL(4, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 1);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first             == last);
                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == nullptr);

                CPPUNIT_ASSERT(first->copyConstructed());
                CPPUNIT_ASSERT(first->id() == child1.id());
            }

            {
                CPPUNIT_ASSERT_EQUAL(4, child_t::objectNumber());
                parent.insert(parent.end(), child2);
                CPPUNIT_ASSERT_EQUAL(5, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 2);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == last);

                CPPUNIT_ASSERT(first->copyConstructed());
                CPPUNIT_ASSERT(first->id() == child1.id());


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == first);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(last->copyConstructed());
                CPPUNIT_ASSERT(last->id() == child2.id());
            }

            {
                CPPUNIT_ASSERT_EQUAL(5, child_t::objectNumber());
                parent.insert(++(parent.begin()), child3);
                CPPUNIT_ASSERT_EQUAL(6, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 3);


                child_t* first  = static_cast<child_t*>(parent.first());
                child_t* second = static_cast<child_t*>(first->next());
                child_t* last   = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == second);

                CPPUNIT_ASSERT(first->copyConstructed());
                CPPUNIT_ASSERT(first->id() == child1.id());


                CPPUNIT_ASSERT(second->parent()   == &parent);
                CPPUNIT_ASSERT(second->previous() == first);
                CPPUNIT_ASSERT(second->next()     == last);

                CPPUNIT_ASSERT(second->copyConstructed());
                CPPUNIT_ASSERT(second->id() == child3.id());


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == second);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(last->copyConstructed());
                CPPUNIT_ASSERT(last->id() == child2.id());
            }
        }
        CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
    }

    void test_insert_fill()
    {
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


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == second);

                CPPUNIT_ASSERT(first->copyConstructed());
                CPPUNIT_ASSERT(first->id() == child.id());


                CPPUNIT_ASSERT(second->parent()   == &parent);
                CPPUNIT_ASSERT(second->previous() == first);
                CPPUNIT_ASSERT(second->next()     == last);

                CPPUNIT_ASSERT(second->copyConstructed());
                CPPUNIT_ASSERT(second->id() == child.id());


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == second);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(last->copyConstructed());
                CPPUNIT_ASSERT(last->id() == child.id());
            }
        }
        CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
    }

    void test_insert_range()
    {
        {
            child_t child1;
            child_t child2;
            child_t child3;
            parent_t dummy;
            dummy.insert(dummy.end(), child1);
            dummy.insert(dummy.end(), child2);
            dummy.insert(dummy.end(), child3);

            parent_t parent;

            {
                CPPUNIT_ASSERT_EQUAL(6, child_t::objectNumber());
                parent.insert(parent.begin(), dummy.begin(), dummy.end());
                CPPUNIT_ASSERT_EQUAL(9, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 3);


                child_t* first  = static_cast<child_t*>(parent.first());
                child_t* second = static_cast<child_t*>(first->next());
                child_t* last   = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == second);

                CPPUNIT_ASSERT(first->copyConstructed());
                CPPUNIT_ASSERT(first->id() == child1.id());


                CPPUNIT_ASSERT(second->parent()   == &parent);
                CPPUNIT_ASSERT(second->previous() == first);
                CPPUNIT_ASSERT(second->next()     == last);

                CPPUNIT_ASSERT(second->copyConstructed());
                CPPUNIT_ASSERT(second->id() == child2.id());


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == second);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(last->copyConstructed());
                CPPUNIT_ASSERT(last->id() == child3.id());
            }
        }
        CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
    }

    void test_insert_move()
    {
        {
            parent_t parent;
            child_t child1;
            child_t child2;
            child_t child3;

            int id1 = child1.id();
            int id2 = child2.id();
            int id3 = child3.id();

            {
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                parent.insert(parent.begin(), std::move(child1));
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 1);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first             == last);
                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == nullptr);

                CPPUNIT_ASSERT(first->moveConstructed());
                CPPUNIT_ASSERT(first->id() == id1);
            }

            {
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                parent.insert(parent.end(), std::move(child2));
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 2);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == last);

                CPPUNIT_ASSERT(first->moveConstructed());
                CPPUNIT_ASSERT(first->id() == id1);


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == first);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(last->moveConstructed());
                CPPUNIT_ASSERT(last->id() == id2);
            }

            {
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                parent.insert(++(parent.begin()), std::move(child3));
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 3);


                child_t* first  = static_cast<child_t*>(parent.first());
                child_t* second = static_cast<child_t*>(first->next());
                child_t* last   = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == second);

                CPPUNIT_ASSERT(first->moveConstructed());
                CPPUNIT_ASSERT(first->id() == id1);


                CPPUNIT_ASSERT(second->parent()   == &parent);
                CPPUNIT_ASSERT(second->previous() == first);
                CPPUNIT_ASSERT(second->next()     == last);

                CPPUNIT_ASSERT(second->moveConstructed());
                CPPUNIT_ASSERT(second->id() == id3);


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == second);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(last->moveConstructed());
                CPPUNIT_ASSERT(last->id() == id2);
            }
        }
        CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
    }

    void test_insert_initializer_list()
    {
        {
            parent_t parent;
            child_t child1;
            child_t child2;
            child_t child3;

            {
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                parent.insert(parent.begin(), { child1, child2, child3 });
                CPPUNIT_ASSERT_EQUAL(6, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 3);


                child_t* first  = static_cast<child_t*>(parent.first());
                child_t* second = static_cast<child_t*>(first->next());
                child_t* last   = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == second);

                CPPUNIT_ASSERT(first->copyConstructed());
                CPPUNIT_ASSERT(first->id() == child1.id());


                CPPUNIT_ASSERT(second->parent()   == &parent);
                CPPUNIT_ASSERT(second->previous() == first);
                CPPUNIT_ASSERT(second->next()     == last);

                CPPUNIT_ASSERT(second->copyConstructed());
                CPPUNIT_ASSERT(second->id() == child2.id());


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == second);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(last->copyConstructed());
                CPPUNIT_ASSERT(last->id() == child3.id());
            }
        }
        CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
    }

    void test_push_front_single_element()
    {
        {
            parent_t parent;
            child_t child1;
            child_t child2;
            child_t child3;

            {
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                parent.push_front(child1);
                CPPUNIT_ASSERT_EQUAL(4, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 1);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first             == last);
                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == nullptr);

                CPPUNIT_ASSERT(first->copyConstructed());
                CPPUNIT_ASSERT(first->id() == child1.id());
            }

            {
                CPPUNIT_ASSERT_EQUAL(4, child_t::objectNumber());
                parent.push_front(child2);
                CPPUNIT_ASSERT_EQUAL(5, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 2);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == last);

                CPPUNIT_ASSERT(first->copyConstructed());
                CPPUNIT_ASSERT(first->id() == child2.id());


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == first);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(last->copyConstructed());
                CPPUNIT_ASSERT(last->id() == child1.id());
            }

            {
                CPPUNIT_ASSERT_EQUAL(5, child_t::objectNumber());
                parent.push_front(child3);
                CPPUNIT_ASSERT_EQUAL(6, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 3);


                child_t* first  = static_cast<child_t*>(parent.first());
                child_t* second = static_cast<child_t*>(first->next());
                child_t* last   = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == second);

                CPPUNIT_ASSERT(first->copyConstructed());
                CPPUNIT_ASSERT(first->id() == child3.id());


                CPPUNIT_ASSERT(second->parent()   == &parent);
                CPPUNIT_ASSERT(second->previous() == first);
                CPPUNIT_ASSERT(second->next()     == last);

                CPPUNIT_ASSERT(second->copyConstructed());
                CPPUNIT_ASSERT(second->id() == child2.id());


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == second);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(last->copyConstructed());
                CPPUNIT_ASSERT(last->id() == child1.id());
            }
        }
        CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
    }

    void test_push_front_move()
    {
        {
            parent_t parent;
            child_t child1;
            child_t child2;
            child_t child3;

            int id1 = child1.id();
            int id2 = child2.id();
            int id3 = child3.id();

            {
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                parent.push_front(std::move(child1));
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 1);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first             == last);
                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == nullptr);

                CPPUNIT_ASSERT(first->moveConstructed());
                CPPUNIT_ASSERT(first->id() == id1);
            }

            {
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                parent.push_front(std::move(child2));
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 2);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == last);

                CPPUNIT_ASSERT(first->moveConstructed());
                CPPUNIT_ASSERT(first->id() == id2);


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == first);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(last->moveConstructed());
                CPPUNIT_ASSERT(last->id() == id1);
            }

            {
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                parent.push_front(std::move(child3));
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 3);


                child_t* first  = static_cast<child_t*>(parent.first());
                child_t* second = static_cast<child_t*>(first->next());
                child_t* last   = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == second);

                CPPUNIT_ASSERT(first->moveConstructed());
                CPPUNIT_ASSERT(first->id() == id3);


                CPPUNIT_ASSERT(second->parent()   == &parent);
                CPPUNIT_ASSERT(second->previous() == first);
                CPPUNIT_ASSERT(second->next()     == last);

                CPPUNIT_ASSERT(second->moveConstructed());
                CPPUNIT_ASSERT(second->id() == id2);


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == second);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(last->moveConstructed());
                CPPUNIT_ASSERT(last->id() == id1);
            }
        }
        CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
    }

    void test_push_back_single_element()
    {
        {
            parent_t parent;
            child_t child1;
            child_t child2;
            child_t child3;

            {
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                parent.push_back(child1);
                CPPUNIT_ASSERT_EQUAL(4, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 1);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first             == last);
                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == nullptr);

                CPPUNIT_ASSERT(first->copyConstructed());
                CPPUNIT_ASSERT(first->id() == child1.id());
            }

            {
                CPPUNIT_ASSERT_EQUAL(4, child_t::objectNumber());
                parent.push_back(child2);
                CPPUNIT_ASSERT_EQUAL(5, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 2);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == last);

                CPPUNIT_ASSERT(first->copyConstructed());
                CPPUNIT_ASSERT(first->id() == child1.id());


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == first);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(last->copyConstructed());
                CPPUNIT_ASSERT(last->id() == child2.id());
            }

            {
                CPPUNIT_ASSERT_EQUAL(5, child_t::objectNumber());
                parent.push_back(child3);
                CPPUNIT_ASSERT_EQUAL(6, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 3);


                child_t* first  = static_cast<child_t*>(parent.first());
                child_t* second = static_cast<child_t*>(first->next());
                child_t* last   = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == second);

                CPPUNIT_ASSERT(first->copyConstructed());
                CPPUNIT_ASSERT(first->id() == child1.id());


                CPPUNIT_ASSERT(second->parent()   == &parent);
                CPPUNIT_ASSERT(second->previous() == first);
                CPPUNIT_ASSERT(second->next()     == last);

                CPPUNIT_ASSERT(second->copyConstructed());
                CPPUNIT_ASSERT(second->id() == child2.id());


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == second);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(last->copyConstructed());
                CPPUNIT_ASSERT(last->id() == child3.id());
            }
        }
        CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
    }

    void test_push_back_move()
    {
        {
            parent_t parent;
            child_t child1;
            child_t child2;
            child_t child3;

            int id1 = child1.id();
            int id2 = child2.id();
            int id3 = child3.id();

            {
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                parent.push_front(std::move(child1));
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 1);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first             == last);
                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == nullptr);

                CPPUNIT_ASSERT(first->moveConstructed());
                CPPUNIT_ASSERT(first->id() == id1);
            }

            {
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                parent.push_front(std::move(child2));
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 2);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == last);

                CPPUNIT_ASSERT(first->moveConstructed());
                CPPUNIT_ASSERT(first->id() == id2);


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == first);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(last->moveConstructed());
                CPPUNIT_ASSERT(last->id() == id1);
            }

            {
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                parent.push_front(std::move(child3));
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 3);


                child_t* first  = static_cast<child_t*>(parent.first());
                child_t* second = static_cast<child_t*>(first->next());
                child_t* last   = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == second);

                CPPUNIT_ASSERT(first->moveConstructed());
                CPPUNIT_ASSERT(first->id() == id3);


                CPPUNIT_ASSERT(second->parent()   == &parent);
                CPPUNIT_ASSERT(second->previous() == first);
                CPPUNIT_ASSERT(second->next()     == last);

                CPPUNIT_ASSERT(second->moveConstructed());
                CPPUNIT_ASSERT(second->id() == id2);


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == second);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(last->moveConstructed());
                CPPUNIT_ASSERT(last->id() == id1);
            }
        }
        CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
    }

    void test_emplace()
    {
        {
            parent_t parent;

            {
                CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
                parent.template emplace<child_t>(parent.begin(), (parent_t*)0xdeadbeef);
                CPPUNIT_ASSERT_EQUAL(1, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 1);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first             == last);
                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == nullptr);

                CPPUNIT_ASSERT(!first->copyConstructed());
                CPPUNIT_ASSERT(!first->moveConstructed());
            }

            {
                CPPUNIT_ASSERT_EQUAL(1, child_t::objectNumber());
                parent.template emplace<child_t>(parent.end(), (parent_t*)0xdeadbeef);
                CPPUNIT_ASSERT_EQUAL(2, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 2);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == last);

                CPPUNIT_ASSERT(!first->copyConstructed());
                CPPUNIT_ASSERT(!first->moveConstructed());


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == first);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(!last->copyConstructed());
                CPPUNIT_ASSERT(!last->moveConstructed());
            }

            {
                CPPUNIT_ASSERT_EQUAL(2, child_t::objectNumber());
                parent.template emplace<child_t>(++(parent.begin()), (parent_t*)0xdeadbeef);
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 3);


                child_t* first  = static_cast<child_t*>(parent.first());
                child_t* second = static_cast<child_t*>(first->next());
                child_t* last   = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == second);

                CPPUNIT_ASSERT(!first->copyConstructed());
                CPPUNIT_ASSERT(!first->moveConstructed());


                CPPUNIT_ASSERT(second->parent()   == &parent);
                CPPUNIT_ASSERT(second->previous() == first);
                CPPUNIT_ASSERT(second->next()     == last);

                CPPUNIT_ASSERT(!second->copyConstructed());
                CPPUNIT_ASSERT(!second->moveConstructed());


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == second);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(!last->copyConstructed());
                CPPUNIT_ASSERT(!last->moveConstructed());
            }
        }
        CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
    }

    void test_emplace_front()
    {
        {
            parent_t parent;

            {
                CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
                parent.template emplace_front<child_t>((parent_t*)0xdeadbeef);
                CPPUNIT_ASSERT_EQUAL(1, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 1);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first             == last);
                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == nullptr);

                CPPUNIT_ASSERT(!first->copyConstructed());
                CPPUNIT_ASSERT(!first->moveConstructed());
            }

            {
                CPPUNIT_ASSERT_EQUAL(1, child_t::objectNumber());
                parent.template emplace_front<child_t>((parent_t*)0xdeadbeef);
                CPPUNIT_ASSERT_EQUAL(2, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 2);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == last);

                CPPUNIT_ASSERT(!first->copyConstructed());
                CPPUNIT_ASSERT(!first->moveConstructed());


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == first);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(!last->copyConstructed());
                CPPUNIT_ASSERT(!last->moveConstructed());
            }

            {
                CPPUNIT_ASSERT_EQUAL(2, child_t::objectNumber());
                parent.template emplace_front<child_t>((parent_t*)0xdeadbeef);
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 3);


                child_t* first  = static_cast<child_t*>(parent.first());
                child_t* second = static_cast<child_t*>(first->next());
                child_t* last   = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == second);

                CPPUNIT_ASSERT(!first->copyConstructed());
                CPPUNIT_ASSERT(!first->moveConstructed());


                CPPUNIT_ASSERT(second->parent()   == &parent);
                CPPUNIT_ASSERT(second->previous() == first);
                CPPUNIT_ASSERT(second->next()     == last);

                CPPUNIT_ASSERT(!second->copyConstructed());
                CPPUNIT_ASSERT(!second->moveConstructed());


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == second);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(!last->copyConstructed());
                CPPUNIT_ASSERT(!last->moveConstructed());
            }
        }
        CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
    }

    void test_emplace_back()
    {
        {
            parent_t parent;

            {
                CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
                parent.template emplace_back<child_t>((parent_t*)0xdeadbeef);
                CPPUNIT_ASSERT_EQUAL(1, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 1);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first             == last);
                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == nullptr);

                CPPUNIT_ASSERT(!first->copyConstructed());
                CPPUNIT_ASSERT(!first->moveConstructed());
            }

            {
                CPPUNIT_ASSERT_EQUAL(1, child_t::objectNumber());
                parent.template emplace_back<child_t>((parent_t*)0xdeadbeef);
                CPPUNIT_ASSERT_EQUAL(2, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 2);


                child_t* first = static_cast<child_t*>(parent.first());
                child_t* last  = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == last);

                CPPUNIT_ASSERT(!first->copyConstructed());
                CPPUNIT_ASSERT(!first->moveConstructed());


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == first);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(!last->copyConstructed());
                CPPUNIT_ASSERT(!last->moveConstructed());
            }

            {
                CPPUNIT_ASSERT_EQUAL(2, child_t::objectNumber());
                parent.template emplace_back<child_t>((parent_t*)0xdeadbeef);
                CPPUNIT_ASSERT_EQUAL(3, child_t::objectNumber());
                CPPUNIT_ASSERT(parent.size() == 3);


                child_t* first  = static_cast<child_t*>(parent.first());
                child_t* second = static_cast<child_t*>(first->next());
                child_t* last   = static_cast<child_t*>(parent.last());


                CPPUNIT_ASSERT(first->parent()   == &parent);
                CPPUNIT_ASSERT(first->previous() == nullptr);
                CPPUNIT_ASSERT(first->next()     == second);

                CPPUNIT_ASSERT(!first->copyConstructed());
                CPPUNIT_ASSERT(!first->moveConstructed());


                CPPUNIT_ASSERT(second->parent()   == &parent);
                CPPUNIT_ASSERT(second->previous() == first);
                CPPUNIT_ASSERT(second->next()     == last);

                CPPUNIT_ASSERT(!second->copyConstructed());
                CPPUNIT_ASSERT(!second->moveConstructed());


                CPPUNIT_ASSERT(last->parent()   == &parent);
                CPPUNIT_ASSERT(last->previous() == second);
                CPPUNIT_ASSERT(last->next()     == nullptr);

                CPPUNIT_ASSERT(!last->copyConstructed());
                CPPUNIT_ASSERT(!last->moveConstructed());
            }
        }
        CPPUNIT_ASSERT_EQUAL(0, child_t::objectNumber());
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_parent_node<char>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_parent_node<char16_t>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_parent_node<char32_t>);
CPPUNIT_TEST_SUITE_REGISTRATION(test_parent_node<wchar_t>);
