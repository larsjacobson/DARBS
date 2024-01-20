// $Id$

/**
 * Copyright (C) 2010 Adrian Hopgood et al.
 *
 * Licensed under the Academic Free License version 3.0
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *
 */

/**
 * @file RuleBasedKS.cpp
 */

// Global includes
#include <iostream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/thread/thread.hpp> 
#include <boost/filesystem.hpp>

//Local includes
#include "Rule.hpp"
#include "RuleBasedKS.hpp"
#include "Logger.hpp"
#include "Util.hpp"

// Namespaces

// Defines 

// Global variables

// Member functions


/**
 * @brief Constructor.
 */
RuleBasedKS::RuleBasedKS(TcpClient &client, boost::filesystem::path path)
    :KnowledgeSource(client, path) {
}

/**
 * @brief Destructor.
 */
RuleBasedKS::~RuleBasedKS() {
}

/**
 * @brief The KSs main loop
 */
void RuleBasedKS::run() {
    notifyExistance();
    notifyInfo();
    bool result;
    bool haveIncremented;
    bool canPause;
    while (!client.isError()) {
        result = false;
        haveIncremented = true;
        canPause = true;
        while (haveIncremented) {
            LOG("Evaluating preconditions", INFO);
            result = precondition.evaluate();
            LOG("Preconditions evaluated " << result, INFO);

            if (result) {
                LOG("Running rules", INFO);
                readConnection.block();
                std::vector<Rule *>::iterator it = rules.begin();
                for (it; it != rules.end(); it++) {
                    (*it)->run();
                }

                LOG("Evaluating actions", INFO);
                readConnection.unblock();
                action.evaluate();
                canPause = false;
                timesFired += 1;
                notifyTimesFired();
                notifyRules();
            }

            LOG("Incrementing variables", INFO);
            haveIncremented = precondition.incrementVariables();
        }
        precondition.reset();
        action.reset();

        if (canPause) {
            // Assume we will pause
            setState(PAUSED);
            // Don't pause if whilst running any of our watched partitions
            // have changed, as we cannot be sure if we have reacted
            bool needRun = false;
            boost::mutex::scoped_lock lock(stateMutex);
            std::set<std::string>::iterator it = changedPartitions.begin();
            for (it; it != changedPartitions.end(); it++) {
                std::string partitionName = *it;
                if (precondition.partitionWatched(partitionName)) {
                    needRun = true;
                    break;
                }
            }
            lock.unlock();

            if (needRun) {
                // Watched partition changed, so we need to run
                setState(RUNNING);
            }
            else { 
                // We can actually pause
                LOG("Pausing", INFO);
                // Only notify if we will actually pause
                notifyState();
            }
            changedPartitions.clear();
        }
        
        do {
            boost::this_thread::sleep(boost::posix_time::milliseconds(100));
            watchState();
        } while (getState() == PAUSED || getState() == STOPPED);

        if (canPause) {
            // We have paused, but woken up.  Notify!
            notifyState();
        }
    }
    LOG("Exiting KS main loop", INFO);
}

/**
 * @brief Parse a rule-based KS xml file, extracting the rules
 *
 * @param ksDoc The xml document containing the ks
 */
void RuleBasedKS::parse(xml_document &ksDoc) {
    KnowledgeSource::parse(ksDoc);
    xml_node ks = ksDoc.child("agent");

    xml_node ruleNode = ks.child("rule");
    for (ruleNode; ruleNode; ruleNode = ruleNode.next_sibling("rule")) {
        xml_document ruleDoc;
        std::string fileName = ruleNode.child_value();
        boost::algorithm::trim(fileName);

        boost::filesystem::path rulePath = fileName;
        if (!rulePath.is_complete()) {
            // Add the base path to the filename
            boost::filesystem::path tmpPath = path;
            boost::filesystem::path basePath = tmpPath.remove_filename();
            rulePath = basePath / rulePath;
        }
        xml_parse_result parseResult = ruleDoc.load_file(rulePath.string().c_str());
        if (parseResult) {
            Rule *rule = new Rule(client, this, rulePath);
            rules.push_back(rule);
            rule->parse(ruleDoc);
        }
        else {
            LOG("Syntax error parsing rule file " << ruleNode.child_value() <<
                " - " << parseResult.description(), ERROR);
        }
    }
}

/**
 * @brief Add patterns to the blackboard containing info about the KS
 */
void RuleBasedKS::notifyInfo() {
    xml_document requestDoc;
    xml_node request = addRequest("add", requestDoc);
    xml_node partition = addPartition("_ks_" + name + "_info", request);
    xml_node typePattern = addPattern(partition);
    addValue("Rule Based Knowledge Source", typePattern);
    xml_node firedPattern = addPattern(partition);
    addValue("Fired " + itos(timesFired) + " times", firedPattern);
    xml_node rulePattern = addPattern(partition);
    std::string rulesText = itos(rules.size()) + " rules contained : ";
    for (int i=0; i<rules.size(); i++) {
        rulesText += rules[i]->getName() + " (";
        rulesText += itos(rules[i]->getTimesFired());
        rulesText += ") ";
    }
    addValue(rulesText, rulePattern);
    xml_node statePattern = addPattern(partition);
    addValue("State is " + itos(state), statePattern);
    xml_node response = sendRequest(request);
}
    
void RuleBasedKS::notifyRules() {
    xml_document requestDoc;
    xml_node request = addRequest("replace", requestDoc);
    xml_node partition = addPartition("_ks_" + name + "_info", request);
    xml_node findPattern = addPattern(partition);
    addValue("?rules rules contained ??info", findPattern);
    xml_node replacePattern = addPattern(partition);
    std::string rulesText = itos(rules.size()) + " rules contained : ";
    for (int i=0; i<rules.size(); i++) {
        rulesText += rules[i]->getName() + " (";
        rulesText += itos(rules[i]->getTimesFired());
        rulesText += ") ";
    }
    addValue(rulesText, replacePattern);
    xml_node response = sendRequest(request);
}
