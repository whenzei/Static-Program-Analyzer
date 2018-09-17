#pragma once

#ifndef SPA_MODIFIES_TABLE_H
#define SPA_MODIFIES_TABLE_H

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

using std::list;
using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;

using StmtNum = string;
using StmtNumList = list<string>;
using VarName = string;
using VarNameList = list<string>;
using VarNameSet = unordered_set<string>;
using StmtListIndex = int;
using ModifiesMap = unordered_map<string, list<string>>;
using ModifiedByMap = unordered_map<string, list<string>>;
using ModifiesStmtListMap = unordered_map<int, unordered_set<string>>;
using StmtVarPairList = list<pair<string, string>>;
using ProcVarPairList = list<pair<string, string>>;

class ModifiesTable {
  StmtNumList modifying_stmt_num_list_;
  VarNameList modified_var_name_list_;
  ModifiesMap modifies_map_;
  ModifiedByMap modified_by_map_;
  ModifiesStmtListMap modifies_stmtlist_map_;

 public:
  void InsertModifies(StmtNum stmt_num, StmtListIndex stmtlist_index,
                      VarName var_name);

  bool IsModifiedBy(StmtNum stmt_num, VarName var_name);

  bool IsModified(VarName var_name);

  VarNameList GetModifiedVar(StmtNum stmt_num);

  VarNameSet GetModifiedVar(StmtListIndex stmtlist_index);

  VarNameList GetAllModifiedVar();

  StmtNumList GetModifyingStmt(VarName var_name);

  StmtNumList GetAllModifyingStmt();

  bool HasModifiesRelationship();

  StmtVarPairList GetAllModifiesPair();
};

#endif !SPA_MODIFIES_TABLE_H