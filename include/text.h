#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include <string>
#include <istream>

#include <child-node.h>

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
        typedef std::basic_string<charT> string_type; //!< The type of string stored.

        //!@}

        virtual std::basic_string<charT> type() const;

        //! \brief Get text content.
        /*!
         *  \return A constant reference to text content.
         */
        const string_type& data() const;

        //! \brief Set text content.
        /*!
         *  The text content to set must respect rules described in :
         *  https://www.w3.org/TR/xml/#syntax
         *  If these rules are not respected, an exception will be thrown.
         *
         *  \param [in] data The text content to set.
         *
         *  \return A reference to the current \c basic_text.
         *
         *  \throw xml::parsing_exception An error occured whilst parsing data.
         */
        basic_text<charT>& set_data(const string_type& data);

    private:
        string_type mData; //!< The content of a \c basic_text object.
    };

    typedef basic_text<char>    text;  //!< A specialized \c basic_text for char.
    typedef basic_text<wchar_t> wtext; //!< A specialized \c basic_text for wchar_t.
}

#endif /* TEXT_H_INCLUDED */