#include <cppunit/TestCase.h>

#include <tuple>

namespace {
    template <size_t len>
    struct tuple_action_applier : tuple_action_applier<len - 1>
    {
        template <typename Fixture, typename ... ArgsT, typename ... AccT>
        static inline void apply(Fixture* that, void (Fixture::* func)(ArgsT ...), std::tuple<ArgsT ...> tuple, AccT ... args)
        {
            tuple_action_applier<len - 1>::apply(that, func, tuple, std::get<len - 1>(tuple), args ...);
        }
    };

    template <>
    struct tuple_action_applier<0>
    {
        template <typename Fixture, typename ... ArgsT, typename ... AccT>
        static inline void apply(Fixture* that, void (Fixture::* func)(ArgsT ...), std::tuple<ArgsT ...> tuple, AccT ... args)
        {
            (that->*func)(args ...);
        }
    };

    template <typename Fixture, typename ... ArgsT>
    inline void tuple_action(Fixture* that, void (Fixture::* func)(ArgsT ...), std::tuple<ArgsT ...> tuple)
    {
        tuple_action_applier<std::tuple_size<decltype(tuple)>::value>::apply(that, func, tuple);
    }



    template <size_t len>
    struct tuple_func_applier : tuple_func_applier<len - 1>
    {
        template <typename retT, typename ... ArgsT, typename ... AccT>
        static inline retT apply(retT (*func)(ArgsT ...), std::tuple<ArgsT ...> tuple, AccT ... args)
        {
            return tuple_func_applier<len - 1>::apply(func, tuple, std::get<len - 1>(tuple), args ...);
        }
    };

    template <>
    struct tuple_func_applier<0>
    {
        template <typename retT, typename ... ArgsT, typename ... AccT>
        static inline retT apply(retT (*func)(ArgsT ...), std::tuple<ArgsT ...> tuple, AccT ... args)
        {
            return func(args ...);
        }
    };

    template <typename retT, typename ... ArgsT>
    inline retT tuple_func(retT (*func)(ArgsT ...), std::tuple<ArgsT ...> tuple)
    {
        return tuple_func_applier<std::tuple_size<decltype(tuple)>::value>::apply(func, tuple);
    }



    template <typename ArgT>
    std::string to_string(ArgT arg)
    {
        std::stringstream sstr;
        sstr << "\"" << arg << "\"";
        return sstr.str();
    }

    template <typename ArgT, typename ... ArgsT>
    std::string to_string(ArgT arg, ArgsT ... args)
    {
        std::stringstream sstr;
        sstr << "\"" << arg << "\", " << to_string(args ...);
        return sstr.str();
    }
}

namespace CPPUNIT_NS
{
    template <typename Fixture, typename ... Args>
    class TestCallerWithParameter : public TestCase
    {
        typedef void (Fixture::*TestMethod)(Args ...);
        typedef std::tuple<Args ...> TestParameter;
     
    public:
        TestCallerWithParameter( std::string name, TestMethod test, TestParameter param ) :
                 TestCase( name + " (" + tuple_func<std::string, Args ...>(to_string, param) + ")" ), 
                 m_ownFixture( true ),
                 m_fixture( new Fixture() ),
                 m_test( test ),
                 m_parameter( param )
        {
        }

        TestCallerWithParameter(std::string name, TestMethod test, TestParameter param, Fixture& fixture) :
                 TestCase( name + " (" + tuple_func<std::string, Args ...>(to_string, param) + ")" ), 
                 m_ownFixture( false ),
                 m_fixture( &fixture ),
                 m_test( test ),
                 m_parameter( param )
        {
        }
     
        TestCallerWithParameter(std::string name, TestMethod test, TestParameter param, Fixture* fixture) :
                 TestCase( name + " (" + tuple_func<std::string, Args ...>(to_string, param) + ")" ), 
                 m_ownFixture( true ),
                 m_fixture( fixture ),
                 m_test( test ),
                 m_parameter( param )
        {
        }
     
        ~TestCallerWithParameter() 
        {
            if (m_ownFixture)
                delete m_fixture;
        }

        void runTest()
        {
            tuple_action(m_fixture, m_test, m_parameter);
        }  

        void setUp()
        { 
             m_fixture->setUp (); 
        }

        void tearDown()
        { 
               m_fixture->tearDown (); 
        }

        std::string toString() const
        { 
             return "TestCallerWithParameter " + getName(); 
        }

    private: 
        TestCallerWithParameter( const TestCallerWithParameter &other ); 
        TestCallerWithParameter &operator =( const TestCallerWithParameter &other );

    private:
        bool m_ownFixture;
        Fixture *m_fixture;
        TestMethod m_test;
        TestParameter m_parameter;
    };
}
