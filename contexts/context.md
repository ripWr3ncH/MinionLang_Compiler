
Compiler Project Context – MinionLang

I am building a compiler project for my Compiler Design lab (CSE).
The project is to design a new toy programming language and implement its lexical analyzer using Flex (and possibly parser using Bison later).

The language I am designing is called MinionLang.

🎯 Goal of the Project

The goal is to:

Design a C-based programming language

Replace C keywords with Minion-themed keywords

Keep the syntax and semantics similar to C

Implement a lexer using Flex

Possibly extend it with parser and translation later

This is an academic compiler lab project.

🧠 Important Design Rules

The language structure is based on C.

Only the keywords and symbols are renamed.

Grammar remains similar to C.

It is not a joke language — it must be lexically and syntactically valid.

The Minion theme is applied only at the lexical level.


All keywords mapping:
SL No	Minion Keyword / Symbol	C Keyword / Symbol	Description
1	#bringy	#include	Include header files
2	#setty	#define	Macro definition
3	banana	int	Integer data type
4	smoothie	float	Floating point data type
5	megasmoot	double	Double precision data type
6	loooong banana	long int	Long integer data type
7	chibi	char	Character data type
8	yesno	bool / _Bool	Boolean data type
9	nada	void	No return type
10	stoppo	break	Break statement
11	papoy()	printf()	Output / print statement
12	takey()	scanf()	Input from user
13	$$	//	Single line comment
14	$*  *$	/* */	Multi-line comment
15	func name(type1 arg1, type2 arg2) :-> rtype	rtype name(type1 arg1, type2 arg2)	Function declaration
16	kaba (condition)	if (condition)	If statement
17	bababa kaba (condition)	else if (condition)	Else-if statement
18	bababa	else	Else statement
19	againa(init : cond : update)	for(init; cond; update)	For loop
20	loopa(condition)	while(condition)	While loop
21	dodo { } loopa(condition)	do { } while(condition)	Do-while loop
22	gooo	continue	Continue statement
23	choosey	switch	Switch statement
24	optiona	case	Switch case
25	otherwise	default	Default case
26	;	;	End of statement
27	add	+	Addition operator
28	minus	-	Subtraction operator
29	mul	*	Multiplication operator
30	divide	/	Division operator
31	mod	%	Modulus operator
32	AND	&&	Logical AND
33	OR	||	Logical OR
34	NOT	!	Logical NOT
35	XOR	^	Logical XOR
36	power()	pow()	Exponentiation function
37	root()	sqrt()	Square root function
38	flr()	floor()	Floor function
39	ceil()	ceil()	Ceiling function
40	abs()	abs()	Absolute value function
41	loggy()	log()	Logarithm function
42	sine()	sin()	Sine function
43	cosine()	cos()	Cosine function
44	tan()	tan()	Tangent function
45	:=	=	Assignment operator
46	<	<	Less than
47	>	>	Greater than
48	<=	<=	Less than or equal
49	>=	>=	Greater than or equal
50	==	==	Equal to
51	!=	!=	Not equal
52	:->	return type	Function return type indicator
53	backa	return	Return statement
54	asine()	asin()	Arc sine
55	acosine()	acos()	Arc cosine
56	atan()	atan()	Arc tangent
57	primey()	—	Prime number checking function
58	namey	[A-Za-z_][A-Za-z0-9_]*	Variable identifier
59	DIGI	[0-9]	Digit
60	FLOATY	[0-9]+.[0-9]+	Floating number
61	cond	< <= > >= == !=	Conditional operators
62	(	(	Opening bracket
63	)	)	Closing bracket
64	{	{	Opening block
65	}	}	Closing block
66	starta()	main()	Main function

🧪 Example MinionLang Program
#bringy <stdio.h>

banana starta() {
    banana x := 10;

    kaba (x > 5) {
        papoy("Value is greater than 5");
    } bababa {
        papoy("Value is small");
    }

    backa 0;
}

⚙️ What I Have to Implement

Using Flex, I need to:

Recognize all Minion keywords

Recognize identifiers

Recognize numbers (integer & float)

Recognize operators (+, -, *, /, etc.)

Recognize relational operators (<, >, <=, >=, ==, !=)

Recognize logical operators (AND, OR, NOT)

Recognize comments

Generate tokens

Later (optional):

Add syntax rules using Bison

Translate MinionLang to C

📚 Academic Justification

This project demonstrates:

Lexical analysis

Token recognition

Keyword mapping

Language design principles

Regular expression usage in Flex

The Minion theme is used only to rename keywords, while the language structure remains formally defined and C-like.

🔑 What I Need Help With

Designing consistent keyword mapping

Writing Flex rules

Structuring grammar

Preparing report content

Explaining the project clearly for viva






