/***************************************************************************
                          pwcknowledgesource.hpp  -  description
                             -------------------
    begin                : Wed Jun 7 2000
    copyright            : (C) 2000 by Patrick Wong
    email                : k.c.p.wong@open.ac.uk
 ***************************************************************************/

#ifndef PWCKNOWLEDGESOURCE_H
#define PWCKNOWLEDGESOURCE_H

///////////////////////////// global includes ////////////////////////////
#include <string>
#include <vector>

///////////////////////////// local includes /////////////////////////////
#include "pwcclient.hpp"
#include "pwcrule.hpp"
// #include "pwcpreconditionstatements.hpp"
#include "pwcvariable.hpp"
#include "pwcinstantiation.hpp"

// All ARBS keywords
#define BBALLKEYWORDS "add del_first del_all clr_partition clr_board ret_first ret_all get_contents setup_blackboard add_partitions get_partitions partition_exist partition_not_exist on_partition not_on_partition on_blackboard not_on_blackboard";
// ARBS keywords which requires no parameter
#define BBKEYWORDS1 "clr_board get_partitions";
// ARBS keywords which requires 1 parameters
#define BBKEYWORDS2 "clr_partition get_contents setup_blackboard add_partitions partition_exist partition_not_exist on_blackboard not_on_blackboard";
// ARBS keywords which requires 2 parameters
#define BBKEYWORDS3 "add del_first del_all ret_first ret_all on_partition not_on_partition";

class pwCClient;
class pwCPreconditionStatements;

/** class for knowledge sources.
  *@author Patrick Wong
  */

