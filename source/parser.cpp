#pragma once

#include <fstream>
#include <iostream>
#include <stack>

#include "expression_helper.h"
#include "parser.h"
#include "pkb.h"
#include "statement_data.h"
#include "tokenizer.h"
#include "util.h"
#include "validator.h"

const bool DEBUG_FLAG = true;

using std::cout;
using std::endl;
using std::stack;

using tt = Tokenizer::TokenType;
using ts = Tokenizer::TokenSubtype;

const std::unordered_set<string> kKeywords({"procedure", "read", "call",
                                            "print", "if", "then", "else",
                                            "while"});

Parser::Parser(PKB* pkb) {
  pkb_ = pkb;
  current_index_ = 0;  // index for list of tokens
  stmt_num_ = 0;
  stmt_list_num_ = 1;
}

void Parser::Parse(string filepath) {
  // read content from file
  string contents = Util::ReadContentFromFile(filepath);
  // retrieve vector of tokens
  tokens_ = Tokenizer::Tokenize(contents);

  // validate tokens for syntax errors
  Validator validator = Validator(tokens_);
  if (!validator.ValidateProgram()) {
    return;
  }

  ParseProgram();
}

void Parser::ParseProgram() {
  do {
    ProcessProcedure(stmt_list_num_);
  } while (!IsAtEnd());
}

void Parser::ProcessProcedure(int given_stmt_list_index) {
  ReadNextToken();
  pkb_->InsertProcName(ReadNextToken().value);
  // eat the open brace
  ReadNextToken();

  ProcessStatementList(given_stmt_list_index);

  ReadNextToken();
}

void Parser::ProcessStatementList(int given_stmt_list_num) {
  do {
    ProcessStatement(given_stmt_list_num);
  } while (!IsNextType(tt::kCloseBrace));
}

void Parser::ProcessStatement(int given_stmt_list_num) {
  ++stmt_num_;
  ReadNextToken();
  if (IsCurrentTokenKeyword() && !IsNextType(tt::kAssignment)) {
    ProcessKeyword(given_stmt_list_num);
  } else {
    ProcessAssignment(given_stmt_list_num);
  }
}

void Parser::ProcessKeyword(int given_stmt_list_num) {
  if (IsCurrentKeywordType(ts::kIf)) {
    ProcessIfBlock(given_stmt_list_num);
  } else if (IsCurrentKeywordType(ts::kWhile)) {
    ProcessWhileBlock(given_stmt_list_num);
  } else if (IsCurrentKeywordType(ts::kCall)) {
    // todo call handling
  } else if (IsCurrentKeywordType(ts::kRead)) {
  } else if (IsCurrentKeywordType(ts::kPrint)) {
  }
}

void Parser::ProcessAssignment(int given_stmt_list_num) {
  VarName lhs_var = current_token_.value;
  VarNameSet rhs_vars;
  ConstValueSet rhs_consts;
  TokenList infix_tokens_rhs;
  TokenList postfix_tokens_rhs;

  while (!IsCurrentType(tt::kSemicolon)) {
    ReadNextToken();
    if (IsCurrentType(tt::kName)) {
      rhs_vars.insert(current_token_.value);
    } else if (IsCurrentType(tt::kDigit)) {
      rhs_consts.insert(stoi(current_token_.value));
    }
    infix_tokens_rhs.push_back(current_token_);
  }

  // postfix_tokens_rhs to be passed into PKB
  postfix_tokens_rhs = ExpressionHelper::ToPostfix(infix_tokens_rhs);

  // Update PKB of assignment statement
  pkb_->InsertAssignStmt(&AssignStmtData(stmt_num_, given_stmt_list_num,
                                         lhs_var, rhs_vars, rhs_consts,
                                         postfix_tokens_rhs));

  //**************** DEBUG********************
  string rhsvar = string();
  for (VarName var : rhs_vars) {
    rhsvar = rhsvar + " " + var;
  }
  string rhsconst = string();
  for (int var : rhs_consts) {
    rhsconst = rhsconst + " " + std::to_string(var);
  }

  cout << "Assignment statement#" << stmt_num_
       << " added, stmtLst#: " << given_stmt_list_num << ", lhs: " << lhs_var
       << ", rhs_vars: " << rhsvar << ", rhs_consts: " << rhsconst << endl;
  //***********************************************
}

