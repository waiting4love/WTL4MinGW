/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/CharType.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 10:08:49
// 
// $Id: CharType.h,v 1.4 2006/12/03 07:51:57 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_CHARTYPE_H__
#define __STDEXT_CHARTYPE_H__

#ifndef __STDEXT_BASIC_H__
#include "Basic.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// struct DigitTable

#define STD_DIGIT_TABLE_BASE	'0'
#define STD_DIGIT_TABLE_MAX		('f' - '0' + 1)
#define STD_DIGIT_INVALID		(0xff)

template <class Unused>
struct DigitTableT
{
	static BYTE data[STD_DIGIT_TABLE_MAX];

	static unsigned toDigit(int c) {
		unsigned index = c - STD_DIGIT_TABLE_BASE;
		return index < STD_DIGIT_TABLE_MAX ? data[index] : STD_DIGIT_INVALID;
	}
};

template <class Unused>
BYTE DigitTableT<Unused>::data[STD_DIGIT_TABLE_MAX] =
{
	0,	// 0 [48]
	1,	// 1 [49]
	2,	// 2 [50]
	3,	// 3 [51]
	4,	// 4 [52]
	5,	// 5 [53]
	6,	// 6 [54]
	7,	// 7 [55]
	8,	// 8 [56]
	9,	// 9 [57]
	STD_DIGIT_INVALID,	// : [58]
	STD_DIGIT_INVALID,	// ; [59]
	STD_DIGIT_INVALID,	// < [60]
	STD_DIGIT_INVALID,	// = [61]
	STD_DIGIT_INVALID,	// > [62]
	STD_DIGIT_INVALID,	// ? [63]
	STD_DIGIT_INVALID,	// @ [64]
	10,	// A [65]
	11,	// B [66]
	12,	// C [67]
	13,	// D [68]
	14,	// E [69]
	15,	// F [70]
	STD_DIGIT_INVALID,	// G [71]
	STD_DIGIT_INVALID,	// H [72]
	STD_DIGIT_INVALID,	// I [73]
	STD_DIGIT_INVALID,	// J [74]
	STD_DIGIT_INVALID,	// K [75]
	STD_DIGIT_INVALID,	// L [76]
	STD_DIGIT_INVALID,	// M [77]
	STD_DIGIT_INVALID,	// N [78]
	STD_DIGIT_INVALID,	// O [79]
	STD_DIGIT_INVALID,	// P [80]
	STD_DIGIT_INVALID,	// Q [81]
	STD_DIGIT_INVALID,	// R [82]
	STD_DIGIT_INVALID,	// S [83]
	STD_DIGIT_INVALID,	// T [84]
	STD_DIGIT_INVALID,	// U [85]
	STD_DIGIT_INVALID,	// V [86]
	STD_DIGIT_INVALID,	// W [87]
	STD_DIGIT_INVALID,	// X [88]
	STD_DIGIT_INVALID,	// Y [89]
	STD_DIGIT_INVALID,	// Z [90]
	STD_DIGIT_INVALID,	// [ [91]
	STD_DIGIT_INVALID,	// \ [92]
	STD_DIGIT_INVALID,	// ] [93]
	STD_DIGIT_INVALID,	// ^ [94]
	STD_DIGIT_INVALID,	// _ [95]
	STD_DIGIT_INVALID,	// ` [96]
	10,	// a [97]
	11,	// b [98]
	12,	// c [99]
	13,	// d [100]
	14,	// e [101]
	15,	// f [102]
};

typedef DigitTableT<void> DigitTable;

// -------------------------------------------------------------------------
// STD_CTYPE_XXX

#define STD_CTYPE_UPPER				0x01	/* upper case letter[A-Z] */
#define STD_CTYPE_LOWER				0x02	/* lower case letter[a-z] */
#define STD_CTYPE_DIGIT				0x04	/* digit[0-9] */
#define STD_CTYPE_UNDERLINE			0x08	/* underline[_] */
#define STD_CTYPE_XDIGIT			0x10	/* xdigit[0-9a-fA-F] */
#define STD_CTYPE_PATH_SEP			0x20	/* [/\\] */
#define STD_CTYPE_MAX_CHAR			128

#define STD_CTYPE_ALPHA				(STD_CTYPE_UPPER|STD_CTYPE_LOWER)
#define STD_CSYMBOL_FIRST_CHAR		(STD_CTYPE_ALPHA|STD_CTYPE_UNDERLINE)
#define STD_CSYMBOL_NEXT_CHAR		(STD_CSYMBOL_FIRST_CHAR|STD_CTYPE_DIGIT)

