%{
#include <stdio.h>
%}

%%
.	/* se traga todos os caracteres que non son reconocidos en vez de escupilos*/
stop    printf("Stop command received\n");
start   printf("Start command received\n");
%%