void Parser::ProcessIfBlock(int given_stmt_list_num) {
  int if_stmt_num = stmt_num_;
  int then_stmt_list_num = ++stmt_list_num_;
  int else_stmt_list_num = ++stmt_list_num_;
  VarNameSet control_vars;
  control_vars = ProcessConditional();

  //************* DEBUG******************
  string control_var_str = string();
  for (VarName var : control_vars) {
    control_var_str = control_var_str + " " + var;
  }
  cout << "If statement#" << if_stmt_num
       << " added, stmtLst#: " << given_stmt_list_num
       << ", control_variable: " << control_var_str << endl;
  //*************************************

  // eat 'then {' tokens
  EatNumTokens(2);

  cout << "[inside then block, stmtLst# should be " << then_stmt_list_num << "]"
       << endl;
  // Process everything inside the if block
  ProcessStatementList(then_stmt_list_num);

  // eat "} else {" tokens
  EatNumTokens(3);
  cout << "[inside else block, stmtLst# should be " << else_stmt_list_num << "]"
       << endl;
  // Process everything inside the counterpart else block
  ProcessStatementList(else_stmt_list_num);

  // eat closing brace
  ReadNextToken();

  cout << "[leaving if block]" << endl;

  // Update PKB of the 'if' block
  pkb_->InsertIfStmt(if_stmt_num, given_stmt_list_num, then_stmt_list_num,
                     else_stmt_list_num, control_vars);
}

void Parser::ProcessWhileBlock(int given_stmt_list_num) {
  VarNameSet control_vars;
  int while_stmt_num = stmt_num_;
  int while_stmt_list_num = ++stmt_list_num_;
  control_vars = ProcessConditional();

  //************* DEBUG******************
  string control_var_str = string();
  for (VarName var : control_vars) {
    control_var_str = control_var_str + " " + var;
  }
  cout << "While statement#" << while_stmt_num
       << " added, stmtList#: " << given_stmt_list_num
       << ", control_variable: " << control_var_str << endl;
  //*************************************

  // eat open brace
  ReadNextToken();

  cout << "[inside while block, stmtLst# should be " << while_stmt_list_num
       << "]" << endl;

  ProcessStatementList(while_stmt_list_num);

  // eat close brace
  ReadNextToken();

  cout << "[leaving while block]" << endl;

  // Update PKB of the 'while' block
  pkb_->InsertWhileStmt(while_stmt_num, given_stmt_list_num,
                        while_stmt_list_num, control_vars);
}

VarNameSet Parser::ProcessConditional() {
  stack<Token> paren_stack;

  // Push "(" onto stack
  paren_stack.push(ReadNextToken());
  ReadNextToken();

  VarNameSet control_vars;

  while (!paren_stack.empty()) {
    if (current_token_.type == tt::kOpenParen) {
      paren_stack.push(current_token_);
    }
    if (current_token_.type == tt::kName) {
      control_vars.insert(current_token_.value);
    }
    if (current_token_.type == tt::kCloseParen) {
      paren_stack.pop();
    }

    // Stops reading the next token when at the end of conditional ')'
    if (!paren_stack.empty()) {
      ReadNextToken();
    }
  }

  return control_vars;
}

// Helper methods
bool Parser::IsAtEnd() { return PeekNextToken().type == tt::kEOF; }

bool Parser::IsNextType(Tokenizer::TokenType type) {
  return PeekNextToken().type == type;
}

bool Parser::IsCurrentType(Tokenizer::TokenType type) {
  return current_token_.type == type;
}

bool Parser::IsCurrentKeywordType(Tokenizer::TokenSubtype type) {
  return current_token_.subtype == type;
}

bool Parser::IsNextKeywordType(Tokenizer::TokenSubtype type) {
  return PeekNextToken().subtype == type;
}

bool Parser::IsCurrentTokenKeyword() {
  return current_token_.subtype != ts::kNone;
}

bool Parser::IsNextTokenKeyword() {
  return PeekNextToken().subtype != Tokenizer::TokenSubtype::kNone;
}

Token Parser::ReadNextToken() {
  if (!IsAtEnd()) {
    current_index_++;
  }
  current_token_ = tokens_[current_index_ - 1];
  return current_token_;
}

Token Parser::PeekNextToken() { return tokens_[current_index_]; }

void Parser::EatNumTokens(int num) {
  current_index_ += num;
  current_token_ = tokens_[current_index_];
}