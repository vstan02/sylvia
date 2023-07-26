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

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "core/token.hpp"
#include "core/ast/node.hpp"
#include "core/ast/stmts.hpp"
#include "core/ast/values.hpp"

#include "syntax/lexer.hpp"
#include "syntax/parser.hpp"

namespace sylvia::syntax {
  extern core::ast_nodes<core::ast_stmt> parser::parse() {
    core::ast_nodes<core::ast_stmt> result;
    advance();
    while (_token.type != core::token_type::END) {
      result.push_back(parse_stmt());
    }
    return result;
  }

  extern std::shared_ptr<core::ast_stmt> parser::parse_stmt() {
    return parse_stmt_const();
  }

  extern core::ast_node<core::ast_stmt> parser::parse_stmt_const() {
    if (_token.type != core::token_type::DEF) {
      throw std::runtime_error("Expected a 'def'!");
    }
    advance();
    if (_token.type != core::token_type::ID) {
      throw std::runtime_error("Expected an id!");
    }
    core::token token = std::move(_token);
    advance();
    if (_token.type != core::token_type::EQUAL) {
      throw std::runtime_error("Expected an '='!");
    }
    advance();
    auto value = parse_value();
    return core::make_node<core::ast_stmt>(
      core::ast_stmt_const(token, value)
    );
  }

  extern core::ast_node<core::ast_value> parser::parse_value() {
    return _token.type == core::token_type::OPN_PAREN
      ? parse_value_func()
      : parse_value_call();
  }

  extern core::ast_node<core::ast_value> parser::parse_value_base() {
    if (_token.type != core::token_type::NUMBER && _token.type != core::token_type::ID) {
      throw std::runtime_error("Expected a value!");
    }
    core::token token = std::move(_token);
    advance();
    return core::make_node<core::ast_value>(
      core::ast_value_base(token)
    );
  }

  extern core::ast_node<core::ast_value> parser::parse_value_call() {
    auto base_value = parse_value_base();
    if (_token.type != core::token_type::OPN_PAREN) {
      return base_value;
    }

    advance();
    core::ast_nodes<core::ast_value> args;
    while (_token.type != core::token_type::CLS_PAREN && _token.type != core::token_type::END) {
      args.push_back(parse_value());
      if (_token.type == core::token_type::COMMA) {
        advance();
      } else if (_token.type != core::token_type::CLS_PAREN) {
        throw std::runtime_error("What??");
      }
    }
    if (_token.type == core::token_type::CLS_PAREN) {
      advance();
    }
    return core::make_node<core::ast_value>(
      core::ast_value_call(
        std::get<core::ast_value_base>(base_value->target).value,
        args
      )
    );
  }

  extern core::ast_node<core::ast_value> parser::parse_value_func() {
    advance();
    std::vector<core::token> params;
    while (_token.type != core::token_type::CLS_PAREN && _token.type != core::token_type::END) {
      if (_token.type == core::token_type::NUMBER || _token.type == core::token_type::ID) {
        params.push_back(_token);
        advance();
      } else {
        throw std::runtime_error("What???");
      }

      if(_token.type != core::token_type::COMMA && _token.type != core::token_type::CLS_PAREN) {
        throw std::runtime_error("What happened?");
      } else if (_token.type == core::token_type::COMMA) {
        advance();
      }
    }

    if (_token.type == core::token_type::CLS_PAREN) {
      advance();
    }

    if(_token.type != core::token_type::ARROW) {
      throw std::runtime_error("Where is the arrow?");
    }
    advance();
    auto base_value = parse_value();
    return core::make_node<core::ast_value>(
      core::ast_value_func(base_value, params)
    );
  }

  extern void parser::advance() {
    _token = _lexer.scan_next();
  }
}
