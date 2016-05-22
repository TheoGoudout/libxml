#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <string>
#include <istream>
#include <list>
#include <stack>
#include <limits>

namespace xml {
    //! \brief A wrapper for a \c basic_istream.
    /*!
     *  This wrapper allows to keep track of states the stream was in
     *  using a stack of previous states.
     *
     *  It also defines methods to parse basic XML strings.
     */
    template <typename charT>
    class basic_parser {
    public:
        //! \name Member types
        //!@{
        typedef          std::basic_istream<charT> istream_type;
        typedef          std::basic_string<charT>  string_type;
        typedef typename istream_type::char_type   char_type;
        typedef typename istream_type::pos_type    pos_type;
        typedef typename istream_type::off_type    off_type;
        typedef          uint64_t                  line_type;

        //!@}

        //! \brief Constructor.
        /*!
         *  It will initialize the line and column number of the
         *  \c basic_istream.
         *
         *  \param [in] sin The \c basic_istream to wrap.
         */
        basic_parser(
            istream_type& sin)
        :
            mStream(sin),
            mLine(0),
            mCol(0)
        {}

        //! \brief Get the next character in stream.
        /*!
         *  This functions gets the next character in stream and returns it
         *  without consuming it.
         *
         *  \return The next character in stream.
         */
        char_type peek() const { return mStream.peek(); }

        //! \brief Read the next character in stream.
        /*!
         *  This functions gets the next character in stream, consumes it
         *  and returns it. It will also update the current line and/or column.
         *
         *  \return The next character in stream.
         */
        char_type read() {
            const char_type c = mStream.get();

            if (c == '\n') {
                ++mLine;
                mCol = 0;
            } else {
                ++mCol;
            }

            return c;
        }
        
        //! \brief Push the current stream state.
        /*!
         *  This function saves the current stream state and push it
         *  on top of the state stack. This state can be restored by
         *  calling \c pop, or dropped by calling \b drop.
         *
         *  \sa pop
         *  \sa drop
         */
        void push() { mStates.push(state(mStream.tellg(), mLine, mCol)); }

        //! \brief Pop a stream state.
        /*!
         *  This function pop the stream state on top of the
         *  state stack, and restore it.
         *
         *  \sa push
         *  \sa drop
         */
        void pop()  {
            const state& top = mStates.top();

            mStream.seekg(top.pos);
            mLine = top.line;
            mCol = top.col;

            mStates.pop();
        }

        //! \brief Drop a stream state.
        /*!
         *  This function drops the stream state on top of the
         *  state stack.
         *
         *  \sa push
         *  \sa pop
         */
        void drop() { mStates.pop(); }

        bool read(
            charT& c,
            uint64_t value)
        {
            static const uint64_t max = std::numeric_limits<charT>::max();
            const uint64_t current = peek();
            if (current <= max && current == value) {
                c = read();
                return true;
            }

            return false;
        }

        bool read(
            charT& c,
            uint64_t first,
            uint64_t last)
        {
            static const uint64_t max = std::numeric_limits<charT>::max();
            const uint64_t current = peek();
            if (current >= first && current <= max && current <= last) {
                c = read();
                return true;
            }

            return false;
        }

        bool read_eof()
        {
            // TODO: implement function
            return true;
        }

        bool read_digit(
            charT& c)
        {
            return read(c, '0', '9');
        }

        bool read_upper_letter(
            charT& c)
        {
            return read(c, 'A', 'Z');
        }

        bool read_lower_letter(
            charT& c)
        {
            return read(c, 'a', 'z');
        }

        bool read_char(
            charT& c)
        {
            return 
                read(c, 0x9) ||
                read(c, 0xA) ||
                read(c, 0xD) ||
                read(c, 0x20, 0xD7FF) ||
                read(c, 0xE000, 0xFFFD) ||
                read(c, 0x10000, 0x10FFFF);
        }

        bool read_whitespace(
            charT& c)
        {
            return
                read(c, 0x9) ||
                read(c, 0xA) ||
                read(c, 0xD) ||
                read(c, 0x20);
        }

        bool read_name_start_char(
            charT& c)
        {
            return
                read(c, ':') ||
                read_upper_letter(c) ||
                read(c, '_') ||
                read_lower_letter(c) ||
                read(c, 0xC0, 0xD6) ||
                read(c, 0xD8, 0xF6) ||
                read(c, 0xF8, 0x2FF) ||
                read(c, 0x370, 0x37D) ||
                read(c, 0x37F, 0x1FFF) ||
                read(c, 0x200C, 0x200D) ||
                read(c, 0x2070, 0x218F) ||
                read(c, 0x2C00, 0x2FEF) ||
                read(c, 0x3001, 0xD7FF) ||
                read(c, 0xF900, 0xFDCF) ||
                read(c, 0xFDF0, 0xFFFD) ||
                read(c, 0x10000, 0xEFFFF);
        }

