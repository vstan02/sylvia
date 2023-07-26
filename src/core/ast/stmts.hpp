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

#ifndef _SYLVIA_CORE_AST_STATEMENTS_HPP_
#define _SYLVIA_CORE_AST_STATEMENTS_HPP_

#include <variant>

#include "core/token.hpp"
#include "core/ast/node.hpp"
#include "core/ast/values.hpp"

namespace sylvia::core {
  struct ast_stmt;

  enum class ast_stmt_type {
    CONST
  };

  struct ast_stmt_const {
    token name;
    ast_node<ast_value> value;

    ast_stmt_const(
      const token& name,
      const ast_node<ast_value>& value
    ) : name(name)
      , value(value) {}
  };

  struct ast_stmt {
    ast_stmt_type type;
    std::variant<
      ast_stmt_const
    > target;

    ast_stmt(const ast_stmt_const& target)
      : type(ast_stmt_type::CONST)
      , target(target) {}
  };
}

#endif // _SYLVIA_CORE_AST_STATEMENTS_HPP_
