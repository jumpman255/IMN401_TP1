#ifndef _UTILITIES_STRINGUTILITIES_H_
#define _UTILITIES_STRINGUTILITIES_H_

#include <string>
#include <vector>

class StringUtilities
{
	StringUtilities() = delete;
	StringUtilities(const StringUtilities&) = delete;
    StringUtilities& operator=(const StringUtilities&) = delete;
    ~StringUtilities() = delete;
public:
    static bool Equals(const std::string& first, const std::string& second)
    {
        return first.compare(second) == 0;
    }

    static bool Equals(const std::string& first, const char* second)
    {
        return first.compare(second) == 0;
    }

    static bool Equals(const char* first, const std::string& second)
    {
        return second.compare(first) == 0;
    }

    static bool Equals(const char* first, const char* second)
    {
        return strcmp(first, second) == 0;
    }

	static bool StartsWith(const std::string& source, const std::string& str, bool skipWhitespace = true)
	{
		return StartsWith(source.c_str(), str.c_str(), skipWhitespace);
	}

	static bool StartsWith(const char* source, const char* str, bool skipWhitespace = true)
	{
		const char* current = source;
		if (skipWhitespace)
		{
			while ((*source == ' ' || *source == '\t') && *source != '\0')
			{
				++source;
			}
		}
		
		while (*str != '\0' && *source != '\0' && *str == *source)
		{
			++str;
			++source;
		}

		return *str == '\0';
	}
	
	static bool StartsWith(const std::string& source, char c, bool skipWhitespace = true)
	{
		return StartsWith(source.c_str(), c, skipWhitespace);
	}

	static bool StartsWith(const char* source, char c, bool skipWhitespace = true)
	{
		const char* current = source;
		if (skipWhitespace)
		{
			while ((*current == ' ' || *current == '\t') && *current != '\0')
			{
				++current;
			}
		}

		return *current == c;
	}

    static bool EndsWith(const std::string& source, const std::string& str, bool skipWhitespace = true)
    {
        return EndsWith(source.c_str(), str.c_str(), skipWhitespace);
    }

    static bool EndsWith(const char* source, const char* str, bool skipWhitespace = true)
    {
        size_t len = strlen(source);
        if (skipWhitespace)
        {
            while ((source[len-1] == ' ' || source[len-1] == '\t') && len > 0)
            {
                --len;
            }
        }

        size_t lenStr = strlen(str);
        while (len > 0 && lenStr > 0 && str[lenStr] == source[len])
        {
            --len;
            --lenStr;
        }

        return lenStr == 0;
    }

    static bool EndsWith(const std::string& source, char c, bool skipWhitespace = true)
    {
        return EndsWith(source.c_str(), c, skipWhitespace);
    }

    static bool EndsWith(const char* source, char c, bool skipWhitespace = true)
    {
        size_t len = strlen(source);
        if (skipWhitespace)
        {
            while (source[len-1] == ' ' || source[len-1] == '\t')
            {
                --len;
            }
        }

        return source[len-1] == c;
    }

	static std::vector<std::string> Split(std::string source, const std::string& sep)
	{
		std::vector<std::string> resultSplit;
		size_t found;
		while ((found = source.find(sep)) != std::string::npos) {
			resultSplit.push_back(source.substr(0, found));
			source = source.substr(found + sep.size());
		}
		return resultSplit;
	}

	static std::vector<std::string> Split(const char* source, const char* sep)
	{
        return Split(std::string(source), std::string(sep));
	}

	static std::vector<std::string> Split(std::string source, char sep)
	{
		std::vector<std::string> resultSplit;
		size_t found;
		while ((found = source.find(sep)) != std::string::npos) {
			resultSplit.push_back(source.substr(0, found));
			source = source.substr(found + 1);
		}
		return resultSplit;
	}

    static std::vector<std::string> Split(const char* source, char sep)
    {
        return Split(std::string(source), sep);
    }
};

#endif
