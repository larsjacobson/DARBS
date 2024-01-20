#include <boost/test/unit_test.hpp>

#include "../../../server/src/LnTokenizer.hpp"

BOOST_AUTO_TEST_CASE(ConstructorSetsValue)
{
    std::string token("[a token]");
    LnTokenizer tokenizer(token);
    BOOST_CHECK_EQUAL(tokenizer.getRemains(), token);
}

