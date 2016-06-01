#ifndef READABLE_H_INCLUDED
#define READABLE_H_INCLUDED

#include <string>
#include <istream>
#include <fstream>
#include <sstream>
#include <array>
#include <list>
#include <tuple>
#include <stack>

namespace xml {
    template <typename charT>
    class basic_readable {
    public:
        typedef charT                           char_t;
        typedef std::basic_string<charT>        string_t;
        typedef std::basic_istream<charT>       istream_t;
        typedef std::basic_ifstream<charT>      ifstream_t;
        typedef std::basic_istringstream<charT> istringstream_t;
        typedef std::list<string_t>             string_list_t;

    protected:
        basic_readable(istream_t& input = empty_stream)
        :
            mInput(input),
            mCurrentState(0, 0, mInput.tellg())
        {
            if (mInput.fail() || mInput.bad()) {
                throw -1; // TODO : exception
            } else if (mInput.eof() || mInput.good()) {
                // Do nothing
            } else {
                throw -1; // TODO : assert
            }
        }

        void push()
        {
            mSavedStates.emplace(mCurrentState);
        }

        void pop()
        {
            mCurrentState = mSavedStates.top();
            seek(std::get<2>(mCurrentState));
            drop();
        }

        void drop()
        {
            mSavedStates.pop();
        }


    private:
        typedef std::streamoff line_t;
        typedef size_t         column_t;
        typedef std::streampos index_t;

        typedef std::tuple<line_t, column_t, index_t> state_t;
        typedef std::stack<state_t>                   state_stack_t;

        static istream_t empty_stream;

        index_t tell()
        {
            index_t res = mInput.tellg();

            if (mInput.fail() || mInput.bad()) {
                throw -1; // TODO : exception
            } else if (mInput.eof() || mInput.good()) {
                // Do nothing
            } else {
                throw -1; // TODO : assert
            }

            return res;
        }

        void seek(index_t i)
        {
            mInput.seekg(i);

            if (mInput.fail() || mInput.bad()) {
                throw -1; // TODO : exception
            } else if (mInput.eof() || mInput.good()) {
                // Do nothing
            } else {
                throw -1; // TODO : assert
            }
        }

        char_t read()
        {
            // Read character
            char_t res = mInput.get();

            // Check for stream error
            if (mInput.fail() || mInput.bad()) {
                throw -1; // TODO : exception
            } else if (mInput.eof()) {
                return istream_t::traits_type::eof();
            } else if (mInput.good()) {
                // Do nothing
            } else {
                throw -1; // TODO : assert
            }

            // Update current state
            if (res == '\r' && peek() == '\n') { // Handle CRLF
                std::get<1>(mCurrentState)++;
                return read();
            } else if (res == '\n') {            // Update line & column indexes
                std::get<0>(mCurrentState)++;
                std::get<1>(mCurrentState) = 0;
            } else {                             // Update column index
                std::get<1>(mCurrentState)++;
            }

            // Update stream index
            std::get<2>(mCurrentState) = tell();

            return res;
        }

        char_t peek()
        {
            char_t c = mInput.peek();

            if (mInput.fail() || mInput.bad()) {
                throw -1; // TODO : exception
            } else if (mInput.eof()) {
                return istream_t::traits_type::eof();
            } else if (mInput.good()) {
                // Do nothing
            } else {
                throw -1; // TODO : assert
            }

            return c;
        }

    protected:
        bool match(const char_t c)
        {
            if (peek() != c) {
                return false;
            }

            read();
            return true;
        }

        bool match(const string_t& str)
        {
            push();

            for(auto it = str.begin(); it != str.end(); ++it) {
                if (!match(*it)) {
                    pop();
                    return false;
                }
            }

            drop();
            return true;
        }

        bool match_in(const char_t c1, const char_t c2, char_t& res = char_t())
        {
            char_t p = peek();
            if (p < c1 || p > c2) {
                return false;
            }

            res = read();
            return true;
        }

        bool match_in(const string_t& arr, char_t& res = char_t())
        {
            char_t p = peek();
            for(auto it = arr.begin(); it != arr.end(); ++it) {
                if (p == *it) {
                    res = read();
                    return true;
                }
            }

            return false;
        }

        bool match_not(const char_t c)
        {
            if (peek() == c) {
                return false;
            }

            return true;
        }

        bool match_not(const string_t& str)
        {
            push();

            for(auto it = str.begin(); it != str.end(); ++it) {
                if (!match(*it)) {
                    pop();
                    return true;
                }
            }

            pop();
            return false;
        }

        bool match_not_in(const char_t c1, const char_t c2, char_t& res = char_t())
        {
            char_t p = peek();
            if (p >= c1 && p <= c2) {
                return false;
            }

            res = read();
            return true;
        }

        bool match_not_in(const string_t& arr, char_t& res = char_t())
        {
            char_t p = peek();
            for(auto it = arr.begin(); it != arr.end(); ++it) {
                if (p == *it) {
                    return false;
                }
            }

            res = read();
            return true;
        }

