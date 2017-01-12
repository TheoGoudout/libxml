#ifndef DOCUMENT_H_INCLUDED
#define DOCUMENT_H_INCLUDED

#include <string>

#include <parent-node.h>
#include <element.h>

namespace xml {
    //! \brief A XML document.
    /*!
     *  This class represents a XML document. It can have a version,
     *  encoding and a standalone status. It has a mandatory root element.
     *
     *  \sa xml::basic_parent_node
     *
     *  \tparam charT The type of character used in the XML node.
     *                By default, char and wchar_t are supported.
     */
    template <typename charT>
    class basic_document : public basic_parent_node<charT> {
    public:
        //! \name Member types
        //!@{
        typedef          basic_node_interface<charT> node_interface_t; //!< The base type of this node.
        typedef typename node_interface_t::type_t    type_t;           //!< The type of a node type.

        typedef          basic_parent_node<charT> parent_t; //!< The parent node type.
        typedef typename parent_t::child_t        child_t;  //!< The child node type.

        typedef basic_element<charT> root_t; //!< The root node type.

        typedef basic_document<charT> document_t; //!< The type of document this node is.

        typedef std::basic_string<charT> string_t; //!< The string type.

        //!@}

        //! \brief The version of a XML document
        /*!
         *  It stores the version number of a XML document.
         *  The version number is in format "<major>.<minor>".
         */
        class version_t {
        public:
            uint8_t major; //!< The major version number.
            uint8_t minor; //!< The minor version number.
        };

        //! \brief The encoding of a XML document
        /*!
         *  It stores the encoding name of a XML document.
         */
        class encoding_t {
        public:
            //! The available XML encoding of a document. Might be undefined.
            enum {
                UTF8,
                undefined
            } value;
        };

        //! \brief Whether a XML document is standalone
        /*!
         *  It represents a XML document standalone status.
         *  It can be yes, no or undefined.
         */
        class standalone_t {
        public:
            //! Whether the XML document is a standalone. Might be undefined.
            enum {
                yes,
                no,
                undefined
            } value;
        };

        //! \brief Constructor.
        /*!
         *  This constructor initialise the internals of an document, and
         *  inserts a root element with name \c root_name.
         *
         *  \param[in] root_name The name of the root element.
         */
        basic_document(const string_t& root_name)
        :
            parent_t(),
            mVersion(),
            mEncoding(),
            mStandalone(),
            mRoot(nullptr)
        {
            parent_t::template emplace_front<root_t>(root_name);

            mRoot = &(*parent_t::template begin<root_t>());
        }

        //! \brief Constructor.
        /*!
         *  This constructor initialise the internals of an document, and
         *  inserts a root element.
         *
         *  \param[in] root The root element.
         */
        basic_document(const root_t& root)
        :
            parent_t(),
            mVersion(),
            mEncoding(),
            mStandalone(),
            mRoot(nullptr)
        {
            parent_t::push_front(root);

            mRoot = &(*parent_t::template begin<root_t>());
        }

        //! \brief Constructor.
        /*!
         *  This constructor initialise the internals of an document, and moves
         *  a root element.
         *
         *  \param[in] root The root element.
         */
        basic_document(root_t&& root)
        :
            parent_t(),
            mVersion(),
            mEncoding(),
            mStandalone(),
            mRoot(nullptr)
        {
            parent_t::push_front(std::move(root));

            mRoot = &(*parent_t::template begin<root_t>());
        }

        //! \brief Copy constructor.
        /*!
         *  Creates a copy of an XML document.
         *
         *  \param [in] rhs A constant reference to a \c document_t.
         */
        basic_document(const document_t& rhs)
        :
            parent_t(rhs),
            mVersion(rhs.mVersion),
            mEncoding(rhs.mEncoding),
            mStandalone(rhs.mStandalone),
            mRoot(nullptr)
        {
            mRoot = &(*parent_t::template begin<root_t>());
        }

        //! \brief Move constructor.
        /*!
         *  Moves the internal of a \c element_t.
         *
         *  \param [in] rhs A rvalue reference to a \c element_t.
         */
        basic_document(document_t&& rhs)
        :
            parent_t(std::move(rhs)),
            mVersion(rhs.mVersion),
            mEncoding(rhs.mEncoding),
            mStandalone(rhs.mStandalone),
            mRoot(rhs.mRoot)
        {}

        //! \brief Destructor.
        /*!
         *  This destructor does nothing.
         */
        virtual ~basic_document()
        {}

        //! \brief Get the type of a \c document_t.
        /*!
         *  This function returns the type of a \c document_t.
         *
         *  \return The type of a \c document_t.
         */
        virtual type_t type() const
        {
            return node_interface_t::stringToType("document");
        }

        //! \brief Get a constant reference to the root element of this XML document.
        /*!
         *  This function returns a constant reference to the root element of
         *  the current XML document.
         *
         *  \return A constant reference to the root element of this XML document.
         */
        const root_t& root() const { return *mRoot; }

        //! \brief Get a reference to the root element of this XML document.
        /*!
         *  This function returns a reference to the root element of the current
         *  XML document.
         *
         *  \return A reference to the root element of this XML document.
         */
        root_t& root() { return *mRoot; }

    private:
        version_t    mVersion;    //!< The XML version of this document.
        encoding_t   mEncoding;   //!< The encoding version of this document.
        standalone_t mStandalone; //!< Whether this XML document is a standalone.

        root_t* mRoot; //!< A pointer to the root element of this document.
    };

    typedef basic_document<char>    document;  //!< A specialized \c basic_document for char.
    typedef basic_document<wchar_t> wdocument; //!< A specialized \c basic_document for wchar_t.
}

#endif /* DOCUMENT_H_INCLUDED */