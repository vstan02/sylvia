/* Copyright (C) 2023 Stan Vlad <vstan02@protonmail.com>
 *
 * This file is part of Sylvia.
 *
 * Sylvia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _SYLVIA_SYNTAX_LEXER_HPP_
#define _SYLVIA_SYNTAX_LEXER_HPP_

#include <string>
#include <stdexcept>

#include "core/token.hpp"

namespace sylvia::syntax {
  class lexer {
    public:
      class exception: public std::runtime_error {
        public:
          exception(const char* message, const core::token& token)
            : std::runtime_error(message)
            , _token(token) {}

          exception(const char* message, core::token&& token)
            : std::runtime_error(message)
            , _token(token) {}

          core::token where() const noexcept { return _token; }

        private:
          core::token _token;
      };

      class unexpect_char: exception {
        public:
          unexpect_char(const core::token& token)
            : exception("Unexpected character", token) {}

          unexpect_char(core::token&& token)
            : exception("Unexpected character", token) {}
      };

      class unterm_string: exception {
        public:
          unterm_string(const core::token& token)
            : exception("Unterminated string", token) {}

          unterm_string(core::token&& token)
            : exception("Unterminated string", token) {}
      };

    public:
      lexer(
        std::string::const_iterator begin,
        std::string::const_iterator end
      ) : _line(1)
        , _start(begin)
        , _end(end)
        , _current(begin) {}

      core::token scan_next();

    private:
      std::size_t _line;
      std::string::const_iterator _start;
      std::string::const_iterator _end;
      std::string::const_iterator _current;

    private:
      void skip_whitespaces();

      core::token scan_kw_or_id();
      core::token scan_number();
      core::token scan_string();

      core::token scan_with_choice(
        char expected,
        core::token_type first,
        core::token_type second
      ) {
        return scan_token(*(_current++) == expected ? first : second);
      }

      core::token scan_unknown() {
        return scan_token(core::token_type::UNKNOWN);
      }

      core::token scan_token(core::token_type type) {
        return { type, { _start, _current }, _line };
      }
  };
}

#endif // _SYLVIA_SYNTAX_LEXER_HPP_