        bool read_upper_letter(char_t& c)
        {
            return match_any('A', 'Z', c);
        }

        bool read_lower_letter(char_t& c)
        {
            return match_any('a', 'z', c);
        }

        bool read_digit(char_t& c)
        {
            return match_any('0', '9', c);
        }

        bool read_quote(char_t& c)
        {
            return match_any({'\'', '"'}, c);
        }


        bool read_char(char_t& c)
        {
            return
                match_any({0x9, 0xA, 0xD}, c) ||
                match_any(0x20, 0xD7FF, c) ||
                match_any(0xE000, 0xFFFD, c) ||
                match_any(0x10000, 0x10FFFF, c);
        }

        bool read_space(char_t& c)
        {
            return match_any({0x9, 0xA, 0xD, 0x20}, c);
        }

        bool read_name_start_char(char_t& c)
        {
            return
                match(':', c) ||
                read_upper_letter(c) ||
                match('_', c) ||
                read_lower_letter(c) ||
                match_any(0xC0, 0xD6, c) ||
                match_any(0xD8, 0xF6, c) ||
                match_any(0xF8, 0x2FF, c) ||
                match_any(0x370, 0x37D, c) ||
                match_any(0x37F, 0x1FFF, c) ||
                match_any(0x200C, 0x200D, c) ||
                match_any(0x2070, 0x218F, c) ||
                match_any(0x2C00, 0x2FEF, c) ||
                match_any(0x3001, 0xD7FF, c) ||
                match_any(0xF900, 0xFDCF, c) ||
                match_any(0xFDF0, 0xFFFD, c) ||
                match_any(0x10000, 0xEFFFF, c);
        }

        bool read_name_char(char_t& c)
        {
            return
                read_name_start_char(c) ||
                match('-', c) ||
                match('.', c) ||
                read_digit(c) ||
                match(0xB7, c) ||
                match_any(0x300, 0x36F, c) ||
                match_any(0x203F, 0x2040, c);
        }

        bool read_public_id_char(char_t& c)
        {
            return
                match(0x20, c) ||
                match(0xD, c) ||
                match(0xA, c) ||
                read_upper_letter(c) ||
                read_lower_letter(c) ||
                read_digit(c) ||
                match_any("-'()+,./:=?;!*#@$_%%", c);
        }

        bool read_name(string_t& name)
        {
            char_t c;
            name.clear();
            push();

            if (!read_name_start_char(c))
                goto error;
            name += c;            

            while(read_name_char(c))
                name += c;

            drop();
            return true;

        error:
            name.clear();
            pop();
            return false;
        }

        bool read_names(string_list_t& names)
        {
            string_t name;
            names.clear();
            push();

            if (!read_name(name))
                goto error;
            names += name;

            while (match(0x20) && read_name(name))
                names += name;

            drop();
            return true;

        error:
            names.clear();
            pop();
            return false;
        }

        bool read_token(string_t& token)
        {
            char_t c;
            token.clear();
            push();

            if (!read_name_char(c))
                goto error;
            token += c;            

            while(read_name_char(c))
                token += c;

            drop();
            return true;

        error:
            token.clear();
            pop();
            return false;

        }

        bool read_tokens(string_list_t& tokens)
        {
            string_t token;
            tokens.clear();
            push();

            if (!read_token(token))
                goto error;
            tokens += token;

            while (match(0x20) && read_token(token))
                tokens += token;

            drop();
            return true;

        error:
            tokens.clear();
            pop();
            return false;
        }

        bool read_character_data(string_t& data)
        {
            char_t c;
            while (match_not("]]>") && match_not_in("<&", c))
                data += c;

            return true;
        }

        bool read_comment(string_t& comment)
        {
            char_t c;
            comment.clear();
            push();

            if(!match("<!--"))
                goto error;

            while(match_not("-->") && read_char(c))
                if (c == '-' && !match_not("-->"))
                    goto error;
                else
                    comment += c;

            if(!match("-->"))
                goto error;

            drop();
            return true;

        error:
            comment.clear();
            pop();
            return false;
        }

        bool read_processing_instructions_content(string_t& content);
        bool read_processing_instructions_target(string_t& target);
        bool read_cdata(string_t& cdata);

        bool read_entity_value(const char_t quote, string_t& entity);
        bool read_attribute_value(const char_t quote, string_t& attribute);
        bool read_system_literal(const char_t quote, string_t& id);
        bool read_public_id_literal(const char_t quote, string_t& id);

        template <typename funcT>
        bool read_quoted(funcT func, string_t& value);

        template <typename funcT1, typename funcT2>
        bool read_name_value(
            funcT1 func1,
            string_t& name,
            funcT2 func2,
            string_t& value);
    private:
        istream_t& mInput;

        state_stack_t mSavedStates;
        state_t       mCurrentState;
    };
}

#endif /* READABLE_H_INCLUDED */