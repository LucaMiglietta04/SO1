-module(divZero).
-export([safeDiv/2]).


safeDiv(N,M) -> 
    case catch (N / M) of
        {'EXIT', {Reason, Stack}} -> io:format("División por cero capturada ~p ~n", [Reason]);
        Res -> Res
    end.