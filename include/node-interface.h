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
        //! \brief Returns the type of a node.
        /*!
         *  This functions will return the type of a node.
         *  It is a pure virtual function that should be overridden
         *  by inheriting classes.
         *
         *  \return A string representing the type of a node.
         */
        virtual std::basic_string<charT> type() const = 0;
    };
}

#endif /* NODE_INTERFACE_H_INCLUDED */