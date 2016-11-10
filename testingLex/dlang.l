%{
#include <stdio.h>

int numeroLineas=0;
%}

Character			[a-zA-Z]

Digit				[0-9]

BlockCommentStart		\/\*

BlockCommentEnd			\*\/

LineComment			\/\/.*[\n\r]

NestingBlockCommentStart	\/\+

NestingBlockCommentEnd		\+\/

/*comprobar que funciona*/
Identifier			({Character}|[_])+({Character}|[_]|[0-9])*

DecimalInteger			({Digit}|[_])+

BinaryInteger			((0b)|(0B))[01_]*

HexadecimalInteger		((0x)|(0X))[0-9a-fA-F]*

Integer				{DecimalInteger}|{BinaryInteger}|{HexadecimalInteger}

/* Falta contemplar o exponente e*/
Float				{Digit}+\.{Digit}+(e[+-]{Digit}+){0,1}

String				["].*["]

/*=========*/
/* ESTADOS */
/*=========*/
%x comentarioBloque
%x comentarioAnidado
%x comentarioDocumentacion

%%
{BlockCommentStart}			printf("\033[41m");ECHO;BEGIN(comentarioBloque);
<comentarioBloque>{BlockCommentEnd}	ECHO;printf("\033[0m");BEGIN(INITIAL);
<comentarioBloque>[\n]			numeroLineas++;ECHO;
<comentarioBloque>[^\n]			ECHO;
{LineComment}				printf("\033[41m");ECHO;printf("\033[0m");	
{Integer}				printf("\033[1;91m");ECHO;printf("\033[0m");
{Float}					printf("\033[1;95m");ECHO;printf("\033[0m");
{String}				printf("\033[42m");ECHO;printf("\033[0m");
{Identifier}				printf("\033[1;93m");ECHO;printf("\033[0m");
[ \t\r]					ECHO;
[\n]					numeroLineas++;ECHO;

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


.	/*TRAGA TODO*/


%%
