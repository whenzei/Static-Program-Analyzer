#include "parse_data.h"

ParseData::ParseData(StmtNumInt stmt_num, VarNameSet used_vars,
                     VarNameSet modified_vars) {
  stmt_num_ = stmt_num;
  used_vars_ = used_vars;
  modified_vars_ = modified_vars;
}

ParseData::ParseData(StmtNumIntList stmt_num_list, VarNameSet used_vars,
                     VarNameSet modified_vars) {
  stmt_num_list_ = stmt_num_list;
  used_vars_ = used_vars;
  modified_vars_ = modified_vars;
}

ParseData::ParseData(StmtNumIntList stmt_num_list, VarNameSet used_vars,
                     VarNameSet modified_vars,
                     StmtNumIntList nested_last_stmts_1,
                     StmtNumIntList nested_last_stmts_2) {
  nested_last_stmts_1_ = nested_last_stmts_1;
  nested_last_stmts_2_ = nested_last_stmts_2;
  stmt_num_list_ = stmt_num_list;
  used_vars_ = used_vars;
  modified_vars_ = modified_vars;
}


ParseData::ParseData(StmtNumInt stmt_num, VarNameSet used_vars,
                     VarNameSet modified_vars, StmtNumIntList nested_last_stmts_1,
                     StmtNumIntList nested_last_stmts_2) {
  nested_last_stmts_1_ = nested_last_stmts_1;
  nested_last_stmts_2_ = nested_last_stmts_2;
  stmt_num_ = stmt_num;
  used_vars_ = used_vars;
  modified_vars_ = modified_vars;
}

ParseData::ParseData(VarNameSet used_vars, VarNameSet modified_vars) {
  used_vars_ = used_vars;
  modified_vars_ = modified_vars;
}

ParseData::ParseData(VarNameSet used_vars, VarName modified_var) {
  used_vars_ = used_vars;
  modified_var_ = modified_var;
}

ParseData::ParseData(VarNameSet used_vars,
                     VarNameSet modified_vars,
                     StmtNumIntList nested_last_stmts_1,
                     StmtNumIntList nested_last_stmts_2) {
  nested_last_stmts_1_ = nested_last_stmts_1;
  nested_last_stmts_2_ = nested_last_stmts_2;
  used_vars_ = used_vars;
  modified_vars_ = modified_vars;
}

StmtNumInt ParseData::GetStmtNum() { return stmt_num_; }

StmtNumIntList ParseData::GetStmtNumList() { return stmt_num_list_; }

VarNameSet ParseData::GetUsedVariables() { return used_vars_; }

VarNameSet ParseData::GetModifiedVariables() { return modified_vars_; }

VarName ParseData::GetModifiedVariable() { return modified_var_; }

StmtNumIntList ParseData::GetNestedLastStmtsOne() { return nested_last_stmts_1_; }

StmtNumIntList ParseData::GetNestedLastStmtsTwo() { return nested_last_stmts_2_; }
