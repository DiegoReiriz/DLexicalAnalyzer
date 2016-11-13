%{
#include <stdio.h>
#include "Errors.h"
#include "definitions.h"
int numeroLineas=1;
int anidamiento=0;
%}

Character			        [a-zA-Z]

Digit				        [0-9]

BlockCommentStart		    \/\*

DocumentationCommentStartNL	{BlockCommentStart}\*\n
DocumentationCommentStart	{BlockCommentStart}\*[^/]

BlockCommentEnd			    \*\/

LineComment			        \/\/.*[\n\r]

NestingBlockCommentStart	\/\+

NestingBlockCommentEnd		\+\/

Identifier			        ({Character}|[_])+({Character}|[_]|[0-9])*

DecimalInteger			    ({Digit}|[_])+

BinaryInteger			    ((0b)|(0B))[01_]*

HexadecimalInteger		    ((0x)|(0X))[0-9a-fA-F]*

Integer				        {DecimalInteger}|{BinaryInteger}|{HexadecimalInteger}

Float				        {Digit}+(\.{Digit}+)?(e[+-]{Digit}+)?

StringStart			        ["]

/*=========*/
/* ESTADOS */
/*=========*/
%x comentarioBloque
%x comentarioAnidado
%x comentarioDocumentacion
%x string

%%

{DocumentationCommentStartNL}                   numeroLineas++;yymore();BEGIN(comentarioDocumentacion);
{DocumentationCommentStart}                     yymore();BEGIN(comentarioDocumentacion);
<comentarioDocumentacion>{BlockCommentEnd}      BEGIN(INITIAL);return(DOCUMENTATION_COMMENT);
<comentarioDocumentacion>[\n]                   numeroLineas++;yymore();
<comentarioDocumentacion><<EOF>>                showError(ERROR_PREMATURE_EOF_COMMENT,numeroLineas);return 0;
<comentarioDocumentacion>[^\n<<EOF>>]           yymore();


{BlockCommentStart}				                BEGIN(comentarioBloque);
<comentarioBloque>{BlockCommentEnd}		        BEGIN(INITIAL);
<comentarioBloque>[\n]  				        numeroLineas++;
<comentarioBloque><<EOF>>                       showError(ERROR_PREMATURE_EOF_COMMENT,numeroLineas);return 0;
<comentarioBloque>[^\n]				

{NestingBlockCommentStart}			            anidamiento=1;BEGIN(comentarioAnidado);

<comentarioAnidado>{NestingBlockCommentStart}	anidamiento++;

<comentarioAnidado>{NestingBlockCommentEnd}	    {
							                        anidamiento--;
							                        if( anidamiento == 0 ){
								                        BEGIN(INITIAL);
							                        }
						                        }

<comentarioAnidado>[\n]				            numeroLineas++;
<comentarioAnidado><<EOF>>                      showError(ERROR_PREMATURE_EOF_COMMENT,numeroLineas);return 0;
<comentarioAnidado>[^\n]			            /* NOTHING */

{LineComment}				                    numeroLineas++;
{Integer}					                    return LITERAL_INTEGER;
{Float}						                    return LITERAL_FLOAT;
{StringStart}		                            yymore();BEGIN(string);

<string>["]                                     BEGIN(INITIAL);return LITERAL_STRING;
<string><<EOF>>                                 showError(ERROR_PREMATURE_EOF_STRING,numeroLineas);return 0;
<string>\n                                      yymore();showError(ERROR_FOUND_NEW_LINE_ON_STRING,numeroLineas);numeroLineas++;

<string>(\\\"|.)                                yymore();


{Identifier}				                    return IDENTIFIER;
[ \t\r]						                    /* NOTHING */
[\n]						                    numeroLineas++;

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
==	    return TOKEN_EQUALS_EQUALS;
\+=	    return TOKEN_ADDITION_EQUALS;

.	    showError(ERROR_UNKNOW_ELEMENT,numeroLineas);


%%
