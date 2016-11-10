%{
#include <stdio.h>

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

/* Falta contemplar o exponente e, ESTA MAL FEITO ; BE CAREFULLY AMIGO! ! ! ! */
Float				{Digit}+(\.{Digit}+)?(e[+-]{Digit}+){0,1}

String				["].*["]

/*=========*/
/* ESTADOS */
/*=========*/
%x comentarioBloque
%x comentarioAnidado
%x comentarioDocumentacion

%%

{DocumentationCommentStart}                     printf("\033[41m");yymore();;BEGIN(comentarioDocumentacion);
<comentarioDocumentacion>{BlockCommentEnd}      ECHO;printf("\033[0m");BEGIN(INITIAL);
<comentarioDocumentacion>[\n]                   numeroLineas++;yymore();
<comentarioDocumentacion>[^\n]                  yymore();


{BlockCommentStart}				printf("\033[41m");ECHO;BEGIN(comentarioBloque);
<comentarioBloque>{BlockCommentEnd}		ECHO;printf("\033[0m");BEGIN(INITIAL);
<comentarioBloque>[\n]				numeroLineas++;ECHO;
<comentarioBloque>[^\n]				ECHO;

{NestingBlockCommentStart}			{
							printf("\033[41m");
							ECHO;
							anidamiento=1;
							BEGIN(comentarioAnidado);
						}

<comentarioAnidado>{NestingBlockCommentStart}	ECHO;anidamiento++;

<comentarioAnidado>{NestingBlockCommentEnd}	{
							ECHO;	
							anidamiento--;
							if( anidamiento == 0 ){
								printf("\033[0m");
								BEGIN(INITIAL);
							}
						}

<comentarioAnidado>[\n]				numeroLineas++;ECHO;
<comentarioAnidado>[^\n]			ECHO;

{LineComment}					printf("\033[41m");ECHO;printf("\033[0m");	
{Integer}					printf("\033[1;91m");ECHO;printf("\033[0m");
{Float}						printf("\033[1;95m");ECHO;printf("\033[0m");
{String}					printf("\033[42m");ECHO;printf("\033[0m");
{Identifier}					printf("\033[1;93m");ECHO;printf("\033[0m");
[ \t\r]						ECHO;
[\n]						numeroLineas++;ECHO;

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
