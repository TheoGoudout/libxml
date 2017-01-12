#ifndef NODE_INTERFACE_H_INCLUDED
#define NODE_INTERFACE_H_INCLUDED

#include <string>

#include <readable.h>
#include <writeable.h>

namespace xml {
    //! \brief An interface defining available properties for any node.
    /*!
     *  This interface will define a set of pure virtual function
     *  that every inheriting class should have.
     *
     *  \tparam charT The type of character used in the XML node.
     *                By default, char and wchar_t are supported.
     */
    template <typename charT>
    class basic_node_interface {
    public:
        //! \name Member types
        //!@{
        typedef std::basic_string<charT> type_t; //!< The type of a node type.

        typedef basic_node_interface<charT> node_interface_t; //!< The type of node interface this node is.

        //!@}

        //! \brief Default constructor
        /*!
         *  This constructor does nothing.
         */
        basic_node_interface()
        {}

        //! \brief Copy constructor
        /*!
         *  This constructor does nothing.
         *
         *  \param [in] rhs A constant reference to a \c node_interface_t.
         */
        basic_node_interface(const node_interface_t& rhs)
        {}

        //! \brief Move constructor
        /*!
         *  This constructor does nothing.
         *
         *  \param [in] rhs A rvalue reference to a \c node_interface_t.
         */
        basic_node_interface(node_interface_t&& rhs)
        {}

        //! \brief Default destructor
        /*!
         *  This destructor does nothing.
         */
        virtual ~basic_node_interface()
        {}

        //! \brief Returns the type of a node.
        /*!
         *  This functions will return the type of a node.
         *  It is a pure virtual function that should be overridden
         *  by inheriting classes.
         *
         *  \return A string representing the type of a node.
         */
        virtual type_t type() const = 0;

    protected:

        //! \brief Convert a standard string to a node type.
        /*!
         *  This function will convert a standard string to a node type.
         *  At the moment, a node type is a \c basic_string, but this could
         *  change in the future. Don't use this function to convert string
         *  from one type of char to another.
         *
         *  \param[in] str The standard string to convert to a node type.
         *
         *  \return A node type representing the given string.
         */
        static type_t stringToType(const std::string& str)
        {
            return type_t(str.begin(), str.end());
        }
    };

    typedef basic_node_interface<char>    node_interface;  //!< A specialized \c basic_node_interface for char.
    typedef basic_node_interface<wchar_t> wnode_interface; //!< A specialized \c basic_node_interface for wchar_t.
}

#endif /* NODE_INTERFACE_H_INCLUDED */