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

#ifndef _SYLVIA_SYNTAX_PARSER_HPP_
#define _SYLVIA_SYNTAX_PARSER_HPP_

#include <string>

#include "core/token.hpp"
#include "core/ast/node.hpp"
#include "core/ast/stmts.hpp"
#include "core/ast/values.hpp"

#include "syntax/lexer.hpp"

namespace sylvia::syntax {
  class parser {
    public:
      parser(const lexer& lexer)
        : _lexer(lexer)
        , _token() {}

      parser(
        std::string::const_iterator begin,
        std::string::const_iterator end
      ) : _lexer(begin, end)
        , _token() {}

      core::ast_nodes<core::ast_stmt> parse();

    private:
      lexer _lexer;
      core::token _token;

    private:
      void advance();

      core::ast_node<core::ast_stmt> parse_stmt();
      core::ast_node<core::ast_stmt> parse_stmt_const();

      core::ast_node<core::ast_value> parse_value();
      core::ast_node<core::ast_value> parse_value_call();
      core::ast_node<core::ast_value> parse_value_func();
      core::ast_node<core::ast_value> parse_value_base();
  };
}

#endif // _SYLVIA_SYNTAX_PARSER_HPP_
