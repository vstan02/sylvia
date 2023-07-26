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

#ifndef _SYLVIA_CORE_TOKEN_HPP_
#define _SYLVIA_CORE_TOKEN_HPP_

#include <string>
#include <cstddef>
#include <utility>

namespace sylvia::core {
  enum class token_type {
    NIL,
    TRUE,
    FALSE,
    NUMBER,
    STRING,
    ID,

    DEF,

    EQUAL,
    COMMA,
    ARROW,
    SGL_COLON,
    DBL_COLON,
    OPN_PAREN,
    CLS_PAREN,

    UNKNOWN,
    END
  };

  struct token {
    token_type type;
    std::string value;
    std::size_t line;

    token(token_type type = token_type::END)
      : type(type)
      , value()
      , line(1) {}

    token(token_type type, std::size_t line)
      : type(type)
      , value()
      , line(line) {}

    token(token_type type, const std::string& value, std::size_t line)
      : type(type)
      , value(value)
      , line(line) {}

    token(token_type type, std::string&& value, std::size_t line)
      : type(type)
      , value(std::move(value))
      , line(line) {}
  };
}

#endif // _SYLVIA_CORE_TOKEN_HPP_
