#include <boost/test/unit_test.hpp>

#include "Blackboard.hpp"
#include "pugixml.hpp"

using namespace pugi;

/**
 * @brief A fixture to simplify testing the blackboard
 */
class BlackboardFixture {
public:
    Blackboard blackboard;
    xml_document request;
    xml_document response;
    std::string req, res;

    /**
     * @brief Empty constructor
     */
    BlackboardFixture() {
    }

    /**
     * @brief Empty destructor
     */
    ~BlackboardFixture() {
    }

    /**
     * @brief Convert the blackboard response to a raw string
     */
    std::string getResponseString() {
        std::stringstream outStringStream;
        xml_writer_stream streamWriter(outStringStream);
        response.save(streamWriter, "\t", format_raw);
        return(outStringStream.str());
    }

    /**
     * @brief Add a declaration and response tags around an xml string
     */
    std::string buildResponse(std::string contents) {
        return("<?xml version=\"1.0\"?><response>" + contents + "</response>");
    }

    /**
     * @brief Check that the request in req gives the response in res
     */
    void check() {
        res = buildResponse(res);
    
        request.load(req.c_str());
        blackboard.process(request, response);
        BOOST_CHECK_EQUAL(getResponseString(), res);
    }

    /**
     * @brief Run the request contained in req
     */
    void run() {
        request.load(req.c_str());
        blackboard.process(request, response);
    }

};

BOOST_FIXTURE_TEST_SUITE(blackboard, BlackboardFixture)

/**
 * @brief Adding a partition to the blackboard
 */
BOOST_AUTO_TEST_CASE(AddPartition)
{
    // Check response
    req = "<request type=\"add\"><partition name=\"aPartition\" /></request>";
    res = "<partition name=\"aPartition\">true</partition>";
    check();

    // Check presence
    req = "<request type=\"present\"><partition name=\"aPartition\" /></request>";
    res = "<partition name=\"aPartition\">true</partition>";
    check();

}

/**
 * @brief Trying to add the same partition twice
 */
BOOST_AUTO_TEST_CASE(AddDuplicatePartition)
{
    // Check response
    req = "<request type=\"add\"><partition name=\"aPartition\" /><partition name=\"aPartition\" /></request>";
    res = "<partition name=\"aPartition\">true</partition><partition name=\"aPartition\">false</partition>";
    check();

    // Check presence
    req = "<request type=\"present\"><partition name=\"aPartition\" /></request>";
    res = "<partition name=\"aPartition\">true</partition>";
    check();
}

/**
 * @brief Adding data to a new partition
 */
BOOST_AUTO_TEST_CASE(AddDataNewPartition) {
    // Check response
    req = "<request type=\"add\"><partition name=\"aPartition\"><pattern>Data</pattern></partition></request>";
    res = "<partition name=\"aPartition\"><pattern>true</pattern></partition>";
    check();


    // Check presence
    req = "<request type=\"present\"><partition name=\"aPartition\"><pattern>Data</pattern></partition></request>";
    res = "<partition name=\"aPartition\"><pattern>true</pattern></partition>";
    check();
} 

/**
 * Adding data to an existing partition
 */
BOOST_AUTO_TEST_CASE(AddDataExistingPartition) {
    // Create partition
    req = "<request type=\"add\"><partition name=\"aPartition\" /></request>";
    run();

    // Check response
    req = "<request type=\"add\"><partition name=\"aPartition\"><pattern>Data</pattern></partition></request>";
    res = "<partition name=\"aPartition\"><pattern>true</pattern></partition>";
    check();


    // Check presence
    req = "<request type=\"present\"><partition name=\"aPartition\"><pattern>Data</pattern></partition></request>";
    res = "<partition name=\"aPartition\"><pattern>true</pattern></partition>";
    check();
}

/**
 * @brief Deleting a partition
 */
BOOST_AUTO_TEST_CASE(DeletePartition) {
    // Create partition
    req = "<request type=\"add\"><partition name=\"aPartition\" /></request>";
    run();

    // Check response
    req = "<request type=\"delete\"><partition name=\"aPartition\" /></request>";
    res = "<partition name=\"aPartition\">true</partition>";
    check();

    // Check presence
    req = "<request type=\"present\"><partition name=\"aPartition\" /></request>";
    res = "<partition name=\"aPartition\">false</partition>";
    check();
}

