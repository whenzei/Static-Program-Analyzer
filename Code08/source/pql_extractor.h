#pragma once

#ifndef PQL_EXTRACTOR_H
#define PQL_EXTRACTOR_H

#include "affects_extractor.h"
#include "next_extractor.h"

class PqlExtractor {
  AffectsExtractor ae_;
  NextExtractor ne_;

 public:
  PqlExtractor(PKB* pkb);

  //**************** Next* *********************************

  // @returns true if Next*(previous_stmt, next_stmt) holds
  bool IsNextT(StmtNum previous_stmt, StmtNum next_stmt);

  // @returns true if Next*(_, stmt_num) holds
  bool IsNextT(StmtNum stmt_num);

  // @returns true if Next*(stmt_num, _) holds
  bool IsPreviousT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Next*(stmt_num, n)
  StmtNumList GetNextT(StmtNum stmt_num);

  // @returns a list of all n's that satisfy Next*(n, stmt_num)
  StmtNumList GetPreviousT(StmtNum stmt_num);

  // @returns a list of all pairs of <n1, n2> that satisfy Next*(n1, n2)
  StmtNumPairList GetAllNextTPairs();

  //****************** Affects *******************************

  // @returns true if Affects(stmt_1, stmt_2) holds, else false
  bool IsAffects(StmtNum stmt_1, StmtNum stmt_2);

  // @returns true if stmt_num affects any statement
  bool IsAffects(StmtNum stmt_num);

  // @returns true if stmt_num is affected by any statement
  bool IsAffected(StmtNum stmt_num);

  // @returns a list of n that Affects(stmt_1, n) holds true
  StmtNumList GetAffects(StmtNum stmt_1);

  // @returns a list of n that Affects(n, stmt_num) holds true
  StmtNumList GetAffectedBy(StmtNum stmt_num);

  // @returns set of all statements stmt_num such that Affects(stmt_num, _)
  // holds true
  VertexSet GetAllAffects();

  // @returns set of all statements stmt_num such that Affects(_, stmt_num)
  // holds true
  VertexSet GetAllAffectedBy();

  // Get the AffectsTable of the whole program
  // @returns a hashmap of <key> StmtNum <value> set of all affected StmtNums
  AffectsTable GetAffectsTable();

  // Get the AffectedByTable of the whole program
  // @returns a hashmap of <key> StmtNum <value> set of all affecting StmtNums
  AffectsTable GetAffectedByTable();
  //*********************************************************

  //****************** AffectsBip *******************************

  // @returns true if AffectsBip(stmt_1, stmt_2) holds, else false
  bool IsAffectsBip(StmtNum stmt_1, StmtNum stmt_2);

  // @returns true if there is any AffectsBip(stmt_num, _ ) relation that holds,
  // else false
  bool IsAffectsBip(StmtNum stmt_num);

  // @returns true if there is any AffectsBip(_,stmt_num ) relation that holds,
  // else false
  bool IsAffectedBip(StmtNum stmt_num);

  // @returns a list of n that AffectsBip(stmt_1, n) holds true
  StmtNumList GetAffectsBip(StmtNum stmt_1);

  // @returns a list of n that Affects(n, stmt_num) holds true
  StmtNumList GetAffectedByBip(StmtNum stmt_num);

  // Get the AffectsBipTable of the whole program
  // @returns a hashmap of <key> StmtNum <value> set of all affectedBip StmtNums
  AffectsTable GetAffectsBipTable();

  // Get the AffectedByBipTable of the whole program
  // @returns a hashmap of <key> StmtNum <value> set of all affectingBip StmtNums
  AffectsTable GetAffectedByBipTable();
};

#endif  // !PQL_EXTRACTOR_H
