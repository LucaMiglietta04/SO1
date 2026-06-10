-module(client_echo).
-export([cliente/0]).
-define(PUERTO, 1234).
-define(DIR, "localhost").

cliente()->
    case gen_tcp:connect(?DIR, ?PUERTO, [binary, {active,false}]) of
        {ok, Socket} -> gen_tcp:send(Socket, "hola"),
                        case gen_tcp:recv(Socket,0) of
                            {ok, Packet} -> io:fwrite("Cliente haciendo echo ~p ~n",[Packet]),
                            gen_tcp:close(Socket);
                            {error, Reason} -> io:fwrite("Cliente falló el socket con la razón ~p. ~n",[Reason])
                        end;
        {error, Reason} -> io:fwrite("Falló el socket con la razón ~p. ~n",[Reason])
    end.

