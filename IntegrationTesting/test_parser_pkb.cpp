#include "stdafx.h"
#include "CppUnitTest.h"
#include "parser.h"
#include "pkb.h"

#include <list>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using tt = Tokenizer::TokenType;

namespace PKBPQLTests {
	TEST_CLASS(TestParserPkb) {

		public: 

			//Include both Follows and Follows* tests
			TEST_METHOD(TestFollowsOfStatementsOnly){
				
				PKB test_pkb = PKB();
				Parser parser = Parser(&test_pkb);
				string program = "procedure one {\n a = b;\n c = d;\n e = f;\n }";
				TokenList tokenized_program = Tokenizer::Tokenize(program);
				
				parser.Parse(tokenized_program);

				PKB true_pkb = PKB();
				true_pkb.InsertFollows("1", "2");
				true_pkb.InsertFollows("1", "3");
				true_pkb.InsertFollows("2", "3");

				//*********************************

				StmtNumList stmt_nums_test_get_all_followed_by = test_pkb.GetAllFollowedBy();
				StmtNumList stmt_nums_true_get_all_followed_by = true_pkb.GetAllFollowedBy();

				StmtNumList::iterator iter_1 = stmt_nums_test_get_all_followed_by.begin(); 
				StmtNumList::iterator iter_2 = stmt_nums_true_get_all_followed_by.begin(); 
				
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1, *iter_2);

				//***********************************

				StmtNumList stmt_nums_test_get_all_follows = test_pkb.GetAllFollows();
				StmtNumList stmt_nums_true_get_all_follows = true_pkb.GetAllFollows();

				iter_1 = stmt_nums_test_get_all_follows.begin(); 
				iter_2 = stmt_nums_true_get_all_follows.begin(); 
				
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1, *iter_2);

				//*************************************

				StmtNumList stmt_num_test_get_follows = test_pkb.GetFollows("2");
				StmtNumList stmt_num_true_get_follows = true_pkb.GetFollows("2");

				iter_1 = stmt_num_test_get_follows.begin();
				iter_2 = stmt_num_true_get_follows.begin();

				//Should only be statement number 3
				Assert::AreEqual(*iter_1, *iter_2);

				//*************************************

				StmtNumList stmt_num_test_get_follows_t = test_pkb.GetFollowsT("1");
				StmtNumList stmt_num_true_get_follows_t = true_pkb.GetFollowsT("1");

				iter_1 = stmt_num_test_get_follows_t.begin();
				iter_2 = stmt_num_true_get_follows_t.begin();

