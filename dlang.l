%{
#include <stdio.h>
#include "Errors.h"
#include "definitions.h"
int numeroLineas=1;
int anidamiento=0;
%}

Character			        				[a-zA-Z]

Digit				        					[0-9]

BlockCommentStart		    			\/\*

DocumentationCommentStartNL		{BlockCommentStart}\*\n
DocumentationCommentStart			{BlockCommentStart}\*[^/]

BlockCommentEnd			    			\*\/

LineComment			        			\/\/.*[\n\r]

NestingBlockCommentStart			\/\+

NestingBlockCommentEnd				\+\/

Identifier			        			({Character}|[_])+({Character}|[_]|[0-9])*

DecimalInteger			    			({Digit}|[_])+

BinaryInteger			    				((0b)|(0B))[01_]*

HexadecimalInteger		    		((0x)|(0X))[0-9a-fA-F]*

Integer				        				{DecimalInteger}|{BinaryInteger}|{HexadecimalInteger}

Float				        					{Digit}+(\.{Digit}+)?(e[+-]{Digit}+)?

StringStart			        			["]

/*=========*/
/* ESTADOS */
/*=========*/
%x blockComment
%x nestedComment
%x docuentationComment
%x string

%%

{DocumentationCommentStartNL}                   numeroLineas++;yymore();BEGIN(docuentationComment);
{DocumentationCommentStart}                     yymore();BEGIN(docuentationComment);
<docuentationComment>{BlockCommentEnd}      		BEGIN(INITIAL);return(DOCUMENTATION_COMMENT);
<docuentationComment>[\n]                   		numeroLineas++;yymore();
<docuentationComment><<EOF>>                		showError(ERROR_PREMATURE_EOF_COMMENT,numeroLineas);return 0;
<docuentationComment>[^\n<<EOF>>]           		yymore();


{BlockCommentStart}							                BEGIN(blockComment);
<blockComment>{BlockCommentEnd}					        BEGIN(INITIAL);
<blockComment>[\n]  				        						numeroLineas++;
<blockComment><<EOF>>                       		showError(ERROR_PREMATURE_EOF_COMMENT,numeroLineas);return 0;
<blockComment>[^\n]

{NestingBlockCommentStart}			            		anidamiento=1;BEGIN(nestedComment);

<nestedComment>{NestingBlockCommentStart}				anidamiento++;

<nestedComment>{NestingBlockCommentEnd}	    		{
							                        						anidamiento--;
							                        						if( anidamiento == 0 ){
								                        						BEGIN(INITIAL);
							                        						}
						                        						}

<nestedComment>[\n]				            					numeroLineas++;
<nestedComment><<EOF>>                      		showError(ERROR_PREMATURE_EOF_COMMENT,numeroLineas);return 0;
<nestedComment>[^\n]			            					/* NOTHING */

{LineComment}				                    				numeroLineas++;
{Integer}					                    					return LITERAL_INTEGER;
{Float}						                    					return LITERAL_FLOAT;
{StringStart}		                            		yymore();BEGIN(string);

<string>["]                                     BEGIN(INITIAL);return LITERAL_STRING;
<string><<EOF>>                                 showError(ERROR_PREMATURE_EOF_STRING,numeroLineas);return 0;
<string>\n                                      yymore();showError(ERROR_FOUND_NEW_LINE_ON_STRING,numeroLineas);numeroLineas++;

<string>(\\\"|.)                                yymore();


{Identifier}				                    				return IDENTIFIER;
[ \t\r]						                    					/* NOTHING */
[\n]						                    						numeroLineas++;

\/																							|
\.																							|
&																								|
\|																							|
-																								|
\+																							|
\<																							|
>																								|
!																								|
\(																							|
\)																							|
\[																							|
\]																							|
\{																							|
\}																							|
\?																							|
,																								|
;																								|
:																								|
\$																							|
=																								|
\*																							|
%																								|
\^																							|
~																								|
@																								|
#																								return yytext[0];

\+\+																						return TOKEN_ADDITION_ADDITION;
==	    																				return TOKEN_EQUALS_EQUALS;
\+=	    																				return TOKEN_ADDITION_EQUALS;

.	    																					showError(ERROR_UNKNOW_ELEMENT,numeroLineas);


%%
