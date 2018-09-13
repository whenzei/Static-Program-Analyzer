#pragma once

#ifndef PQL_PARSER_H
#define PQL_PARSER_H

#include <string>
#include <vector>
#include "pql_query.h"
#include "tokenizer.h"
using std::string;
using std::vector;

class PqlParser {
  public:
  PqlParser(string, PqlQuery*);

  bool Parse();
  string GetErrorMessage();

  private:
  string queryText_;
  PqlQuery* query_;
  string errorMessage_;

  bool ParseStatement(string, bool);
  bool ParseSynonym(string);
  bool ParseSelect(TokenList);
  bool ParseDeclaration(TokenList);
  bool ParseSuchthat(TokenList, int*);
  bool ParseSuchthatParameter(TokenList, int*, string*, PqlDeclarationEntity*);
};

#endif