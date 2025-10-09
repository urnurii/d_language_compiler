// This is a single-line comment
int a = 10;  // Initializing the variable
/* This is a multi-line comment */
/* Start of the multi-line comment
   This continues on a new line
   and spans several more lines
   Until the end of the comment */
/*
    This is a multi-line comment that continues
    over multiple lines to check the lexer processing
*/
/* This is a* multi-line comment with a *** inside */
/* This comment contains the sequence * and / for testing */
// This /* This is a multi-line comment */ after it
/* This is a multi-line // comment */
/* Escaped character: \t */
/* This /* This is a multi-line comment */ comment */
/*
    This is a multi-line comment that won't be closed
    We need to make sure
int a = 10;