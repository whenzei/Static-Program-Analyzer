#pragma once

#include "pkb.h"
#include <sstream>

using std::make_pair;
using std::stringstream;

bool PKB::InsertProcName(ProcName proc_name) {
  return proc_list_.InsertProcName(proc_name);
}

ProcNameList PKB::GetAllProcName() { return proc_list_.GetAllProcName(); }

VarNameList PKB::GetAllVarName() { return var_list_.GetAllVarName(); }

bool PKB::InsertConstValue(ConstValue const_value) {
  return const_list_.InsertConstValue(const_value);
}

ConstValueList PKB::GetAllConstValue() {
  return const_list_.GetAllConstValue();
}

StmtType PKB::GetStmtType(StmtNumInt stmt_num_int) {
  return stmt_table_.GetStmtType(ToString(stmt_num_int));
}

bool PKB::InsertAssignStmt(StmtNumInt stmt_num_int,
                           StmtListIndex stmtlist_index,
                           VarName modified_var_name,
                           VarNameSet used_var_name_set) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, PqlDeclarationEntity::kAssign, stmtlist_index)) {
    // insert statement
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertAssignStmt(stmt_num);
    var_list_.InsertVarName(modified_var_name);
    // insert variables
    for (auto& var_name : used_var_name_set) {
      var_list_.InsertVarName(var_name);
    }
    // TODO: add follows and uses relationship
    modifies_table_.InsertModifies(stmt_num, modified_var_name);
    StmtNumList parents = parent_table_.GetParentT(stmtlist_index);
    for (StmtNum& parent : parents) {
      modifies_table_.InsertModifies(parent, modified_var_name);
    }
    return true;
  } else {
    return false;
  }
}

bool PKB::InsertWhileStmt(StmtNumInt stmt_num_int,
                          StmtListIndex parent_stmtlist_index,
                          StmtListIndex child_stmtlist_index,
                          VarNameSet control_var_name_set) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, PqlDeclarationEntity::kWhile,
                             parent_stmtlist_index)) {
    // insert statement
    stmtlist_table_.InsertStmt(stmt_num, parent_stmtlist_index);
    stmt_type_list_.InsertWhileStmt(stmt_num);
    // insert variables
    for (auto& var_name : control_var_name_set) {
      var_list_.InsertVarName(var_name);
    }
    // insert parent relationships
    parent_table_.InsertDirectParentRelationship(stmt_num,
                                                 child_stmtlist_index);
    StmtNumList indirect_parents =
        parent_table_.GetParentT(parent_stmtlist_index);
    for (StmtNum& indirect_parent : indirect_parents) {
      parent_table_.InsertIndirectParentRelationship(indirect_parent,
                                                     child_stmtlist_index);
    }
    // TODO: insert follows and uses relationship
    return true;
  } else {
    return false;
  }
}

bool PKB::InsertIfStmt(StmtNumInt stmt_num_int,
                       StmtListIndex parent_stmtlist_index,
                       StmtListIndex then_stmtlist_index,
                       StmtListIndex else_stmtlist_index,
                       VarNameSet control_var_name_set) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, PqlDeclarationEntity::kIf, parent_stmtlist_index)) {
    // insert statement
    stmtlist_table_.InsertStmt(stmt_num, parent_stmtlist_index);
    stmt_type_list_.InsertIfStmt(stmt_num);
    // insert variables
    for (auto& var_name : control_var_name_set) {
      var_list_.InsertVarName(var_name);
    }
    // insert parent relationships
    parent_table_.InsertDirectParentRelationship(stmt_num, then_stmtlist_index);
    parent_table_.InsertDirectParentRelationship(stmt_num, else_stmtlist_index);
    StmtNumList indirect_parents =
        parent_table_.GetParentT(parent_stmtlist_index);
    for (StmtNum& indirect_parent : indirect_parents) {
      parent_table_.InsertIndirectParentRelationship(indirect_parent,
                                                     then_stmtlist_index);
      parent_table_.InsertIndirectParentRelationship(indirect_parent,
                                                     else_stmtlist_index);
    }
    // TODO: insert follows and uses relationship
    return true;
  } else {
    return false;
  }
}

bool PKB::InsertReadStmt(StmtNumInt stmt_num_int, StmtListIndex stmtlist_index,
                         VarName var_name) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, PqlDeclarationEntity::kRead, stmtlist_index)) {
    // insert statement
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertReadStmt(stmt_num);
    // insert variable
    var_list_.InsertVarName(var_name);
    // insert modifies relationship
    StmtNumList parents = parent_table_.GetParentT(stmtlist_index);
    modifies_table_.InsertModifies(stmt_num, var_name);
    for (StmtNum& parent : parents) {
      modifies_table_.InsertModifies(parent, var_name);
    }
    // TODO: insert follows relationship
    return true;
  } else {
    return false;
  }
}

