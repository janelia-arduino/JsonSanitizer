// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _JSON_SANITIZER_CONSTANTS_H_
#define _JSON_SANITIZER_CONSTANTS_H_
#include "ConstantVariable.h"


namespace constants
{
enum
  {
    SPACE = (int)' ',
    TAB = (int)'\t',
    CARRIAGE_RETURN = (int)'\r',
    NEWLINE = (int)'\n',
    FORWARD_SLASH = (int)'/',
    ASTERISK = (int)'/',
    OPEN_BRACE = (int)'{',
    CLOSE_BRACE = (int)'}',
    OPEN_BRACKET = (int)'[',
    CLOSE_BRACKET = (int)']',
    DOUBLE_QUOTE = (int)'\"',
  };
}
#endif
