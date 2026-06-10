-module(macros_test).
-export([test/0]).
-define(Debug, debug).
-ifdef(Debug).
 -define(DBG(Str,Args), io:format(Str,Args)).
-else.
 -define(DBG(_Str,_Args), ok).
-endif.
test() -> ?DBG("Esto es un ~p~n",[test]).