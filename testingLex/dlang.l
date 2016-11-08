%{
#include <stdio.h>
%}

Character	[a-zA-Z]

Characters	{Character}+

BlockComment	\/\*{Characters}*\*\/

LineComment	\/\/{Characters}*$

NestingBlockCommentCharacters	({Characters}*)

NestingBlockComment	\/\+{NestingBlockCommentCharacters}\+\/

Comment	{BlockComment}|{LineComment}|{NestingBlockComment}

/*comprobar que funciona*/
Identifier	({Character}|[_])*({Character}|[_]|[0-9])*

%%

{Identifier}		printf("Identificador\n");
{NestingBlockComment}	printf("Se ha encontrado un comentario\n");
{Character}{1}		printf("Found a Character\n");

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
#	ECHO;


.		/*TRAGA TODO*/


%%
