#pragma once

#ifndef QUERY_EVALUATOR_H
#define QUERY_EVALUATOR_H

#include <list>
#include <string>
#include <unordered_map>

#include "pkb.h"
#include "pql_evaluator.h"
#include "pql_query.h"

using std::list;
using std::string;
using std::unordered_map;
using QueryResultList = list<string>;

/*A class to evaluate user query and return result to user*/
class PqlEvaluator {
 private:
  PqlDeclarationEntity
      select_type_;  // the type of the variable in 'Select' statement
  PKB pkb_;          // pkb database
  bool clause_flag_;
  PqlQuery pql_query_;

 public:
  /* Contructor */
  PqlEvaluator();

  /* Setter */
  void SetQuery(PqlQuery);
  void SetSelectType(PqlDeclarationEntity);
  void SetPKB(PKB);
  void SetClauseFlag(bool);

  /* Getter */
  PqlQuery GetQuery();
  PqlDeclarationEntity GetSelectType();
  PKB GetPKB();
  bool GetClauseFlag();

  /**
   * Use the @Query provided by user
   * and return a list of results based on the query
   * @param query the query by the user
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  QueryResultList GetResultFromQuery(PqlQuery* query, PKB pkb);

  /**
   * Return a list of results based on the query. This method will only be
   * used when there are no "such that" or "pattern" clause.
   * @input declaration entities of select variable and pkb
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  QueryResultList GetSelectAllResult(PqlDeclarationEntity select_type);

  /**
   * Return a list of results based on the query. This method will only be
   * used when there are "such that" clause.
   * @input such that clause provided by user
   * @returns a list of string if there is result,
   * or an empty list otherwise
   */
  QueryResultList GetSuchThatResult(PqlSuchthat suchthat);

  /**
   * Evaluate such that follows
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  QueryResultList EvaluateFollows(PqlDeclarationEntity select_type,
                                  PqlSuchthat suchthat,
                                  SuchthatParamType arrangement);

  /**
   * Evaluate such that follows*
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  QueryResultList EvaluateFollowsT(PqlDeclarationEntity select_type,
                                   PqlSuchthat suchthat,
                                   SuchthatParamType arrangement);

  /**
   * Evaluate such that parent
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  QueryResultList EvaluateParent(PqlDeclarationEntity select_type,
                                 PqlSuchthat suchthat,
                                 SuchthatParamType arrangement);

  /**
   * Evaluate such that parent*
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  QueryResultList EvaluateParentT(PqlDeclarationEntity select_type,
                                  PqlSuchthat suchthat,
                                  SuchthatParamType arrangement);

  /**
   * Evaluate such that uses stmt
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  QueryResultList EvaluateUsesS(PqlDeclarationEntity select_type,
                                PqlSuchthat suchthat,
                                SuchthatParamType arrangement);

  /**
   * Evaluate such that uses procedure
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  QueryResultList EvaluateUsesP(PqlDeclarationEntity select_type,
                                PqlSuchthat suchthat,
                                SuchthatParamType arrangement);

  /**
   * Evaluate such that modify stmt
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  QueryResultList EvaluateModifiesS(PqlDeclarationEntity select_type,
                                    PqlSuchthat suchthat,
                                    SuchthatParamType arrangement);

  /**
   * Evaluate such that modify procedure
   * @input select type and such that clause provided by user
   * @returns results based on evaluation
   */
  QueryResultList EvaluateModifiesP(PqlDeclarationEntity select_type,
                                    PqlSuchthat suchthat,
                                    SuchthatParamType arrangement);

  /**
   * Determine the declaration type of the select variable
   * @input select clause and declaration entities provided by user
   * @returns PqlDeclarationEntity of the select variable
   */
  PqlDeclarationEntity CheckSelectDeclarationType(string select_var_name);

  /**
   * Determine the number of synonym in the such that param (e.g such that
   * Follows(a,b) => 2 entity) also determines whether the synonym in the param
   * is a select variable (e.g Select a)
   * @input left and right param of the such that clause
   * @returns properties of synonym in the form of
   * PqlPropertyOfEntityInSuchthatParam
   */
  SuchthatParamType CheckSuchthatParamType(string select_var_name,
                                           Parameters such_that_param);

  /**
   * Filter the result list based on the selection entity type
   * @input unfiltered result list and selection entity type (e.g assign)
   * @returns result list that only contains result of a certain entity type
   */
  QueryResultList FilterResult(list<string> unfiltered_result,
                               PqlDeclarationEntity select_type);

  /**
   * Filter the result list based on the variable entity type
   * @input unfiltered result list and variable entity type (e.g constant)
   * @returns result list that only contains result of a certain entity type
   */
  QueryResultList FilterVariableResult(list<string> unfiltered_result,
                                       PqlDeclarationEntity variable_type);

  /**
   * Filter the result pair list based on the selection entity type and filter
   * type
   * @input the filter type, unfiltered result list and selection entity type
   * (e.g assign) for left and right of the pair
   * @returns result pair list that only contains result of a certain entity
   * type
   */
  list<pair<string, string>> FilterPairResult(
      PqlResultFilterType filter_type,
      list<pair<string, string>> unfiltered_pair_result,
      PqlDeclarationEntity left_type, PqlDeclarationEntity right_type);

  /**
   * Get all the result from left of the pair and return it in a list
   * @input the list of pairs
   * @returns list of result in the left of the pair
   */
  QueryResultList GetAllLeftOfPair(list<pair<string, string>> filtered_list);

  /**
   * Get all the result from right of the pair and return it in a list
   * @input the list of pairs
   * @returns list of result in the right of the pair
   */
  QueryResultList GetAllRightOfPair(list<pair<string, string>> filtered_list);
};

#endif  // !QUERY_EVALUATOR_H