/**
 * @brief Deleting a pattern from a partition
 */
BOOST_AUTO_TEST_CASE(DeletePattern) {
    // Create pattern
    req = "<request type=\"add\"><partition name=\"aPartition\"><pattern>Data</pattern></partition></request>";
    run();

    // Check response
    req = "<request type=\"delete\"><partition name=\"aPartition\"><pattern>Data</pattern></partition></request>";
    res = "<partition name=\"aPartition\"><pattern>true</pattern></partition>";
    check();

    // Check presence
    req = "<request type=\"present\"><partition name=\"aPartition\"><pattern>Data</pattern></partition></request>";
    res = "<partition name=\"aPartition\"><pattern>false</pattern></partition>";
    check();

}

/**
 * @brief Deleting a pattern from any partition
 */
BOOST_AUTO_TEST_CASE(DeletePatternFromAll) {
    // Create patterns
    req = "<request type=\"add\"><partition name=\"aPartition\"><pattern>Data</pattern></partition><partition name=\"anotherPartition\"><pattern>Data</pattern></partition></request>";
    run();

    // Check response
    req = "<request type=\"delete\"><partition name=\"?\"><pattern>Data</pattern></partition></request>";
    res = "<partition name=\"?\"><pattern>true</pattern></partition>";
    check();

    // Check presence
    req = "<request type=\"present\"><partition name=\"aPartition\"><pattern>Data</pattern></partition><partition name=\"anotherPartition\"><pattern>Data</pattern></partition></request>";
    res = "<partition name=\"aPartition\"><pattern>false</pattern></partition><partition name=\"anotherPartition\"><pattern>false</pattern></partition>";
    check();
}

/**
 * Replace a pattern on a partition
 */
BOOST_AUTO_TEST_CASE(ReplacePattern) {
    // Create pattern
    req = "<request type=\"add\"><partition name=\"aPartition\"><pattern>Data</pattern></partition></request>";
    run();

    // Check response
    req = "<request type=\"replace\"><partition name=\"aPartition\"><pattern>Data</pattern><pattern>Other Data</pattern></partition></request>";
    res = "<partition name=\"aPartition\"><pattern>true</pattern></partition>";
    check();

    // Check presence
    req = "<request type=\"present\"><partition name=\"aPartition\"><pattern>Data</pattern><pattern>Other Data</pattern></partition></request>";
    res = "<partition name=\"aPartition\"><pattern>false</pattern><pattern>true</pattern></partition>";
    check();
}

/**
 * @brief Replace a pattern on all partitions
 */
BOOST_AUTO_TEST_CASE(ReplacePatternOnAll) {
    req = "<request type=\"add\"><partition name=\"aPartition\"><pattern>Data</pattern></partition><partition name=\"anotherPartition\"><pattern>Data</pattern></partition></request>";
    run();

    req = "<request type=\"replace\"><partition name=\"?\"><pattern>Data</pattern><pattern>Other Data</pattern></partition></request>";
    res = "<partition name=\"?\"><pattern>true</pattern></partition>";
    check();

    req = "<request type=\"present\"><partition name=\"aPartition\"><pattern>Data</pattern><pattern>Other Data</pattern></partition><partition name=\"anotherPartition\"><pattern>Data</pattern><pattern>Other Data</pattern></partition></request>";
    res = "<partition name=\"aPartition\"><pattern>false</pattern><pattern>true</pattern></partition><partition name=\"anotherPartition\"><pattern>false</pattern><pattern>true</pattern></partition>";
    check();
}

/**
 * @brief Check whether a pattern exists on any partition
 */
BOOST_AUTO_TEST_CASE(PatternPresentOnAny) {
    req = "<request type=\"add\"><partition name=\"aPartition\"><pattern>Data</pattern></partition></request>";
    run();

    req = "<request type=\"present\"><partition name=\"?\"><pattern>Data</pattern></partition></request>";
    res = "<partition name=\"?\"><pattern>true</pattern></partition>";
    check();
}