        bool read_name_char(
            charT& c)
        {
            return 
                read_name_start_char(c) ||
                read(c, '-') ||
                read(c, '.') ||
                read_digit(c) ||
                read(c, 0xB7) ||
                read(c, 0x300, 0x36F) ||
                read(c, 0x203F, 0x2040);
        }

        bool read_public_id_char(
            charT& c)
        {
            return
                read(c, 0xA) ||
                read(c, 0xD) ||
                read(c, 0x20) ||
                read_lower_letter(c) ||
                read_upper_letter(c) ||
                read_digit(c) ||
                read(c, '-') ||
                read(c, '\'') ||
                read(c, '(') ||
                read(c, ')') ||
                read(c, '+') ||
                read(c, ',') ||
                read(c, '.') ||
                read(c, '/') ||
                read(c, ':') ||
                read(c, '=') ||
                read(c, '?') ||
                read(c, ';') ||
                read(c, '!') ||
                read(c, '*') ||
                read(c, '#') ||
                read(c, '@') ||
                read(c, '$') ||
                read(c, '_') ||
                read(c, '%');
        }

        bool read_encoding_start_char(
            charT& c)
        {
            return
                read_upper_letter(c) ||
                read_lower_letter(c);
        }

        bool read_encoding_char(
            charT& c)
        {
            return
                read_upper_letter(c) ||
                read_lower_letter(c) ||
                read_digit(c) ||
                read(c, '.') ||
                read(c, '_') ||
                read(c, '-');
        }


        bool read_whitespaces(
            std::basic_string<charT>& whitespaces)
        {
            whitespaces.clear();

            charT c;
            if (!read_whitespace(c)) {
                return false;
            }

            whitespaces += c;
            while(read_whitespace(c)) {
                whitespaces += c;
            }

            return true;
        }

        bool read_name(
            std::basic_string<charT>& name)
        {
            name.clear();

            charT c;
            if (!read_name_start_char(c)) {
                return false;
            }

            name += c;
            while(read_name_char(c)) {
                name += c;
            }

            return true;
        }

        bool read_names(
            std::list<std::basic_string<charT> >& names)
        {
            names.clear();

            std::basic_string<charT> name;
            if (!read_name(name)) {
                return false;
            }

            names.push_back(name);

            while (true) {
                push();
                
                charT c;
                if(read(c, 0x20) && read_name(name)) {
                    drop();
                    names.push_back(name);
                    push();
                } else {
                    pop();
                    return true;
                }
            }
        }

        bool read_token(
            std::basic_string<charT>& token)
        {
            token.clear();

            charT c;
            if (!read_name_char(c)) {
                return false;
            }

            token += c;
            while(read_name_char(c)) {
                token += c;
            }

            return true;
        }

        bool read_tokens(
            std::list<std::basic_string<charT> >& tokens)
        {
            tokens.clear();

            std::basic_string<charT> token;
            if (!read_name(token)) {
                return false;
            }

            tokens.push_back(token);

            while (true) {
                push();
                
                charT c;
                if(read(c, 0x20) && read_name(token)) {
                    drop();
                    tokens.push_back(token);
                    push();
                } else {
                    pop();
                    return true;
                }
            }
        }

        bool read_public_id_literal(
            std::basic_string<charT>& publicIdLiteral);

        bool read_system_literal(
            std::basic_string<charT>& systemLiteral);

        bool read_attribute_value(
            std::basic_string<charT>& attributeValue);

        bool read_entity_value(
            std::basic_string<charT>& entityValue);

        bool read_attribute_name_value(
            std::basic_string<charT>& attributeName,
            std::basic_string<charT>& attributeValue);

        bool read_char_data(
            std::basic_string<charT>& charData);



        bool read_equal(
            std::basic_string<charT>& equal);



        bool read_encoding_name(
            std::basic_string<charT>& encodingName);

    private:
        //! \brief A state of a \c istream.
        /*!
         *  It represents the state of a \c istream at a given time. Used internally only.
         */
        class state {
        public:
            //! \brief Construct a \c istream state.
            state(pos_type pos, line_type line, off_type col) :
                pos(pos),
                line(line),
                col(col)
            {}

            const pos_type pos;  //!< The position in the stream for this state.
            const pos_type line; //!< The line in the stream for this state.
            const pos_type col;  //!< The column in the stream for this state.
        };


        istream_type& mStream; //!< The \c basic_istream to wrap.

        line_type mLine; //!< Current line in stream.
        off_type  mCol;  //!< Current column in stream.

        std::stack<state> mStates; //!< A stack of \c state.
    };
}

#endif /* PARSER_H_INCLUDED */