				//Should have 2 statements
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1, *iter_2);

		}

			//Include both Follows and Follows* tests
		TEST_METHOD(TestFollowsOfOneNestedStatements) {

					PKB test_pkb = PKB();
					Parser parser = Parser(&test_pkb);
					string program = "	procedure one {\n"
                      "a = b;\n"
                      "if (a > b) then {\n"                  
																								"e = f;\n"                      
																								"asd = ddd;\n"                      
                      "}\n"
																						"else {\n"
                        "k = 3;\n"
                      "}\n"
																						"while (a < 10) {\n"
																									"a = a + 1;\n"
																							"}\n"
																						"}\n";

					TokenList tokenized_program = Tokenizer::Tokenize(program);

					parser.Parse(tokenized_program);

					PKB true_pkb = PKB();
					true_pkb.InsertFollows("3", "4");
					true_pkb.InsertFollows("1", "2");
					true_pkb.InsertFollows("1", "6");
					true_pkb.InsertFollows("2", "6");

				//*************************************

				StmtNumList stmt_nums_test_get_all_followed_by = test_pkb.GetAllFollowedBy();
				StmtNumList stmt_nums_true_get_all_followed_by = true_pkb.GetAllFollowedBy();

				StmtNumList::iterator iter_1 = stmt_nums_test_get_all_followed_by.begin(); 
				StmtNumList::iterator iter_2 = stmt_nums_true_get_all_followed_by.begin(); 
				
				// Should have 3 followed by stmt_num
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1, *iter_2);

				//**************************************

				StmtNumList stmt_nums_test_get_all_follows = test_pkb.GetAllFollows();
				StmtNumList stmt_nums_true_get_all_follows = true_pkb.GetAllFollows();

				iter_1 = stmt_nums_test_get_all_follows.begin(); 
				iter_2 = stmt_nums_true_get_all_follows.begin(); 
				
				// Should have 3 follows stmt_num
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1, *iter_2);

				//**************************************

				StmtNumList stmt_num_test_get_follows = test_pkb.GetFollows("1");
				StmtNumList stmt_num_true_get_follows = true_pkb.GetFollows("1");

				iter_1 = stmt_num_test_get_follows.begin();
				iter_2 = stmt_num_true_get_follows.begin();

				// Should have 1 follows stmt_num
				Assert::AreEqual(*iter_1, *iter_2);

				//*************************************

				StmtNumList stmt_num_test_get_follows_t = test_pkb.GetFollowsT("1");
				StmtNumList stmt_num_true_get_follows_t = true_pkb.GetFollowsT("1");

				iter_1 = stmt_num_test_get_follows_t.begin();
				iter_2 = stmt_num_true_get_follows_t.begin();

				// Should have 2 follows stmt_num
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1, *iter_2);

		}
		TEST_METHOD(TestParentOfOneNestedStatements) {
				PKB test_pkb = PKB();
				Parser parser = Parser(&test_pkb);
				string program = "	procedure one {\n"					
                      "a = b;\n"														//1
                      "if (a > b) then {\n"   //2               
																								"e = f;\n"            //3          
																								"asd = ddd;\n"        //4              
                      "}\n"
																						"else {\n"
                        "k = 3;\n"												//5
                      "}\n"																				
																						"while (a < 10) {\n"				//6
																									"a = a + 1;\n"							//7
																							"}\n"
																						"}\n";
					
				TokenList tokenized_program = Tokenizer::Tokenize(program);
				parser.Parse(tokenized_program);

				PKB true_pkb = PKB();
				true_pkb.InsertParent("2","3");
				true_pkb.InsertParent("2","4");
				true_pkb.InsertParent("2","5");
				true_pkb.InsertParent("6","7");

				//*******************************

				StmtNumList test_all_parents = test_pkb.GetAllParent();
				StmtNumList true_all_parents = true_pkb.GetAllParent();
				
				StmtNumList::iterator iter_1 = test_all_parents.begin(); 
				StmtNumList::iterator iter_2 = true_all_parents.begin();

				// Should only have two parents
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1, *iter_2);

				//*******************************

				StmtNumList test_all_children = test_pkb.GetAllChild();
				StmtNumList true_all_children = true_pkb.GetAllChild();
				
				iter_1 = test_all_children.begin(); 
				iter_2 = true_all_children.begin();

				// Should have four children stmt_num
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1, *iter_2);

				//*********************************

				StmtNumList test_child_of_stmt = test_pkb.GetChild("2");
				StmtNumList true_child_of_stmt = true_pkb.GetChild("2");
				
				iter_1 = test_child_of_stmt.begin(); 
				iter_2 = true_child_of_stmt.begin();

				// Should have three children for stmt_num 2
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1, *iter_2);	

				//**********************************
				StmtNumList test_parent_of_stmt = test_pkb.GetParent("7");
				StmtNumList true_parent_of_stmt = true_pkb.GetParent("7");
				
				iter_1 = test_parent_of_stmt.begin(); 
				iter_2 = true_parent_of_stmt.begin();

				// Should have one parent of stmt_num 7
				Assert::AreEqual(*iter_1, *iter_2);	
		}
		TEST_METHOD(TestParentOfNestedStatements) {
				PKB test_pkb = PKB();
				Parser parser = Parser(&test_pkb);
				string program = "	procedure one {\n"					
                      "a = b;\n"														//1
                      "if (a > b) then {\n"   //2               
																								"e = f;\n"            //3          
																								"asd = ddd;\n"        //4     
																								"while (a < 10) {\n"				//5
																											"a = a + 1;\n"							//6
																											"k = k + 1;\n"							//7
																									"}\n"
																						"}\n"
																						"else {\n"
                        "k = 3;\n"												//8
                      "}\n"																				
                     "}\n";																				

					
				TokenList tokenized_program = Tokenizer::Tokenize(program);
				parser.Parse(tokenized_program);

				PKB true_pkb = PKB();
				true_pkb.InsertParent("5", "6");
				true_pkb.InsertParent("5", "7");
				true_pkb.InsertParent("2","3");
				true_pkb.InsertParent("2","4");
				true_pkb.InsertParent("2","5");
				true_pkb.InsertParent("2","8");

				//*******************************

				StmtNumList test_all_parents = test_pkb.GetAllParent();
				StmtNumList true_all_parents = true_pkb.GetAllParent();
				
				StmtNumList::iterator iter_1 = test_all_parents.begin(); 
				StmtNumList::iterator iter_2 = true_all_parents.begin();

				// Should only have two parents
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1, *iter_2);

				//*******************************

				StmtNumList test_all_children = test_pkb.GetAllChild();
				StmtNumList true_all_children = true_pkb.GetAllChild();
				
				iter_1 = test_all_children.begin(); 
				iter_2 = true_all_children.begin();

				// Should have six children stmt_num
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1, *iter_2);

				//*********************************

				StmtNumList test_child_of_stmt = test_pkb.GetChild("5");
				StmtNumList true_child_of_stmt = true_pkb.GetChild("5");
				
				iter_1 = test_child_of_stmt.begin(); 
				iter_2 = true_child_of_stmt.begin();

				// Should have two children for stmt_num 2
				Assert::AreEqual(*iter_1++, *iter_2++);
				Assert::AreEqual(*iter_1, *iter_2);	

				//**********************************

				StmtNumList test_parent_of_stmt = test_pkb.GetParent("5");
				StmtNumList true_parent_of_stmt = true_pkb.GetParent("5");
				
				iter_1 = test_parent_of_stmt.begin(); 
				iter_2 = true_parent_of_stmt.begin();

				// Should have one parent of stmt_num 5
				Assert::AreEqual(*iter_1, *iter_2);	
		}
};
}