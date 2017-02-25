#ifndef VERSION_H_INCLUDED
#define VERSION_H_INCLUDED

#include <string>

#include <strings.h>
#include <readable.h>

namespace xml
{
    //! \brief The version of a XML document
    /*!
     *  It stores the version number of a XML document.
     *  The version number is in format "<major>.<minor>".
     */
    template <typename charT>
    class basic_version {
    public:
        typedef basic_version<charT> version_t;
        typedef basic_readable<charT> readable_t; //!< The stream input type.
        typedef std::basic_string<charT> string_t; //!< The string type.

        
        basic_version(
            uint8_t major,
            uint8_t minor)
        :
            mMajor(major),
            mMinor(minor)
        {}


        const uint8_t major () const
        {
            return mMajor;
        }

        const uint8_t minor () const
        {
            return mMinor;
        }


        static version_t* read(readable_t& input, bool throwLastError = false)
        {
            string_t version(strings::converter<strings::version, charT>::value);
            uint8_t major;
            uint8_t minor;

            bool ret = input.read_name_and_quoted_value(
                true,
                &readable_t::match_name,   version,
                &readable_t::read_version, major, minor
            );

            if (ret)
                return new version_t(major, minor);

            if (throwLastError)
                input.throw_last_error();            

            return nullptr;
        }

    private:
        uint8_t mMajor; //!< The major version number.
        uint8_t mMinor; //!< The minor version number.
    };

}

#endif /* VERSION_H_INCLUDED */