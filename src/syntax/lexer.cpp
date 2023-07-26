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

#include <string>
#include <cctype>
#include <algorithm>
#include <utility>

#include "core/token.hpp"

#include "syntax/lexer.hpp"

namespace sylvia::syntax {
  struct keyword {
    core::token_type type;
    std::string value;
  };

  static const keyword keywords[] = {
    { core::token_type::NIL, "null" },
    { core::token_type::TRUE, "true" },
    { core::token_type::FALSE, "false" },
    { core::token_type::DEF, "def" }
  };

  extern core::token lexer::scan_next() {
    skip_whitespaces();
    if (_current == _end) {
      return scan_token(core::token_type::END);
    }

    _start = _current;

    char ch = *(_current++);
    if (std::isalpha(ch) || ch == '_') {
      return scan_kw_or_id();
    }

    if (std::isdigit(ch)) {
      return scan_number();
    }

    switch (ch) {
      case '"': return scan_string();
      case '(': return scan_token(core::token_type::OPN_PAREN);
      case ')': return scan_token(core::token_type::CLS_PAREN);
      case ',': return scan_token(core::token_type::COMMA);
      case ':': return scan_with_choice(
        ':',
        core::token_type::DBL_COLON,
        core::token_type::SGL_COLON
      );
      case '=': return scan_with_choice(
        '>',
        core::token_type::ARROW,
        core::token_type::EQUAL
      );
    }

    throw unexpect_char(scan_unknown());
  }

  extern core::token lexer::scan_kw_or_id() {
    while (std::isalnum(*_current) || *_current == '_') {
      ++_current;
    }

    std::string value(_start, _current);
    const keyword* it = std::find_if(
      std::begin(keywords), std::end(keywords),
      [&](const keyword& target) -> bool { return value == target.value; }
    );

    return {
      it == std::end(keywords) ? core::token_type::ID : it->type,
      std::move(value),
      _line
    };
  }

  extern core::token lexer::scan_string() {
    ++_current;
    while (_current != _end && *_current != '"') {
      ++_current;
    }
    if (_current == _end) {
      throw unterm_string(scan_unknown());
    }

    return {
      core::token_type::STRING,
      { _start + 1, _current++ },
      _line
    };
  }

  extern core::token lexer::scan_number() {
    while (std::isdigit(*_current)) ++_current;
    if (*_current == '.') {
      ++_current;
      while (std::isdigit(*_current)) ++_current;
    }
    return scan_token(core::token_type::NUMBER);
  }

  extern void lexer::skip_whitespaces() {
    while (_current != _end) {
      switch (*_current) {
        case ' ':
        case '\t':
        case '\r':
          ++_current;
          break;
        case '\n':
          ++_line;
          ++_current;
          break;
        default:
          return;
      }
    }
  }
}
