#ifndef ATTRIBUTE_H_INCLUDED
#define ATTRIBUTE_H_INCLUDED

#include <string>
#include <istream>
#include <sstream>

#include <parser.h>

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
         *  This functions returns a const reference to the name of the
         *  \c basic_attribute. In order to modify the name, use \c setName.
         *
         *  \return A reference to the name of the \c basic_attribute.
         *
         *  \sa setName
         */
        const name_type& name() const { return mName; }

        //! \brief Get the value of an attribute.
        const value_type& value() const { return mValue; }

        //! \brief Set the name of an attribute.
        basic_attribute<charT>& setName(const string_type& name) {
            mName = parseName(name);
            return *this;
        }

        //! \brief Set the value of an attribute.
        basic_attribute<charT>& setValue(const string_type& value) {
            mValue = parseValue(value);
            return *this;
        }

    private:
        basic_attribute() {}

        static basic_attribute<charT> parse(const string_type& str) {
            std::basic_istringstream<charT> sin(str);

            return parse(sin);
        }


        static basic_attribute<charT> parse(istream_type& sin) {
            basic_parser<charT> parser(sin);
            basic_attribute<charT> attr;

            if (!parser.read_attribute_name_value(attr.mName, attr.mValue)) {
                // TODO: error
                throw -1;
            }

            if (!parser.read_eof()) {
                // TODO: error
                throw -1;
            }

            return attr;
        }

        static name_type parseName(const string_type& str) {
            std::basic_istringstream<charT> sin(str);
            basic_parser<charT> parser(sin);
            name_type name;

            if (!parser.read_name(name)) {
                // TODO: error
                throw -1;
            }

            if (!parser.read_eof()) {
                // TODO: error
                throw -1;
            }

            return name;
        }

        static value_type parseValue(const string_type& str) {
            std::basic_istringstream<charT> sin(str);
            basic_parser<charT> parser(sin);
            value_type value;

            if (parser.read_attribute_value(value)) {
                // TODO: error
                throw -1;
            }

            return value;
        }

        name_type  mName;  //!< The name of an attribute.
        value_type mValue; //!< The value of an attribute.
    };
}

#endif /* ATTRIBUTE_H_INCLUDED */