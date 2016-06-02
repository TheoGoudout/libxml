#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <parent-node.h>
#include <child-node.h>

namespace xml {
    //! \brief A XML node.
    /*!
     *  This class represents a XML node that can have both a parent and
     *  children.
     *
     *  \tparam charT The type of character used in the XML node.
     *                By default, char and wchar_t are supported.
     */
    template <typename charT>
    class basic_node : public basic_parent_node<charT>, public basic_child_node<charT> {

    };

    typedef basic_node<char>    node;  //!< A specialized \c basic_node for char.
    typedef basic_node<wchar_t> wnode; //!< A specialized \c basic_node for wchar_t.
}

#endif /* NODE_H_INCLUDED */