// -------------------------------------------------------------------------

#define __STD_CTYPE_STRING													\
	"UPPER", "LOWER", "DIGIT", "UNDERLINE", "XDIGIT", "PATH_SEP"

#define __STD_CTYPE_IS(mask, c)												\
	(STD_CTYPE_MAX_CHAR > (unsigned)(c) ? ((mask) & data[c]) : 0)

#define __STD_CTYPE_TRANS(op, cnv)											\
	struct op {																\
		template <class E>													\
		void operator()(E& c) const { c = cnv(c); }							\
	}

#define __STD_CTYPE_OP(op, is)												\
	struct op {																\
		int operator()(int c) const { return is(c); }						\
	}

#define __STD_CTYPE_OP_AND_NOT(op, is)										\
	__STD_CTYPE_OP(op, is);													\
	__STD_CTYPE_OP(Not##op, !is)

// -------------------------------------------------------------------------
// class CharType

template <class Unused>
struct CharTypeT
{
	typedef int mask_type;

	static BYTE data[STD_CTYPE_MAX_CHAR];

	static int is(int typeMask, int c) {
		return __STD_CTYPE_IS(typeMask, c);
	}

	static int isDigit(int c) {
		return __STD_CTYPE_IS(STD_CTYPE_DIGIT, c);
	}

	static int isLower(int c) {
		return __STD_CTYPE_IS(STD_CTYPE_LOWER, c);
	}

	static int isUpper(int c) {
		return __STD_CTYPE_IS(STD_CTYPE_UPPER, c);
	}

	static int isAlpha(int c) {
		return __STD_CTYPE_IS(STD_CTYPE_ALPHA, c);
	}
	
	static int isXDigit(int c) {
		return __STD_CTYPE_IS(STD_CTYPE_XDIGIT, c);
	}

	static int isCSymbolFirstChar(int c) {
		return __STD_CTYPE_IS(STD_CSYMBOL_FIRST_CHAR, c);
	}

	static int isCSymbolNextChar(int c) {
		return __STD_CTYPE_IS(STD_CSYMBOL_NEXT_CHAR, c);
	}

	static int isPathSeparator(int c) {
		return c == '/' || c == '\\';
	}

	static int isUnderline(int c) {
		return c == '_';
	}

	static int isEOL(int c) {
		return c == 0x0a || c == 0x0c || c == -1;
	}

	struct IsDigit;				struct NotIsDigit;
	struct IsLower;				struct NotIsLower;
	struct IsUpper;				struct NotIsUpper;
	struct IsAlpha;				struct NotIsAlpha;
	struct IsXDigit;			struct NotIsXDigit;
	struct IsCSymbolFirstChar;	struct NotIsCSymbolFirstChar;
	struct IsCSymbolNextChar;	struct NotIsCSymbolNextChar;
	struct IsUnderline;			struct NotIsUnderline;
	struct IsPathSeparator;		struct NotIsPathSeparator;
	struct IsEOL;				struct NotIsEOL;

	__STD_CTYPE_OP_AND_NOT(IsDigit, isDigit);
	__STD_CTYPE_OP_AND_NOT(IsLower, isLower);
	__STD_CTYPE_OP_AND_NOT(IsUpper, isUpper);
	__STD_CTYPE_OP_AND_NOT(IsAlpha, isAlpha);
	__STD_CTYPE_OP_AND_NOT(IsXDigit, isXDigit);
	__STD_CTYPE_OP_AND_NOT(IsUnderline, isUnderline);
	__STD_CTYPE_OP_AND_NOT(IsPathSeparator, isPathSeparator);
	__STD_CTYPE_OP_AND_NOT(IsCSymbolFirstChar, isCSymbolFirstChar);
	__STD_CTYPE_OP_AND_NOT(IsCSymbolNextChar, isCSymbolNextChar);
	__STD_CTYPE_OP_AND_NOT(IsEOL, isEOL);

	struct ToUpper;
	struct ToLower;

	__STD_CTYPE_TRANS(ToUpper, toupper);
	__STD_CTYPE_TRANS(ToLower, tolower);
};

template <class Unused>
BYTE CharTypeT<Unused>::data[STD_CTYPE_MAX_CHAR] =
{
	0,	//   [0]
	0,	//   [1]
	0,	//   [2]
	0,	//   [3]
	0,	//   [4]
	0,	//   [5]
	0,	//   [6]
	0,	//   [7]
	0,	//   [8]
	0,	//   [9]
	0,	//   [10]
	0,	//   [11]
	0,	//   [12]
	0,	//   [13]
	0,	//   [14]
	0,	//   [15]
	0,	//   [16]
	0,	//   [17]
	0,	//   [18]
	0,	//   [19]
	0,	//   [20]
	0,	//   [21]
	0,	//   [22]
	0,	//   [23]
	0,	//   [24]
	0,	//   [25]
	0,	//   [26]
	0,	//   [27]
	0,	//   [28]
	0,	//   [29]
	0,	//   [30]
	0,	//   [31]
	0,	//   [32]
	0,	// ! [33]
	0,	// " [34]
	0,	// # [35]
	0,	// $ [36]
	0,	// % [37]
	0,	// & [38]
	0,	// ' [39]
	0,	// ( [40]
	0,	// ) [41]
	0,	// * [42]
	0,	// + [43]
	0,	// , [44]
	0,	// - [45]
	0,	// . [46]
	STD_CTYPE_PATH_SEP,	// / [47]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 0 [48]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 1 [49]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 2 [50]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 3 [51]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 4 [52]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 5 [53]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 6 [54]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 7 [55]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 8 [56]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 9 [57]
	0,	// : [58]
	0,	// ; [59]
	0,	// < [60]
	0,	// = [61]
	0,	// > [62]
	0,	// ? [63]
	0,	// @ [64]
	STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// A [65]
	STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// B [66]
	STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// C [67]
	STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// D [68]
	STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// E [69]
	STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// F [70]
	STD_CTYPE_UPPER,	// G [71]
	STD_CTYPE_UPPER,	// H [72]
	STD_CTYPE_UPPER,	// I [73]
	STD_CTYPE_UPPER,	// J [74]
	STD_CTYPE_UPPER,	// K [75]
	STD_CTYPE_UPPER,	// L [76]
	STD_CTYPE_UPPER,	// M [77]
	STD_CTYPE_UPPER,	// N [78]
	STD_CTYPE_UPPER,	// O [79]
	STD_CTYPE_UPPER,	// P [80]
	STD_CTYPE_UPPER,	// Q [81]
	STD_CTYPE_UPPER,	// R [82]
	STD_CTYPE_UPPER,	// S [83]
	STD_CTYPE_UPPER,	// T [84]
	STD_CTYPE_UPPER,	// U [85]
	STD_CTYPE_UPPER,	// V [86]
	STD_CTYPE_UPPER,	// W [87]
	STD_CTYPE_UPPER,	// X [88]
	STD_CTYPE_UPPER,	// Y [89]
	STD_CTYPE_UPPER,	// Z [90]
	0,	// [ [91]
	STD_CTYPE_PATH_SEP,	// \ [92]
	0,	// ] [93]
	0,	// ^ [94]
	STD_CTYPE_UNDERLINE,	// _ [95]
	0,	// ` [96]
	STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// a [97]
	STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// b [98]
	STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// c [99]
	STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// d [100]
	STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// e [101]
	STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// f [102]
	STD_CTYPE_LOWER,	// g [103]
	STD_CTYPE_LOWER,	// h [104]
	STD_CTYPE_LOWER,	// i [105]
	STD_CTYPE_LOWER,	// j [106]
	STD_CTYPE_LOWER,	// k [107]
	STD_CTYPE_LOWER,	// l [108]
	STD_CTYPE_LOWER,	// m [109]
	STD_CTYPE_LOWER,	// n [110]
	STD_CTYPE_LOWER,	// o [111]
	STD_CTYPE_LOWER,	// p [112]
	STD_CTYPE_LOWER,	// q [113]
	STD_CTYPE_LOWER,	// r [114]
	STD_CTYPE_LOWER,	// s [115]
	STD_CTYPE_LOWER,	// t [116]
	STD_CTYPE_LOWER,	// u [117]
	STD_CTYPE_LOWER,	// v [118]
	STD_CTYPE_LOWER,	// w [119]
	STD_CTYPE_LOWER,	// x [120]
	STD_CTYPE_LOWER,	// y [121]
	STD_CTYPE_LOWER,	// z [122]
};

typedef CharTypeT<void> CharType;

// -------------------------------------------------------------------------
// class TestCharType

#ifdef __STDEXT_LOG_H__

template <class LogT>
class TestCharType
{
#define __STD_CTYPE_GEN(from, to, ctype)									\
	{																		\
		for (int i = from; i <= to; ++i)									\
			data[i] |= ctype;												\
	}

#define __STD_CTYPE_TEST(from, to, ctype)									\
	{																		\
		for (int i = from; i <= to; ++i)									\
			AssertExp(std::CharType::data[i] & ctype);						\
	}

public:
	static void doTest(LogT& log)
	{
		__STD_CTYPE_TEST('a', 'f', STD_CTYPE_XDIGIT);
		__STD_CTYPE_TEST('a', 'z', STD_CTYPE_LOWER);
		__STD_CTYPE_TEST('A', 'F', STD_CTYPE_XDIGIT);
		__STD_CTYPE_TEST('A', 'Z', STD_CTYPE_UPPER);
		__STD_CTYPE_TEST('0', '9', STD_CTYPE_DIGIT);
		__STD_CTYPE_TEST('0', '9', STD_CTYPE_XDIGIT);
		__STD_CTYPE_TEST('_', '_', STD_CTYPE_UNDERLINE);
		__STD_CTYPE_TEST('/', '/', STD_CTYPE_PATH_SEP);
		__STD_CTYPE_TEST('\\', '\\', STD_CTYPE_PATH_SEP);
	}

	static void doGen()
	{
		FileLog log("/__ctype__.h");

		BYTE data[128] = { 0 };
		const char* ctypes[] = { __STD_CTYPE_STRING };

		__STD_CTYPE_GEN('a', 'f', STD_CTYPE_XDIGIT);
		__STD_CTYPE_GEN('a', 'z', STD_CTYPE_LOWER);
		__STD_CTYPE_GEN('A', 'F', STD_CTYPE_XDIGIT);
		__STD_CTYPE_GEN('A', 'Z', STD_CTYPE_UPPER);
		__STD_CTYPE_GEN('0', '9', STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT);
		__STD_CTYPE_GEN('_', '_', STD_CTYPE_UNDERLINE);
		__STD_CTYPE_GEN('/', '/', STD_CTYPE_PATH_SEP);
		__STD_CTYPE_GEN('\\', '\\', STD_CTYPE_PATH_SEP);

		int maxChar = -1;
		for (int i = 0; i < countof(data); ++i)
		{
			if (data[i])
			{
				for (int j = maxChar; ++j < i; )
				{
					log.trace("\t0,\t// %c [%d]\n", (j <= ' ' ? ' ' : j), j);
				}
				int first = TRUE;
				for (int k = 0; k < countof(ctypes); ++k)
				{
					if (data[i] & (1 << k))
					{
						log.trace("%sSTD_CTYPE_%s", first ? "\t":"|", ctypes[k]);
						first = FALSE;
					}
				}
				log.trace(",\t// %c [%d]\n", i, i);
				maxChar = i;
			}
		}

		if (0)
		log.trace(
			"#define STD_CTYPE_MAX_CHAR			('%c'+1) /* %d */\n",
			maxChar, maxChar+1);
	}
};

#endif

// -------------------------------------------------------------------------
// $Log: CharType.h,v $
// Revision 1.4  2006/12/03 07:51:57  xushiwei
// IsEOL/NotIsEOL
//
// Revision 1.2  2006/12/01 05:32:54  xushiwei
// STL-Extension: class DigitTable
//
// Revision 1.1  2006/10/18 12:13:39  xushiwei
// stdext as independent component
//
// Revision 1.2  2006/09/26 07:51:00  xushiwei
// STL-Extension:
//  TestCase(WINX_TEST_APP, WINX_TEST_CLASS, WINX_TEST_SUITE, WINX_TEST, WINX_TEST_SUITE_END)
//  UnitTestAssert(AssertExp, AssertEq, AssertEqBuf)
//
// Revision 1.1  2006/08/19 04:40:48  xushiwei
// STL-Extension:
//   Memory(AutoFreeAlloc, RecycleBuffer, AutoArray, etc)
//   String Algorithm(trim, match, compare, etc), Container(SimpleMultiMap), CharType(isCSymbolFirstChar, etc)
//   Log(OutputLog, ErrorLog, FileLog, StringLog), PerformanceCounter, Diagnost(WINX_ASSERT, WINX_RUN_TEST, etc)
//

__NS_STD_END

#endif /* __STDEXT_CHARTYPE_H__ */
