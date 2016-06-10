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
    public:
        //! \name Member types
        //!@{
        typedef basic_parent_node<charT>  parent_t;           //!< The parent node type.
        typedef parent_t*                 parent_pointer_t;   //!< A pointer to the parent type.

        typedef basic_child_node<charT>   child_t;           //!< The type of children this node is.

        //!@}

        //! \brief Default constructor
        /*!
         *  This constructor initialise the internals of a child node and the
         *  internals of a parent node.
         *
         *  \param[in] parent The parent node of this one.
         */
        basic_node(parent_pointer_t parent = nullptr)
        :
            parent_t(),
            child_t(parent)
        {}

        //! \brief Default destructor
        /*!
         *  This destructor does nothing.
         */
        virtual ~basic_node()
        {}

    };

    typedef basic_node<char>    node;  //!< A specialized \c basic_node for char.
    typedef basic_node<wchar_t> wnode; //!< A specialized \c basic_node for wchar_t.
}

#endif /* NODE_H_INCLUDED */