#pragma once

#ifndef SPA_PKB_H
#define SPA_PKB_H

// forward declaration due to circular dependencies
class AssignStmtData;
class WhileStmtData;
class ReadStmtData;
class PrintStmtData;
class IfStmtData;
class CallStmtData;
class StatementData;

#include "call_table.h"
#include "const_list.h"
#include "follows_table.h"
#include "graph.h"
#include "modifies_table.h"
#include "next_table.h"
#include "parent_table.h"
#include "pattern_table.h"
#include "pql_global.h"
#include "proc_list.h"
#include "stmt_table.h"
#include "stmt_type_list.h"
#include "stmtlist_table.h"
#include "uses_table.h"
#include "var_list.h"
// statement_data.h must be at the end due to forward declaration
#include "statement_data.h"

using StmtNumInt = int;
using CallGraph = Graph;

class PKB {
  ProcList proc_list_;
  VarList var_list_;
  ConstList const_list_;
  StmtTable stmt_table_;
  StmtListTable stmtlist_table_;
  StmtTypeList stmt_type_list_;
  FollowsTable follows_table_;
  ParentTable parent_table_;
  PatternTable pattern_table_;
  ModifiesTable modifies_table_;
  UsesTable uses_table_;
  NextTable next_table_;
  CallGraph call_graph_;
  CallTable call_table_;

 public:
  // inserts the given procedure name into the procedure list
  // @param proc_name the procedure name to be inserted
  void InsertProcName(ProcName proc_name);

  // get all procedure names stored inside procedure list
  // @returns the list of procedure names (can be empty)
  ProcNameList GetAllProcName();

  // @returns the list of <p, p> (same p repeated twice)
  ProcNamePairList GetAllProcNameTwin();

  // get all variable names stored inside variable list
  // @returns the list of variable names (can be empty)
  VarNameList GetAllVarName();

  // @returns the list of <v, v> (same v repeated twice)
  VarNamePairList GetAllVarNameTwin();

  // get all constant values stored inside constant list
  // @returns the list of constant values (can be empty)
  ConstValueList GetAllConstValue();

  // @returns the list of <c, c> (same c repeated twice)
  ConstValuePairList GetAllConstValueTwin();

  // @returns a PqlDeclarationEntity enum to represent the statement type
  // (assign/while/if/read/print)
  StmtType GetStmtType(StmtNum stmt_num);

  // @returns pointer to the call graph of the program
  CallGraph* GetCallGraph();

  // inserts an edge from the node referring to given curr_proc_name to the node
  // referring to given called_proc_name in the call graph
  void InsertEdgeInCallGraph(ProcName curr_proc_name,
                             ProcName called_proc_name);

  // @returns toposorted order of procedure calls in a ProcNameList
  ProcNameList GetToposortedCalls();

  // inserts the given assign statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param AssignStmtData the encapsulation of statement data for PKB use
  void InsertAssignStmt(AssignStmtData*);

  // inserts the given while statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param stmt_data the statement data as encapsulated by WhileStmtData
  // structure
  void InsertWhileStmt(WhileStmtData*);

  // inserts the given if statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param stmt_data the statement data as encapsulated by IfStmtData structure
  void InsertIfStmt(IfStmtData* stmt_data);

  // inserts the given read statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param stmt_data the statement data as encapsulated by ReadStmtData
  // structure
  void InsertReadStmt(ReadStmtData* stmt_data);

  // inserts the given print statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param stmt_data the statement data as encapsulated by PrintStmtData
  // structure
  void InsertPrintStmt(PrintStmtData* stmt_data);

  // Inserts the given call statement into the StmtTable, StmtTypeList and
  // StmtListTable
  // @param stmt_data the stmt data as encapsulated by the CallStmtData
  // structure
  void InsertCallStmt(CallStmtData* stmt_data);

  // Inserts a follows relationship between followee and follower
  void InsertFollows(StmtNum followee, StmtNum follower);

  // Inserts a modifies relationship between modifying_stmt and modified_var
  void InsertModifiesS(StmtNum modifying_stmt, VarName modified_var);

  // Inserts a modifies relationship between modifying_proc and modified_var
  void InsertModifiesP(ProcName modifying_proc, VarName modified_var);

  // Inserts a uses relationship between using_stmt and used_var
  void InsertUsesS(StmtNum using_stmt, VarName used_var);

  // Inserts a uses relationship between using_proc and used_var
  void InsertUsesP(ProcName using_proc, VarName used_var);