bool PKB::InsertPrintStmt(StmtNumInt stmt_num_int, StmtListIndex stmtlist_index,
                          VarName var_name) {
  StmtNum stmt_num = ToString(stmt_num_int);
  if (stmt_table_.InsertStmt(stmt_num, PqlDeclarationEntity::kPrint, stmtlist_index)) {
    // insert statement
    stmtlist_table_.InsertStmt(stmt_num, stmtlist_index);
    stmt_type_list_.InsertPrintStmt(stmt_num);
    // insert variable
    var_list_.InsertVarName(var_name);
    // TODO: insert follows and uses relationship
    return true;
  } else {
    return false;
  }
}

StmtNumList PKB::GetAllStmt() { return stmt_type_list_.GetAllStmt(); }

StmtNumList PKB::GetAllAssignStmt() {
  return stmt_type_list_.GetAllAssignStmt();
}

StmtNumList PKB::GetAllWhileStmt() { return stmt_type_list_.GetAllWhileStmt(); }

StmtNumList PKB::GetAllIfStmt() { return stmt_type_list_.GetAllIfStmt(); }

StmtNumList PKB::GetAllReadStmt() { return stmt_type_list_.GetAllReadStmt(); }

StmtNumList PKB::GetAllPrintStmt() { return stmt_type_list_.GetAllPrintStmt(); }

bool PKB::IsParent(StmtNumInt parent_stmt_num_int,
                   StmtNumInt child_stmt_num_int) {
  StmtNum parent_stmt_num = ToString(parent_stmt_num_int);
  StmtNum child_stmt_num = ToString(child_stmt_num_int);
  StmtListIndex child_stmtlist_index =
      stmt_table_.GetStmtListIndex(child_stmt_num);
  return parent_table_.IsParent(parent_stmt_num, child_stmtlist_index);
}

bool PKB::IsParentT(StmtNumInt parent_stmt_num_int,
                    StmtNumInt child_stmt_num_int) {
  StmtNum parent_stmt_num = ToString(parent_stmt_num_int);
  StmtNum child_stmt_num = ToString(child_stmt_num_int);
  StmtListIndex child_stmtlist_index =
      stmt_table_.GetStmtListIndex(child_stmt_num);
  return parent_table_.IsParentT(parent_stmt_num, child_stmtlist_index);
}

StmtNumList PKB::GetParent(StmtNumInt stmt_num_int) {
  StmtNum stmt_num = ToString(stmt_num_int);
  StmtListIndex stmtlist_index = stmt_table_.GetStmtListIndex(stmt_num);
  StmtNumList direct_parent_stmtnum_list;
  StmtNum direct_parent_stmtnum = parent_table_.GetParent(stmtlist_index);
  if (direct_parent_stmtnum.compare("") != 0) {
    direct_parent_stmtnum_list.push_back(direct_parent_stmtnum);
  }
  return direct_parent_stmtnum_list;
}

StmtNumList PKB::GetParentT(StmtNumInt stmt_num_int) {
  StmtNum stmt_num = ToString(stmt_num_int);
  StmtListIndex stmtlist_index = stmt_table_.GetStmtListIndex(stmt_num);
  return parent_table_.GetParentT(stmtlist_index);
}

StmtNumList PKB::GetAllParent() {
  ParentsSet parent_stmt_num_set = parent_table_.GetParentsSet();
  StmtNumList parent_stmt_num_list;
  for (auto parent_stmt_num : parent_stmt_num_set) {
    parent_stmt_num_list.push_back(parent_stmt_num);
  }
  return parent_stmt_num_list;
}

StmtNumList PKB::GetChild(StmtNumInt stmt_num_int) {
  StmtNum stmt_num = ToString(stmt_num_int);
  StmtListIndexList children_stmtlist_indices =
      parent_table_.GetChild(stmt_num);
  StmtNumList result;
  for (StmtListIndex& stmtlist_index : children_stmtlist_indices) {
    StmtNumList stmt_num_list = stmtlist_table_.GetStmtNumList(stmtlist_index);
    result.insert(result.end(), stmt_num_list.begin(), stmt_num_list.end());
  }
  return result;
}