class pwCKnowledgeSource {
public: 
	pwCKnowledgeSource();
	virtual ~pwCKnowledgeSource();
	/** function that extract the KS variables. */
  virtual int CheckKeyWords();
  /** Load the knowledge file into the KS string*/
  virtual int LoadKnowledgeFile();
  /** this function print the KS variable on the screen. */
  virtual int PrintKS();
  /** This function initialise the KS object. */
  virtual int Initialise();
  /** This function set the KS file name. */
  virtual int SetKSFileName(std::string filename);
  /** Inference engine */
  virtual int Inference();
  /** the actions provoke when the KS is fired. */
  virtual int DoAction(int nInstantiations);
  /** check if the given string is one the ARBS key words. */
  virtual bool IsBBKeyWord(int nCategories, std::string token);
  /** print the message to the screen or other distinations. */
  virtual int Report(std::string message, int nType, int nInstant);
  /** run an external function to preformance a particular task. */
  virtual int RunAlgorithm(std::string Message, int nType, int nInstant);
  /** a recursive function that evaluates the KS precondition */
  virtual bool EvaluatePrecondition(std::string sToken, int nInstantiationIndex);
  /** function evaluate the action string for both KS and rules. */
  virtual int EvaluateAction(std::string sAction, int InstantIndex);
  /** check the BB commands and match variables before sending the command to BB. */
  virtual int SendTCPData(std::string sMessage, int nType, int nInstant);
  /** remove the first pair of square bracket of the given string */
  virtual std::string RemoveBrackets(std::string sToken);
  /** remove the first pair of <> bracket of the given string */
  virtual std::string RemovePointedBrackets(std::string sToken);
  /** it put the match variables (?a or ??a) in the KS variable list and replace the (~a or ~~a) with the insert variables. */
  virtual int EvaluateBBCommandContent(std::string sContent);
  /** it obtains all the match variables from the BB and stored them before the EvaluatePrecondition function is called. */
  virtual int PreEvaluatePrecondition(std::string);
  /** check if the given variable is exist in the variable list. */
  virtual bool IsVariableExist(std::string sVar);
  /** find the given variable and return it's reference. */
  virtual pwCVariable* FindVariable(std::string sName, std::vector<pwCVariable>* pvVar);
	/** SIGIO Call back function for processing the Precondition*/
  virtual int SIGIOcallbackPrecondition(std::string buffer);
	virtual void SIGIOcallbackAction(std::string buffer);
  /** // generate instantiations according to the given precondition string. */
  virtual int CreateInstantiations(std::string sPrecondition);
  /** do the actual data sending to tcp port and wait for the callback. */
  virtual int DoSendTCPData(std::string sMessage);
  /** find and evaluate the insert variables of the report content */
  virtual std::string EvaluateActionContent(std::string sContent, int InstantIndex);
  /** load data to blackboard directly */
  virtual int LoadFileToBB(std::string sContent, int nType, int nInstant);
  /** copy the key words from the given KS to itself. */
  virtual int CopyKeyWords(pwCKnowledgeSource* ks);
  /** Add the given partition name (if not exist in the list) to the partition list. */
  virtual int AddToPartitionList(std::string sPartition);
  /** check if the given partition is exist in the partition list. */
  virtual bool CheckPartitionList(std::string sPartition);
  /** a local function which adds the 2 given numeric values together. */
  virtual std::string IntegerAdder(std::string sIntegerValues);
  /** A local function for rule files, ask the user a question and request an answer. */
	virtual std::string GetTextInput(std::string sPrompt);
  /** a local funcation for displaying message to the output. */
  virtual void DisplayText(std::string sMessage);
  /** Merge a number of string together */
  std::string StringMerge(std::string sMergingStrings);
  /** apply operator to the 2 operands, return true or false. */
  bool Compare(std::string sContent);
  /** add the given string element to m_vsTemp */
  std::string StringListFormer(std::string sElement);
  /** clear the temp list for the external function. */
  std::string ClearStringList(std::string);
  /** Create a string that contains all the elements stored in the external string list. The elements are separated by a space. */
  std::string GetFormedStringList(std::string sStr);
  /** Count how many token is in the given string */
  std::string CountToken(std::string);
protected: // Protected attributes
  /** A set of conditions which must be satisfied before the KS can become active. */
  std::string m_sPreconditions;
  /** it keeps count of the unique ID for each precondition statements. */
  int m_nStatementID;
public: // Public attributes
  /** a pointer to the DARBS client object */
  pwCClient* m_pClient;
  /** the flag indicates KS is reday. */
  bool m_bReady;
  /** the flag indicates if the KS is fired. */
  bool m_bKSFired;
  /** the flag indicates if the KS need to be restarted. */
  bool m_bKSRestart;
  /** a list containing the run time variables. */
  std::vector<pwCVariable> m_vVariables;
  /** a list containing the variables and it's values for each instantiations. */
  std::vector<pwCVariable> m_vInstantVariables;
  /** a list containing each callback value from blackboard. */
  std::vector<std::string> m_vsCallbackValues;
  /** a list of boolean value for the evaluation of the KS preconditions. */
  std::vector<pwCPreconditionStatements> m_vPreConditionStatements;
  /** a flag to indicate what level of debugging info to print */
  int m_nDebug;
  /** The directory name of the KS file. */
  std::string m_sKSFileDirectory;
  /** The file name of the KS file. */
  std::string m_sKSFileName;
  /** The name of the KS. */
  std::string m_sKSName;
  /** word specifying the KS type ("rule_based_KS", "procedural_KS","neural_network_KS") */
  std::string m_sKSType;
  /** word specifyign Inference Mode ("MI_forwardchain","SI_forwardchain","Test_Hypotheses") */
  std::string m_sInferenceMode;
  /** The string that contains the list of rule. */
  std::string m_sRuleString;
  /** This is an extra control flag which can be used to prevent a KS from being examined. */
  std::string m_sFirabilityFlag;
  /** The string that contains the list of preconditions */
  std::string m_sPreconditionString;
  /** The string that contains the list of actions */
  std::string m_sActionString;
  /** a list of variable names that is going to be matched by the callback function. */
  std::vector<std::string> m_vsTempVariables;
  /** the string returned by the BB server. */
  std::string m_sCallBackString;
  /** a list containing a series of BB command contents */
  std::vector<std::string> m_vsBBCommandContents;
  /** a list containing existed partitions */
  std::vector<std::string> m_vsPartitions;
  /** contains the current command content. It can be used by the external function. */
  std::string m_sCurrentCommandContent;
  /** a string storing the content of the KS file. */
  std::string m_sKSString;
  /** A list of all instantiations */
  std::vector<pwCInstantiation> m_vInstantiations;
  /** A temporary string list. It can be used as a temporary storage in association with the external function. */
  std::vector<std::string> m_vsExternal;
  /** It is the index indicating which precondition statement is being process, used in EvaluatePrecondition(). */
  int m_nStatementIndex;
};

#endif
