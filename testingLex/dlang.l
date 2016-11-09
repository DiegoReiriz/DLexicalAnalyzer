%{
#include <stdio.h>
%}

Character			[a-zA-Z]

Digit				[0-9]

BlockComment			\/\*.**\*\/

LineComment			\/\/.*[\n\r]

NestingBlockCommentCharacters	({Character}*)

NestingBlockComment		\/\+{NestingBlockCommentCharacters}\+\/

Comment				{BlockComment}|{LineComment}|{NestingBlockComment}

/*comprobar que funciona*/
Identifier			({Character}|[_])+({Character}|[_]|[0-9])*

DecimalInteger			({Digit}|[_])+

BinaryInteger			((0b)|(0B))[01_]*

HexadecimalInteger		((0x)|(0X))[0-9a-fA-F]*

Integer				{DecimalInteger}|{BinaryInteger}|{HexadecimalInteger}

/* Falta contemplar o exponente e*/
Float				{Digit}+\.{Digit}+

String				["].*["]

%%

{Comment}		printf("\033[41m");ECHO;printf("\033[0m");	
{Integer}		printf("\033[1;91m");ECHO;printf("\033[0m");
{Float}			printf("\033[1;95m");ECHO;printf("\033[0m");
{String}		printf("\033[42m");ECHO;printf("\033[0m");
{Identifier}		printf("\033[1;93m");ECHO;printf("\033[0m");
[ \n\t\r]		ECHO;

\/	|
\/=	|
\.	|
\.\.	|
\.\.\.	|
&	|
&=	|
&&	|
\|	|
\|=	|
\|\|	|
-	|
-=	|
--	|
\+	|
\+=	|
\+\+	|
\<	|
\<=	|
\<\<	|
\<\<=	|
\<>	|
\<>=	|
>	|
>=	|
>>=	|
>>>=	|
>>	|
>>>	|
!	|
!=	|
!<>	|
!<>=	|
!<	|
!<=	|
!>	|
!>=	|
\(	|
\)	|
\[	|
\]	|
\{	|
\}	|
\?	|
,	|
;	|
:	|
\$	|
=	|
==	|
\*	|
\*=	|
%	|
%=	|
\^	|
\^=	|
\^\^	|
\^\^=	|
~	|
~=	|
@	|
=>	|
#	printf("\033[44m");ECHO;printf("\033[0m");


.		/*TRAGA TODO*/


%%
