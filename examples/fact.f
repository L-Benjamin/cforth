( Recursive definition of the factorial function )
: fact dup if dup dec fact * else pop 1 end ;

( Asks the user for a number )
" n = " out in strtoi

( Prints the factorial of that number )
"n! = " out fact itostr out endl