  // Inserts a direct parent relationship between parent_stmt_num and
  // child_stmt_num
  void InsertParent(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // Inserts an indirect parent relationship between parent_stmt_num and
  // child_stmt_num
  void InsertParentT(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // Inserts a next relationship between previous_stmt and next_stmt
  void InsertNext(ProcName proc_name, StmtNumInt previous_stmt, StmtNumInt next_stmt);

  // get statement numbers for all statements stored inside stmt type list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllStmt();

  // @returns the list of <s, s> (same s repeated twice)
  StmtNumPairList GetAllStmtTwin();

  // get statement numbers for all assign statements stored inside stmt type
  // list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllAssignStmt();

  // @returns the list of <a, a> (same a repeated twice)
  StmtNumPairList GetAllAssignStmtTwin();

  // get statement numbers for all while statements stored inside stmt type list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllWhileStmt();

  // @returns the list of <w, w> (same w repeated twice)
  StmtNumPairList GetAllWhileStmtTwin();

  // get statement numbers for all if statements stored inside stmt type list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllIfStmt();

  // @returns the list of <ifs, ifs> (same ifs repeated twice)
  StmtNumPairList GetAllIfStmtTwin();

  // get statement numbers for all read statements stored inside stmt type list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllReadStmt();

  // @returns the list of <r, r> (same r repeated twice)
  StmtNumPairList GetAllReadStmtTwin();

  // get statement numbers for all print statements stored inside stmt type list
  // @returns the list of statement numbers(can be empty)
  StmtNumList GetAllPrintStmt();

  // @returns the list of <pr, pr> (same pr repeated twice)
  StmtNumPairList GetAllPrintStmtTwin();

  // get stmt numbers for all call stmts inside the stmt type list
  // @returns the list of stmt numbers
  StmtNumList GetAllCallStmt();

  // @returns the list of <call, call> (same call repeated twice)
  StmtNumPairList GetAllCallStmtTwin();

  /***********************
 * Is-A Functions *
 ***********************/
  // @returns true if var_name is a variable in the variable list
  bool IsVarName(VarName var_name);

  // @returns true if stmt_num is a statement in the statement list
  bool IsStmtNum(StmtNum stmt_num);

  // @returns true if proc_name is a procedure in the procedure list
  bool IsProcName(ProcName proc_name);

  // @returns true if const_value is a constant in the constant list
  bool IsConstValue(ConstValue const_value);

  // @returns true if the given stmt_num is an assign statement
  bool IsAssignStmt(StmtNum stmt_num);

  // @returns true if the given stmt_num is a while statement
  bool IsWhileStmt(StmtNum stmt_num);

  // @returns true if the given stmt_num is an if statement
  bool IsIfStmt(StmtNum stmt_num);

  // @returns true if the given stmt_num is a read statement
  bool IsReadStmt(StmtNum stmt_num);

  // @returns true if the given stmt_num is a print statement
  bool IsPrintStmt(StmtNum stmt_num);

  // @returns true if the given stmt_num is a call statement
  bool IsCallStmt(StmtNum stmt_num);

  /***********************
   * Follows Table Functions *
   ***********************/

  // @returns true if Follows*(followee, follower) holds
  bool IsFollowsT(StmtNum followee_stmt_num, StmtNum follower_stmt_num);

  // @returns true if Follows(followee, follower) holds
  bool IsFollows(StmtNum followee_stmt_num, StmtNum follower_stmt_num);

  // @returns a list of all n's that satisfy Follows*(stmt_num, n)
  StmtNumList GetFollowsT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Follows(stmt_num, n)
  StmtNumList GetFollows(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Follows(_, n)
  StmtNumList GetAllFollows();

  // @returns a list of all n's that satisfy Follows*(n, stmt_num)
  StmtNumList GetFollowedByT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Follows(n, stmt_num)
  StmtNumList GetFollowedBy(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Follows*(n, _)
  StmtNumList GetAllFollowedBy();

  // @returns true if Follows(_, _) holds
  bool HasFollowsRelationship();

  // @returns list of all pairs of <s1, s2> that satisfy Follows*(s1, s2)
  StmtNumPairList GetAllFollowsTPair();

  // @returns list of all pairs of <s1, s2> that satisfy Follows(s1, s2)
  StmtNumPairList GetAllFollowsPair();

  /***********************
   * Parent Table Functions *
   ***********************/

  // @returns true if Parent(parent_stmt_num, child_stmt_num) holds
  bool IsParent(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // @returns true if Parent*(parent_stmt_num, child_stmt_num) holds
  bool IsParentT(StmtNum parent_stmt_num, StmtNum child_stmt_num);

  // @returns a list of all n's that satisfy Parent(n, stmt_num)
  // only one element
  StmtNumList GetParent(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Parent*(n, stmt_num)
  StmtNumList GetParentT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Parent(n, _)/Parent*(n, _)
  StmtNumList GetAllParent();

  // @returns a list of all n's that satisfy Parent(stmt_num, n)
  StmtNumList GetChild(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Parent*(stmt_num, n)
  StmtNumList GetChildT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Parent(_, n)
  StmtNumList GetAllChild();

  // @returns true if Parent(_, _) holds
  bool HasParentRelationship();

  // @returns list of all pairs of <s1, s2> that satisfy Parent(s1, s2)
  StmtNumPairList GetAllParentPair();

  // @returns list of all pairs of <s1, s2> that satisfy Parent*(s1, s2)
  StmtNumPairList GetAllParentTPair();

  /***********************
   * Modifies Table Functions *
   ***********************/

  // @returns true if Modifies(stmt_num, var_name) holds
  bool IsModifiedByS(StmtNum stmt_num, VarName var_name);

  // @returns true if Modifies(proc_name, var_name) holds
  bool IsModifiedByP(ProcName proc_name, VarName var_name);

  // @returns a list of all n's that satisfy Modifies(stmt_num, n)
  VarNameList GetModifiedVarS(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Modifies(proc_name, n)
  VarNameList GetModifiedVarP(ProcName proc_name);

  // @returns a list of all stmt_num's that satisfy Modifies(stmt_num, var_name)
  StmtNumList GetModifyingS(VarName var_name);

  // @returns a list of all proc_name's that satisfy Modifies(proc_name,
  // var_name)
  ProcNameList GetModifyingP(VarName var_name);

  // @returns a list of all stmt_num's that satisfy Modifies(stmt_num, _)
  StmtNumList GetAllModifyingS();

  // @returns a list of all proc_name's that satisfy Modifies(proc_name,
  // _)
  ProcNameList GetAllModifyingP();

  // @returns a list of all pairs of <stmt_num, var_name> that satisfy
  // Modifies(stmt_num, var_name)
  StmtVarPairList GetAllModifiesPairS();

  // @returns a list of all pairs of <proc_name, var_name> that satisfy
  // Modifies(proc_name, var_name)
  ProcVarPairList GetAllModifiesPairP();

  /***********************
   * Uses Table Functions *
   ***********************/

  // @returns a list of all n's that satisfy Uses(stmt_num, n)
  VarNameList GetUsedVarS(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Uses(proc_name, n)
  VarNameList GetUsedVarP(ProcName proc_name);

  // @returns a list of all stmt_num's that satisfy Uses(stmt_num, _)
  StmtNumList GetAllUsingStmt();

  // @returns a list of all proc_name's that satisfy Uses(proc_name, _)
  ProcNameList GetAllUsingProc();

  // @returns a list of all stmt_num's that satisfy Uses(stmt_num, var_name)
  StmtNumList GetUsingStmt(VarName var_name);

  // @returns a list of all proc_name's that satisfy Uses(proc_name, var_name)
  ProcNameList GetUsingProc(VarName var_name);

  // @returns true if Uses(stmt_num, var_name) holds
  bool IsUsedByS(StmtNum stmt_num, VarName var_name);

  // @returns true if Uses(proc_name, var_name) holds
  bool IsUsedByP(ProcName proc_name, VarName var_name);

  // @returns a list of all pairs of <stmt_num, var_name> that satisfy
  // Uses(stmt_num, var_name)
  StmtVarPairList GetAllUsesPairS();

  // @returns a list of all pairs of <proc_name, var_name> that satisfy
  // Uses(proc_name, var_name)
  ProcVarPairList GetAllUsesPairP();

  // @returns a list of a's that satisfy pattern a(var_name, _sub_expr_)
  // var_name can be an empty string, sub_expr can be an empty TokenList (to
  // represent underscore)
  StmtNumList GetAssignWithPattern(VarName var_name, TokenList sub_expr);

  // @returns a list of a's that satisfy pattern a(var_name, exact_expr)
  // var_name can be an empty string (to represent underscore)
  StmtNumList GetAssignWithExactPattern(VarName var_name, TokenList exact_expr);

  // @returns a list of all pairs of <a, v> that satisfy pattern a(v,
  // _sub_expr_)
  StmtVarPairList GetAllAssignPatternPair(TokenList sub_expr);

  // @returns a list of all pairs of <a, v> that satisfy pattern a(v,
  // exact_expr)
  StmtVarPairList GetAllAssignExactPatternPair(TokenList exact_expr);

  // @returns a list of w's that satisfy pattern w(var_name, _)
  // var_name can be an empty string to represent underscore
  StmtNumList GetWhileWithPattern(VarName var_name);

  // @returns a list of all pairs of <w, v> that satisfy pattern w(v, _)
  StmtVarPairList GetAllWhilePatternPair();

  // @returns a list of ifs' that satisfy pattern ifs(var_name, _)
  // var_name can be an empty string to represent underscore
  StmtNumList GetIfWithPattern(VarName var_name);

  // @returns a list of all pairs of <ifs, v> that satisfy pattern ifs(v, _)
  StmtVarPairList GetAllIfPatternPair();

  /***********************
   * Call Table Functions *
   ***********************/

  // Inserts an indirect caller, callee pair relationship into the Call Table.
  // @returns true if insertion is successful, false otherwise
  // @params caller procedure name and callee procedure name
  bool InsertIndirectCallRelationship(ProcName caller_proc,
                                      ProcName callee_proc);

  // Inserts a direct caller, callee pair relationship into the Call Table.
  // @returns true if insertion is successful, false otherwise
  // @params caller procedure name and callee procedure name
  bool InsertDirectCallRelationship(ProcName caller_proc, ProcName callee_proc);

  // Inserts a calls relationship to the call table.
  // @params stmt num of statement
  // @params proc name of the procedure *being called*
  void InsertCalls(StmtNum stmt_num, ProcName callee_proc);

  // Finds and returns a list of stmt numbers calling the given proc.
  // @params proc name of the procedure being called
  // @returns a list of stmt numbers (can be empty)
  StmtNumList GetCallingStmts(ProcName callee_proc);

  // Finds and returns a list of pairs of all stmt numbers
  // and procedures called by that stmt number.
  // @returns a list of <stmt_num, proc> pairs
  StmtNumProcPairList GetAllCallingStmtPairs();

  // Finds and returns the direct callee for given procedure.
  // @returns a list containing one direct callee (can be empty)
  // @params caller procedure name
  ProcNameList GetCallee(ProcName caller_proc);

  // Finds and returns all callees for given procedure.
  // @returns a list containing all callees for given proc (can be empty)
  // @params caller procedure name
  ProcNameList GetCalleeT(ProcName caller_proc);

  // Finds and returns the direct caller for given procedure.
  // @returns a list containing one direct caller (can be empty)
  // @params callee procedure name
  ProcNameList GetCaller(ProcName callee_proc);

  // Finds and returns all callers for given procedure.
  // @returns a list containing all callers for given proc (can be empty)
  // @params callee procedure name
  ProcNameList GetCallerT(ProcName callee_proc);

  // @returns all procedures calling some other proc (can be empty)
  ProcNameList GetAllCaller();

  // @returns all procedures being called by some other proc (can be empty)
  ProcNameList GetAllCallee();

  // @returns all procedures being called by some other proc (in pairs)
  ProcNamePairList GetAllCalleeTwin();

  // @returns a list of all <caller, direct callee> pairs
  ProcNamePairList GetAllCallPairs();

  // @returns a list of all <caller, callee> pairs
  ProcNamePairList GetAllCallTPairs();

  // @returns true if Call(caller, callee) is true
  bool IsCall(ProcName caller_proc, ProcName callee_proc);

  // @returns true if Call*(caller, callee) is true
  bool IsCallT(ProcName caller_proc, ProcName callee_proc);

  // @returns true if callee is called by any other proc

  bool IsCalledProc(ProcName callee_proc);

  // @returns true if Call Table has any calls relationships
  // false if otherwise
  bool HasCallsRelationship();

  // @returns true if Next(previous_stmt, next_stmt) holds
  bool IsNext(StmtNum previous_stmt, StmtNum next_stmt);

  // @returns true if Next(_, stmt_num) holds
  bool IsNext(StmtNum stmt_num);

  // @returns true if Next(stmt_num, _) holds
  bool IsPrevious(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Next(stmt_num, n)
  StmtNumList GetNext(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Next(n, stmt_num)
  StmtNumList GetPrevious(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Next(_, n)
  StmtNumList GetAllNext();

  // @returns a list of all n's that satisfy Next(n, _)
  StmtNumList GetAllPrevious();

  // @returns a list of all pairs of <n1, n2> that satisfy Next(n1, n2)
  StmtNumPairList GetAllNextPairs();

  // @returns true if Next(_, _) holds
  bool HasNextRelationship();

  // @returns the cfg belonging to a specified procedure
  CFG* GetCFG(ProcName proc_name);

  // Parser calls this method to notify pkb end of parse.
  // PKB will proceed with design extraction
  void NotifyParseEnd();

 private:
  bool HandleInsertStatement(StatementData* stmt_data, StmtType stmt_type);
  void HandleInsertVariables(VarName variable, VarNameSet var_set);
  void HandleInsertVariables(VarNameSet var_set);
  void HandleInsertVariable(VarName variable);
  void HandleInsertConstants(ConstValueSet constants);
  void HandleInsertPattern(StmtType stmt_type, void* stmt_data);
};

#endif  // !SPA_PKB_H
