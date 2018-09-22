#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"
#include "pql_enum.h"
#include "pql_result.h"

#include <list>
#include <string>
#include <unordered_set>
#include <vector>

using std::pair;
using std::string;
using std::unordered_map;
using std::vector;
using ColumnHeader = unordered_map<string, int>;
using ResultTable = vector<vector<string>>;
using ResultRow = vector<string>;
using QueryResultList = vector<string>;
using QueryResultPairList = vector<pair<string, string>>;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PQLTests {
TEST_CLASS(TestPqlResult) {
 public:
  QueryResultList test_result_list;
  QueryResultList test_merge_list;
  QueryResultPairList test_resultpair_list;

  TEST_METHOD(TestInitTable) {
    PqlResult pql_result;

    test_result_list.push_back("1");
    test_result_list.push_back("2");
    test_result_list.push_back("3");

    pql_result.InitTable(test_result_list);

    ResultTable result_table = pql_result.GetResultTable();

    // Should have three rows
    Assert::IsTrue(result_table.size() == 3);

    string expected_result1 = "1";
    string expected_result3 = "3";

    // First row should contain "1"
    Assert::AreEqual(expected_result1, result_table[0][0]);
    // Last row should contain "3"
    Assert::AreEqual(expected_result3, result_table[2][0]);
  }

  TEST_METHOD(TestInitPairTable) {
    PqlResult pql_result;

    test_resultpair_list.push_back(std::make_pair("1", "a"));
    test_resultpair_list.push_back(std::make_pair("2", "b"));
    test_resultpair_list.push_back(std::make_pair("3", "c"));

    pql_result.InitTable(test_resultpair_list);

    ResultTable result_table = pql_result.GetResultTable();

    // Should have three rows
    Assert::IsTrue(result_table.size() == 3);

    string expected_result1 = "1";
    string expected_result3 = "3";
    string expected_resulta = "a";
    string expected_resultc = "c";

    // First row first col should contain "1"
    Assert::AreEqual(expected_result1, result_table[0][0]);
    // First row sec col should contain "a"
    Assert::AreEqual(expected_resulta, result_table[0][1]);
    // Last row first col should contain "3"
    Assert::AreEqual(expected_result3, result_table[2][0]);
    // Last row sec col should contain "c"
    Assert::AreEqual(expected_resultc, result_table[2][1]);
  }

  TEST_METHOD(TestMergeTableNoConflict) {
    PqlResult pql_result;

    test_result_list.push_back("1");
    test_result_list.push_back("2");
    test_result_list.push_back("3");

    pql_result.InitTable(test_result_list);

    test_merge_list.push_back("a");
    test_merge_list.push_back("b");
    test_merge_list.push_back("c");

    pql_result.MergeResults(test_merge_list, kNoConflict, -1);

    ResultTable result_table = pql_result.GetResultTable();

    // Should have nine rows
    Assert::IsTrue(result_table.size() == 9);

    string expected_result1 = "1";
    string expected_result3 = "3";
    string expected_resulta = "a";
    string expected_resultb = "b";
    string expected_resultc = "c";

    // First row first col should contain "1"
    Assert::AreEqual(expected_result1, result_table[0][0]);
    // First row sec col should contain "a"
    Assert::AreEqual(expected_resulta, result_table[0][1]);
    // Sec row first col should contain "1"
    Assert::AreEqual(expected_result1, result_table[1][0]);
    // Sec row sec col should contain "b"
    Assert::AreEqual(expected_resultb, result_table[1][1]);
    // Last row first col should contain "3"
    Assert::AreEqual(expected_result3, result_table[8][0]);
    // Last row sec col should contain "c"
    Assert::AreEqual(expected_resultc, result_table[8][1]);
  }

  TEST_METHOD(TestMergeTableWithConflict) {
    PqlResult pql_result;

    test_resultpair_list.push_back(std::make_pair("1", "a"));
    test_resultpair_list.push_back(std::make_pair("2", "b"));
    test_resultpair_list.push_back(std::make_pair("3", "c"));
    test_resultpair_list.push_back(std::make_pair("4", "d"));
    test_resultpair_list.push_back(std::make_pair("5", "e"));

    pql_result.InitTable(test_resultpair_list);

    test_merge_list.push_back("1");
    test_merge_list.push_back("5");

	//Conflict with column 0
    pql_result.MergeResults(test_merge_list, kConflict, 0);

    ResultTable result_table = pql_result.GetResultTable();

    // Should have two rows
    Assert::IsTrue(result_table.size() == 2);

    string expected_result1 = "1";
    string expected_result5 = "5";
    string expected_resulta = "a";
    string expected_resulte = "e";

    // First row first col should contain "1"
    Assert::AreEqual(expected_result1, result_table[0][0]);
    // First row sec col should contain "a"
    Assert::AreEqual(expected_resulta, result_table[0][1]);
    // Sec row first col should contain "5"
    Assert::AreEqual(expected_result5, result_table[1][0]);
    // Sec row sec col should contain "e"
    Assert::AreEqual(expected_resulte, result_table[1][1]);
  }
};
}  // namespace PQLTests