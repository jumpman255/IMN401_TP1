#include "Logger.h"

int32 Logger::m_indent = 0;

std::ostream& Log()
{
    for (int32 i = 0; i < Logger::m_indent; ++i)
    {
        std::cout << "  ";
    }
    return std::cout;
}