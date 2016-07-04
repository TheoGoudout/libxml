#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include <string>
#include <istream>

#include <parent-node.h>
#include <child-node.h>
#include <readable.h>

namespace xml {
    //! \brief A XML text node.
    /*!
     *  This class represents a XML text node.
     *
     *  \tparam charT The type of character used in the XML node.
     *                By default, char and wchar_t are supported.
     */
    template <typename charT>
    class basic_text : public basic_child_node<charT> {
    public:
        //! \name Member types
        //!@{
        typedef          basic_node_interface<charT> node_interface_t; //!< The base type of this node.
        typedef typename node_interface_t::type_t    type_t;           //!< The type of a node type.

        typedef          basic_parent_node<charT>   parent_t;         //!< The parent type.
        typedef typename parent_t::parent_pointer_t parent_pointer_t; //!< Pointer to \c parent_t.

        typedef          basic_child_node<charT>  child_t;         //!< The child type.
        typedef typename child_t::child_pointer_t child_pointer_t; //!< Pointer to \c child_t.
        typedef typename child_t::child_move_t    child_move_t;    //!< Move a \c child_t.

        typedef basic_text<charT> text_t;                 //!< The type of text this node is.
        typedef text_t*           text_pointer_t;         //!< Pointer to \c text_t.
        typedef text_t&           text_reference_t;       //!< Reference to \c text_t.
        typedef const text_t&     text_const_reference_t; //!< Constant reference to \c text_t.
        typedef text_t&&          text_move_t;            //!< Move a \c text_t.

        typedef std::basic_string<charT> string_t; //!< The type of string stored.

        typedef          basic_readable<charT>            readable_t;           //!< The type of readable.
        typedef typename readable_t::readable_reference_t readable_reference_t; //!< Pointer to \c child_t.

        //!@}

        //! \brief Constructor.
        /*!
         *  Constructs an XML text object. It contains some data and possibly
         *  has a parent.
         *
         *  \param [in] data   The text data of this \c text_t.
         *  \param [in] parent The parent node of this \c text_t.
         */
        basic_text(
            string_t data,
            parent_pointer_t parent = nullptr)
        :
            child_t(parent),
            mData(data)
        {}

        //! \brief Copy constructor.
        /*!
         *  Creates a copy of an XML text object.
         *
         *  \param [in] rhs A constant reference to a \c text_t.
         */
        basic_text(text_const_reference_t rhs)
        :
            child_t(nullptr),
            mData(rhs.mData)
        {}

        //! \brief Move constructor.
        /*!
         *  Moves the internal of a \c text_t.
         *
         *  \param [in] rhs A rvalue reference to a \c text_t.
         */
        basic_text(text_move_t rhs)
        :
            child_t(rhs),
            mData(std::move(rhs.mData))
        {}

        //! \brief Parsing constructor.
        /*!
         *  This constructor parses the internals of a text_t.
         *
         *  \param [in] input The input stream parser used to get text internals.
         */
        basic_text(readable_reference_t input)
        {
            if (!parse(input))
                throw -1; // TODO : throw parsing exception
        }

        //! \brief Destructor.
        /*!
         *  This destructor does nothing.
         */
        virtual ~basic_text()
        {}

        //! \brief Parse text internals.
        /*!
         *  This function parses the internals of a text_t.
         *
         *  \param [in] input The input stream parser used to get text internals.
         */
        bool parse(readable_reference_t input)
        {
            return input.read_character_data(mData);
        }

        //! \brief Get the type of a \c text_t.
        /*!
         *  This function returns the type of a \c text_t.
         *
         *  \return The type of a \c text_t
         */
        virtual type_t type() const
        {
            return node_interface_t::stringToType("text");
        }

        //! \brief Clone the current \c basic_child_node.
        /*!
         *  This function creates a deep copy of this \c basic_child_node,
         *  and returns a pointer to it. It is used when copying element in
         *  parent node.
         */
        virtual child_pointer_t clone() const
        {
            return new text_t(*this);
        }

        //! \brief Clone the given \c basic_child_node using move syntax.
        /*!
         *  This function creates a deep copy of the given\c basic_child_node,
         *  and returns a pointer to it. It is used when copying element in
         *  parent node.
         */
        virtual child_pointer_t clone(child_move_t rhs) const
        {
            return new text_t(static_cast<text_move_t>(rhs));
        }

        //! \brief Get text content.
        /*!
         *  \return A constant reference to text content.
         */
        const string_t& data() const
        {
            return mData;
        }

        //! \brief Get text content.
        /*!
         *  \return A reference to text content.
         */
        string_t& data()
        {
            return mData;
        }

    private:
        string_t mData; //!< The content of a \c basic_text object.
    };

    typedef basic_text<char>    text;  //!< A specialized \c basic_text for char.
    typedef basic_text<wchar_t> wtext; //!< A specialized \c basic_text for wchar_t.
}

#endif /* TEXT_H_INCLUDED */