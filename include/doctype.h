#ifndef DOCTYPE_H_INCLUDED
#define DOCTYPE_H_INCLUDED

#include <node.h>

namespace xml {
    template <typename charT>
    class basic_doctype : public basic_node<charT> {
    public:
        virtual std::basic_string<charT> type() const;

    };

    typedef basic_doctype<char>    doctype;  //!< A specialized \c basic_doctype for char.
    typedef basic_doctype<wchar_t> wdoctype; //!< A specialized \c basic_doctype for wchar_t.
}

#endif /* DOCTYPE_H_INCLUDED */