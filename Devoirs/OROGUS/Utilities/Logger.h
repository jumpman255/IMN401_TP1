#ifndef _UTILITIES_LOGGER_H_
#define _UTILITIES_LOGGER_H_

#include "Types.h"

#include <algorithm>
#include <iostream>

class Logger
{
    static int32 m_indent;

public:
    static void IncIndent() { ++m_indent; }
    static void DecIndent() { --m_indent; m_indent = std::max(m_indent, 0); }
    friend std::ostream& Log();
};

std::ostream& Log();

#endif
