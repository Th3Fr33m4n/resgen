#ifndef UTIL_H
#define UTIL_H

#ifdef WIN32
#define SIZE_T_SPECIFIER    "%Iu"
#else
#define SIZE_T_SPECIFIER    "%zu"
#endif

#ifdef WIN32
#define INLINE __forceinline
#else
#define INLINE inline
#endif

#include <algorithm>
#include <string>
#include <vector>

// RAII wrapper for FILE
class File
{
public:
	File();
	File(const char* fileName, const std::string& mode);
	File(const std::string& fileName, const std::string& mode);
	~File();

	void close();
	void open(const std::string& fileName, const std::string& mode);

	operator FILE*();
	FILE* operator->();

private:
	FILE* fileHandle;
};

struct file_s;

struct config_s
{
	bool verbal; // t -
	bool statusline; // f
	bool overwrite; // f
	bool tolower; // f
	bool matchcase; // f
	bool parseresource; // f
	bool preservewads; // f
	bool contentdisp; // f

	bool searchdisp; // f

	bool help; // f
	bool credits ; // f
	bool warranty; // f

	bool resourcedisp; // f

	std::vector<file_s> files;
	std::vector<file_s> excludes; // Map exclude list - not resource!
	std::vector<std::string> excludelists; // Exclude resource list files - not maps!

	std::string rfafile;

	bool checkpak; // t
	std::string resource_path;

#ifdef WIN32
	bool keypress; // t
#else
	bool symlink; // t
#endif
};

template <char Delimiter>
class Tokenizer
{
public:
	Tokenizer(std::string& str_)
		: str(str_)
		, currentPtr(&str[0])
		, strEnd(currentPtr + str.length())
	{
	}

	int GetLatestTokenLength() const
	{
		// currentPtr points to char after NUL
		const char* const end = (currentPtr ? currentPtr - 1 : strEnd);
		return end - tokenStart;
	}

	INLINE bool FindNext()
	{
		if(!currentPtr)
		{
			return false;
		}

		while(currentPtr != strEnd)
		{
			if(*currentPtr == Delimiter)
			{
				*currentPtr = 0;
				currentPtr++;
				return true;
			}

			currentPtr++;
		}

		currentPtr = NULL;
		return true;
	}

	INLINE bool SkipToken()
	{
		return FindNext();
	}

	INLINE const char * NextToken()
	{
		if(!currentPtr)
		{
			return NULL;
		}

		tokenStart = currentPtr;

		FindNext();
		return tokenStart;
	}

	INLINE const char * NextValue()
	{
		// Goes to the next entity token (key->value or value->key)
		if (!SkipToken()) // exit key/value
		{
			return NULL;
		}

		return NextToken(); // enter key/value
	}

	INLINE bool SkipValue()
	{
		return SkipToken() && SkipToken();
	}

private:
	Tokenizer(const Tokenizer &other);
	Tokenizer& operator=(const Tokenizer &other);

	std::string str;

	// Current position in string
	char* currentPtr;

	// Pointer to char after last
	char* strEnd;

	// Start of most recent token
	char* tokenStart;
};


std::string replaceCharAll(std::string str, const char find, const char replace);

std::string strToLowerCopy(const std::string &str);

void strToLower(std::string &str);

int CompareStrEndNoCase(const std::string &str, const std::string &ending);
int CompareStrEnd(const std::string &str, const std::string &ending);

void leftTrim(std::string &str);
void leftTrim(std::string &str, const std::string &trimmedChars);

void rightTrim(std::string &str);
void rightTrim(std::string &str, const std::string &trimmedChars);

bool readFile(const std::string &filename, std::string &outStr);

int ICompareStrings(const std::string &a, const std::string &b);

std::string BuildValvePath(const std::string &respath);
void EndWithPathSep(std::string &str);

#endif // UTIL_H
