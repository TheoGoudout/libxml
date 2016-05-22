#ifndef ELEMENT_H_INCLUDED
#define ELEMENT_H_INCLUDED

#include <node.h>
#include <text.h>

namespace xml {
    template <typename charT>
    class basic_element : public basic_node<charT> {
    public:
        typedef          basic_parent_node<charT> parent_type;
        typedef          basic_element<charT>     element_type;
        typedef          basic_text<charT>        text_type;
        typedef typename parent_type::child_type  child_type;
        typedef typename parent_type::size_type   size_type;

        virtual std::basic_string<charT> type() const;

        template <class classT = child_type>
        using iterator               = typename parent_type::template iterator<classT>;
        template <class classT = child_type>
        using const_iterator         = typename parent_type::template const_iterator<classT>;
        template <class classT = child_type>
        using reverse_iterator       = typename parent_type::template reverse_iterator<classT>;
        template <class classT = child_type>
        using const_reverse_iterator = typename parent_type::template const_reverse_iterator<classT>;

        iterator<> insert(const_iterator<> position, const element_type& val);
        iterator<> insert(const_iterator<> position, element_type&& val);

        iterator<> insert(const_iterator<> position, const text_type& val);
        iterator<> insert(const_iterator<> position, text_type&& val);
    };

    typedef basic_element<char>    element;  //!< A specialized \c basic_element for char.
    typedef basic_element<wchar_t> welement; //!< A specialized \c basic_element for wchar_t.
}

#endif /* ELEMENT_H_INCLUDED */
