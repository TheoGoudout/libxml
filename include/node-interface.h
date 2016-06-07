#ifndef NODE_INTERFACE_H_INCLUDED
#define NODE_INTERFACE_H_INCLUDED

#include <string>

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

        //!@}

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
        static type_t stringToType(const std::string& str)
        {
            return type_t(str.begin(), str.end());
        }
    };

    typedef basic_node_interface<char>    node_interface;  //!< A specialized \c basic_node_interface for char.
    typedef basic_node_interface<wchar_t> wnode_interface; //!< A specialized \c basic_node_interface for wchar_t.
}

#endif /* NODE_INTERFACE_H_INCLUDED */