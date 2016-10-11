//
// Created by diegoreiriz on 10/10/16.
//

#ifndef ANALIZADORLEXICO_DEFINITIONS_H
#define ANALIZADORLEXICO_DEFINITIONS_H

/*Symbols*/

#define abstract 273
#define alias 274
#define align 275
#define asm 276
#define assert 277
#define auto 278
#define body 279
#define bool 280
#define break 281
#define byte 282
#define case 283
#define cast 284
#define catch 285
#define cdouble 286
#define cent 287
#define cfloat 288
#define char 289
#define class 290
#define const 291
#define continue 292
#define creal 293
#define dchar 294
#define debug 295
#define default 296
#define delegate 297
#define deprecated 298
#define do 299
#define double 300
#define else 301
#define enum 302
#define export 303
#define extern 304
#define false 305
#define final 306
#define finally 307
#define float 308
#define for 309
#define foreach 310
#define foreach_reverse 311
#define function 312
#define goto 313
#define idouble 314
#define if 315
#define ifloat 316
#define immutable 317
#define import 318
#define in 319
#define inout 320
#define int 321
#define interface 322
#define invariant 323
#define ireal 324
#define is 325
#define lazy 326
#define long 327
#define macro 328
#define mixin 329
#define module 330
#define new 331
#define nothrow 332
#define null 333
#define out 334
#define override 335
#define package 336
#define pragma 337
#define private 338
#define protected 339
#define public 340
#define pure 341
#define real 342
#define ref 343
#define return 344
#define scope 345
#define shared 346
#define short 347
#define static 348
#define struct 349
#define super 350
#define switch 351
#define synchronized 352
#define template 353
#define this 354
#define throw 355
#define true 356
#define try 357
#define typeid 358
#define typeof 359
#define ubyte 360
#define ucent 361
#define uint 362
#define ulong 363
#define union 364
#define unittest 365
#define ushort 366
#define version 367
#define void 368
#define wchar 369
#define while 370
#define with 371
#define __FILE__ 372
#define __FILE_FULL_PATH__ 373
#define __MODULE__ 374
#define __LINE__ 375
#define __FUNCTION__ 376
#define __PRETTY_FUNCTION__ 377
#define __gshared 378
#define __traits 379
#define __vector 380
#define __parameters 381

/*Globally defined symbols*/

#define string 382 /*(alias to immutable(char)[])*/
#define wstring 383 /*(alias to immutable(wchar)[])*/
#define dstring 384 /*(alias to immutable(dchar)[])*/
#define size_t 385
#define ptrdiff_t 386

//check if all this is needed

//define RESERVED_WORD 0 -- TODO: it is really needed?

#define IDENTIFIER 387
#define END_OF_FILE $ //original value 388, changed by convention
#define END_OF_LINE $ //original value 389, changed by convention
#define WHITE_SPACE 390
#define CHAR_LITERAL 391
#define COMMENT 392
#define TOKEN 393
#define ESCAPE_CHARACTERS 394
#define INTEGERS 395
#define FLOATING_POINT_NUMBER 396

#endif //ANALIZADORLEXICO_DEFINITIONS_H