/**
 * @brief Get the entire contents of the blackboard
 */
BOOST_AUTO_TEST_CASE(GetAll) {
    req = "<request type=\"add\"><partition name=\"aPartition\"><pattern>Data</pattern></partition><partition name=\"anotherPartition\"><pattern>Other Data</pattern></partition></request>";
    run();

    req = "<request type=\"get\" />";
    res = "<partition name=\"aPartition\"><pattern>Data</pattern></partition><partition name=\"anotherPartition\"><pattern>Other Data</pattern></partition>";
    check();
}

/**
 * @brief Get matches on a partition using a ~wildcard
 */
BOOST_AUTO_TEST_CASE(WildcardGet) {
    req = "<request type=\"add\"><partition name=\"aPartition\"><pattern>Data One</pattern><pattern>Data Two</pattern></partition></request>";
    run();

    req = "<request type=\"get\"><partition name=\"aPartition\"><pattern>Data ?number</pattern></partition></request>";
    res = "<partition name=\"aPartition\"><pattern><key name=\"number\"><match>One</match><match>Two</match></key></pattern></partition>";
    check();
}

/**
 * @brief Get matches on a partition using a ~~wildcard
 */
BOOST_AUTO_TEST_CASE(DoubleWildcardGet) {
    req = "<request type=\"add\"><partition name=\"aPartition\"><pattern>Data One A</pattern><pattern>Data Two B</pattern></partition></request>";
    run();

    req = "<request type=\"get\"><partition name=\"aPartition\"><pattern>Data ??number</pattern></partition></request>";
    res = "<partition name=\"aPartition\"><pattern><key name=\"number\"><match>One A</match><match>Two B</match></key></pattern></partition>";
    check();
}

/**
 * @brief Get matches on the entire blackboard using a ~wildcard
 */
BOOST_AUTO_TEST_CASE(WildcardGetAny) {
    req = "<request type=\"add\"><partition name=\"aPartition\"><pattern>Data One</pattern><pattern>Data Two</pattern></partition><partition name=\"anotherPartition\"><pattern>Data Three</pattern><pattern>Data Four</pattern></partition></request>";
    run();

    req = "<request type=\"get\"><partition name=\"?\"><pattern>Data ?number</pattern></partition></request>";
    res = "<partition name=\"?\"><pattern><key name=\"number\"><match>One</match><match>Two</match><match>Three</match><match>Four</match></key></pattern></partition>";
    check();
}

/**
 * @brief Check for presence using a == wildcard
 */
 BOOST_AUTO_TEST_CASE(WildcardPresent) {
    req = "<request type=\"add\"><partition name=\"aPartition\"><pattern>Data One A</pattern></partition></request>";
    run();

    req = "<request type=\"present\"><partition name=\"aPartition\"><pattern>Data ==</pattern></partition></request>";
    res = "<partition name=\"aPartition\"><pattern>true</pattern></partition>";
    check();
}

/**
 * @brief Clear everything from the blackboard
 */
BOOST_AUTO_TEST_CASE(ClearAll) {
    req = "<request type=\"add\"><partition name=\"aPartition\"><pattern>Data</pattern></partition></request>";
    run();

    req = "<request type=\"clear\" />";
    res = "true";
    check();

    req = "<request type=\"present\"><partition name=\"aPartition\" /></request>";
    res = "<partition name=\"aPartition\">false</partition>";
    check();
}

/**
 * @brief Clear all patterns from a partition on the blackboard
 */
BOOST_AUTO_TEST_CASE(ClearPartition) {
    req = "<request type=\"add\"><partition name=\"aPartition\"><pattern>Data</pattern></partition></request>";
    run();

    req = "<request type=\"clear\"><partition name=\"aPartition\" /></request>";
    res = "<partition name=\"aPartition\">true</partition>";
    check();

    req = "<request type=\"present\"><partition name=\"aPartition\" />\"><partition name=\"aPartition\"><pattern>Data</pattern></partition></request>";
    res = "<partition name=\"aPartition\">true</partition><partition name=\"aPartition\"><pattern>false</pattern></partition>";
}

BOOST_AUTO_TEST_SUITE_END()


