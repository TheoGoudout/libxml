#ifndef ATTRIBUTE_H_INCLUDED
#define ATTRIBUTE_H_INCLUDED

#include <string>
#include <istream>
#include <sstream>

#include <readable.h>
#include <writeable.h>

namespace xml {

    //! \brief An XML attribute.
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
        typedef          basic_readable<charT>  readable_t;
        typedef typename readable_t::string_t   string_t;
        typedef typename readable_t::istream_t  istream_t;

        typedef          std::basic_istringstream<charT> istringstream_t;

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
            const string_t& name,
            const string_t& value)
        {
            setName(name);
            setValue(value);
        }


        basic_attribute(readable_t& input)
        {
            input.read_name_and_quoted_value(
                &readable_t::read_name,            mName,
                &readable_t::read_attribute_value, mValue);
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
        const string_t& name() const
        {
            return mName;
        }

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
        const string_t& value() const
        {
            return mValue;
        }

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
        basic_attribute<charT>& setName(const string_t& name)
        {
            istringstream_t sstr(name);
            readable_t input(sstr);
            input.read_name(mName);
            input.read_eof();

            return *this;
        }

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
        basic_attribute<charT>& setValue(const string_t& value)
        {
            istringstream_t sstr(value);
            readable_t input(sstr);
            input.read_attribute_value('"', mValue);
            input.read_eof();

            return *this;
        }

    private:
        string_t mName;  //!< The name of an attribute.
        string_t mValue; //!< The value of an attribute.
    };
}

#endif /* ATTRIBUTE_H_INCLUDED */