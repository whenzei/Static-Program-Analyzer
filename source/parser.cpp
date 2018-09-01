#pragma once

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "parser.h"
#include "pkb.h"
#include "simple_validator.h"
#include "tokenizer.h"

// Constructor
Parser::Parser(string filepath) {
  content_ = ReadContentFromFile(filepath);
  Parse();
}

string Parser::ReadContentFromFile(string filepath) {
  if (!IsValidFile(filepath)) {
    cout << "File not found! Content is set to empty string" << endl;
    return "";
  }
  // start reading file
  ifstream ifs(filepath);
  string content((istreambuf_iterator<char>(ifs)),
                 (istreambuf_iterator<char>()));
  return content;
}

void Parser::Parse() {
  // retrieve vector of tokens
  vector<Token> tokenized_content = Tokenizer::Tokenize(content_);

  // if we want to debug
  for (vector<Token>::const_iterator token = tokenized_content.begin();
       token != tokenized_content.end(); ++token) {
    cout << Tokenizer::Debug(*token) << endl;
  }

  if (SimpleValidator::validateProcedure(
          tokenized_content, 0, tokenized_content.size() - 1)) {
    // for debugging
    cout << "Procedure " << tokenized_content[1].value << " is syntactically correct" << endl;
  }
  
}

bool Parser::IsValidFile(string filepath) {
  ifstream infile(filepath);
  return infile.good();
}

