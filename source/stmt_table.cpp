#include "stmt_table.h"
#include "statement.h"

#include <string>

using std::string;

StmtTable::StmtTable() {
  num_stmt_ = 0;
}

StmtTable::~StmtTable() {
  delete[] * stmt_table_;
}

// Prerequisite: The line number and statement are valid and non-null.
int StmtTable::InsertStmt(int line_num, string stmt) {
  Statement newStmt(line_num, stmt);
  *stmt_table_[num_stmt_] = newStmt;
  num_stmt_++;
  return (num_stmt_ -1);
}

int* StmtTable::GetAllStmtNums() {
  int stmt_nums[kTempArraySize];
	
  if (num_stmt_ == 0) { return NULL;  }
  for (int i = 0; i < num_stmt_; i++) {
    stmt_nums[i] = stmt_table_[i]->GetLineNum();
  }

  return stmt_nums;
}