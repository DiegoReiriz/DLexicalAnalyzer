%{
#include <stdio.h>
#include "./LexicalAnalyzer/definitions.h"

int numeroLineas=0;
int anidamiento=0;
%}

Character			[a-zA-Z]

Digit				[0-9]

BlockCommentStart		\/\*

DocumentationCommentStart	{BlockCommentStart}\*[^/]

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

Float				{Digit}+(\.{Digit}+)?(e[+-]{Digit}+){0,1}

String				["].*["]

/*=========*/
/* ESTADOS */
/*=========*/
%x comentarioBloque
%x comentarioAnidado
%x comentarioDocumentacion

%%

{DocumentationCommentStart}                     yymore();BEGIN(comentarioDocumentacion);
<comentarioDocumentacion>{BlockCommentEnd}      BEGIN(INITIAL);return(DOCUMENTATION_COMMENT);
<comentarioDocumentacion>[\n]                   numeroLineas++;yymore();
<comentarioDocumentacion>[^\n]                  yymore();


{BlockCommentStart}				BEGIN(comentarioBloque);
<comentarioBloque>{BlockCommentEnd}		BEGIN(INITIAL);
<comentarioBloque>[\n]				numeroLineas++;
<comentarioBloque>[^\n]				

{NestingBlockCommentStart}			anidamiento=1;BEGIN(comentarioAnidado);

<comentarioAnidado>{NestingBlockCommentStart}	anidamiento++;

<comentarioAnidado>{NestingBlockCommentEnd}	{
							anidamiento--;
							if( anidamiento == 0 ){
								BEGIN(INITIAL);
							}
						}

<comentarioAnidado>[\n]				numeroLineas++;
<comentarioAnidado>[^\n]			/* NOTHING */	

{LineComment}					/* NOTHING */	
{Integer}					return LITERAL_INTEGER;
{Float}						return LITERAL_FLOAT;
{String}					return LITERAL_STRING;
{Identifier}					return IDENTIFIER;
[ \t\r]						/* NOTHING */
[\n]						numeroLineas++;

\/	|
\.	|
&	|
\|	|
-	|
\+	|
\<	|
>	|
!	|
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
\*	|
%	|
\^	|
~	|
@	|
#	return yytext[0];

\+\+	return TOKEN_ADDITION_ADDITION;
==	return TOKEN_EQUALS_EQUALS;
\+=	return TOKEN_ADDITION_EQUALS;

.	/* IGNORES ALL*/


%%