StmtNumList PKB::GetChildT(StmtNumInt stmt_num_int) {
  StmtNum stmt_num = ToString(stmt_num_int);
  StmtListIndexList children_stmtlist_indices =
      parent_table_.GetChildT(stmt_num);
  StmtNumList child_stmt_num_list;
  for (StmtListIndex& stmtlist_index : children_stmtlist_indices) {
    StmtNumList stmt_num_list = stmtlist_table_.GetStmtNumList(stmtlist_index);
    child_stmt_num_list.insert(child_stmt_num_list.end(), stmt_num_list.begin(),
                               stmt_num_list.end());
  }
  return child_stmt_num_list;
}

StmtNumList PKB::GetAllChild() {
  ChildrenSet child_stmtlist_index_set = parent_table_.GetChildrenSet();
  StmtNumList child_stmt_num_list;
  for (auto child_stmtlist_index : child_stmtlist_index_set) {
    StmtNumList stmt_num_list =
        stmtlist_table_.GetStmtNumList(child_stmtlist_index);
    child_stmt_num_list.insert(child_stmt_num_list.end(), stmt_num_list.begin(),
                               stmt_num_list.end());
  }
  return child_stmt_num_list;
}

bool PKB::HasParentRelationship() {
  return parent_table_.HasParentRelationship();
}

StmtNumPairList PKB::GetAllParentPair() {
  StmtNumPairList parent_pair_list;
  DirectParentMap parents_map = parent_table_.GetDirectParentMap();
  for (auto entry : parents_map) {
    StmtNumList child_stmt_num_list =
        stmtlist_table_.GetStmtNumList(entry.first);
    for (StmtNum& child_stmt_num : child_stmt_num_list) {
      parent_pair_list.push_back(make_pair(entry.second, child_stmt_num));
    }
  }
  return parent_pair_list;
}

StmtNumPairList PKB::GetAllParentTPair() {
  StmtNumPairList parent_t_pair_list;
  ParentsMap parents_map = parent_table_.GetParentsMap();
  for (auto entry : parents_map) {
    StmtNumList child_stmt_num_list =
        stmtlist_table_.GetStmtNumList(entry.first);
    for (StmtNum& child_stmt_num : child_stmt_num_list) {
      for (StmtNum& parent_stmt_num : entry.second) {
        parent_t_pair_list.push_back(
            make_pair(parent_stmt_num, child_stmt_num));
      }
    }
  }
  return parent_t_pair_list;
}

bool PKB::IsModifiedByS(StmtNumInt stmt_num_int, VarName var_name) {
  return modifies_table_.IsModifiedBy(ToString(stmt_num_int), var_name);
}

bool PKB::IsModifiedByP(ProcName proc_name, VarName var_name) {
  if (proc_name == proc_list_.GetAllProcName().front()) {
    return modifies_table_.IsModified(var_name);
  } else {
    return false;
  }
}

VarNameList PKB::GetModifiedVarS(StmtNumInt stmt_num_int) {
  return modifies_table_.GetModifiedVar(ToString(stmt_num_int));
}

VarNameList PKB::GetModifiedVarP(ProcName proc_name) {
  if (proc_name == proc_list_.GetAllProcName().front()) {
    return modifies_table_.GetAllModifiedVar();
  } else {
    return VarNameList();
  }
}

VarNameList PKB::GetAllModifiedVar() {
  return modifies_table_.GetAllModifiedVar();
}

StmtNumList PKB::GetModifyingS(VarName var_name) {
  return modifies_table_.GetModifyingStmt(var_name);
}

ProcNameList PKB::GetModifyingP(VarName var_name) {
  if (modifies_table_.IsModified(var_name)) {
    return proc_list_.GetAllProcName();
  } else {
    return ProcNameList();
  }
}

StmtNumList PKB::GetAllModifyingS() {
  return modifies_table_.GetAllModifyingStmt();
}

ProcNameList PKB::GetAllModifyingP() {
  if (modifies_table_.HasModifiesRelationship()) {
    return proc_list_.GetAllProcName();
  } else {
    return ProcNameList();
  }
}

bool PKB::HasModifiesRelationship() {
  return modifies_table_.HasModifiesRelationship();
}

StmtVarPairList PKB::GetAllModifiesPairS() {
  return modifies_table_.GetAllModifiesPair();
}

ProcVarPairList PKB::GetAllModifiesPairP() {
  ProcVarPairList proc_var_pair_list;
  ProcName proc_name = proc_list_.GetAllProcName().front();
  for (VarName& var_name : modifies_table_.GetAllModifiedVar()) {
    proc_var_pair_list.push_back(make_pair(proc_name, var_name));
  }
  return proc_var_pair_list;
}

StmtNum PKB::ToString(int stmt_num_int) {
  stringstream stmt_num_ss;
  stmt_num_ss << stmt_num_int;
  return stmt_num_ss.str();
}
