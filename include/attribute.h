#ifndef ATTRIBUTE_H_INCLUDED
#define ATTRIBUTE_H_INCLUDED

#include <string>
#include <istream>
#include <sstream>

namespace xml {

    //! \brief A XML attribute.
    /*!
     *  This class represents an XML attribute that has a name and a value.
     *
     *  \tparam charT The type of character used in the name and value.
     *                By default, char and wchar_t are supported.
     */
    template <typename charT>
    class basic_attribute {
    public:
        //! \name Member types
        //!@{
        typedef std::basic_string<charT>  name_type;    //!< The type of an attribute name
        typedef std::basic_string<charT>  value_type;   //!< The type of an attribute value
        typedef std::basic_istream<charT> istream_type; //!< The type of stream to parse
        typedef std::basic_string<charT>  string_type;  //!< The type of string to parse

        //!@}

        //! \brief Public constructor for attribute.
        /*!
         *  This constructor build a new \c basic_attribute object,
         *  with a name and a value. \c setName and \c setValue will
         *  be called to initialize the object.
         *
         *  \param [in] name  The name of the attribute.
         *  \param [in] value The value of the attribute.
         *
         *  \sa setName
         *  \sa setValue
         */
        basic_attribute(
            const name_type& name,
            const value_type& value)
        {
            setName(name);
            setValue(value);
        }


        //! \brief Get the name of an attribute.
        /*!
         *  This function returns a constant reference to the name of the
         *  \c basic_attribute.
         *
         *  \return A reference to the name of the \c basic_attribute.
         *
         *  \sa setName
         */
        const name_type& name() const;

        //! \brief Get the value of an attribute.
        /*!
         *  This function returns a constant reference to the value of the
         *  \c basic_attribute.
         *  The returned value is a XML formatted string.
         *
         *  \return A reference to the value of the \c basic_attribute.
         *
         *  \sa setValue
         */
        const value_type& value() const;

        //! \brief Set the name of an attribute.
        /*!
         *  This function sets the name of the \c basic_attribute.
         *
         *  \param [in] name A reference to a XML formatted string.
         *
         *  \return The current \c basic_attribute.
         *
         *  \sa name
         */
        basic_attribute<charT>& setName(const string_type& name);

        //! \brief Set the value of an attribute.
        /*!
         *  This function sets the value of the \c basic_attribute.
         *
         *  \param [in] value A reference to a XML formatted string.
         *
         *  \return The current \c basic_attribute.
         *
         *  \sa value
         */
        basic_attribute<charT>& setValue(const string_type& value);

    private:
        name_type  mName;  //!< The name of an attribute.
        value_type mValue; //!< The value of an attribute.
    };

    typedef basic_attribute<char>    attribute;  //!< A specialized \c basic_attribute for char.
    typedef basic_attribute<wchar_t> wattribute; //!< A specialized \c basic_attribute for wchar_t.
}

#endif /* ATTRIBUTE_H_INCLUDED */