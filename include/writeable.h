#ifndef WRITEABLE_H_INCLUDED
#define WRITEABLE_H_INCLUDED

#include <ostream>

namespace xml {
    template <typename charT>
    class basic_writeable {
    public:
                //! \name Member types
        //!@{
        typedef std::basic_ostream<charT> ostream_t;  //!< The type of stream to write into

        typedef basic_writeable<charT> writeable_t; //!< The type of writeable.

        //!@}

        //! \brief Stream operator.
        /*!
         *  Write current writeable to the given output stream.
         *
         *  \param [in] os The output stream to write into.
         *
         *  \return The output stream given as parameter.
         */
        virtual ostream_t& operator<< (const ostream_t& os) = 0;
    };
}

#endif /* WRITEABLE_H_INCLUDED */
