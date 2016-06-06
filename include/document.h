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
        typedef          basic_parent_node<charT>             parent_type;                  //!< The parent node type.
        typedef typename parent_type::child_type              child_type;                   //!< The child node type.
        typedef typename parent_type::size_type               size_type;                    //!< The size type.

        typedef          basic_element<charT>                 root_type;                    //!< The root node type.
        typedef          std::basic_string<charT>             string_type;                  //!< The string type.

        //!@}

        virtual std::basic_string<charT> type() const;

        //! \brief The version of a XML document
        /*!
         *  It stores the version number of a XML document.
         *  The version number is in format "<major>.<minor>".
         */
        class version_t {
        public:
            uint8_t major; //!< The major version number.
            uint8_t minor; //!< The minor version number.

            string_type toString() const;      //!< Converts the version to a string formatted as "<major>.<minor>".
            string_type majorToString() const; //!< Converts the major version number to a string.
            string_type minorToString() const; //!< Converts the minor version number to a string.
        } version; //!< The XML version of this document.

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

            string_type toString() const; //!< Converts the document encoding to a string.
        } encoding; //!< The encoding version of this document.

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

            string_type toString() const; //!< Converts the document standalone value to a string.
        } standalone; //!< Whether this XML document is a standalone.

    private:
        root_type*    mRoot;       //!< A pointer to the root element of this document.
    };

    typedef basic_document<char>    document;  //!< A specialized \c basic_document for char.
    typedef basic_document<wchar_t> wdocument; //!< A specialized \c basic_document for wchar_t.
}

#endif /* DOCUMENT_H_INCLUDED */