-module(server_echo).
-export([start/0, receptor/1, echoResp/1]).
-define(PUERTO, 1234).

start()->
    {ok, Socket}=gen_tcp:listen(?PUERTO, [binary, {active,false}]),
    spawn(?MODULE,receptor,[Socket]),
    okServ.

receptor(Socket)->
    case gen_tcp:accept(Socket) of
        {ok, CSocket} -> spawn(?MODULE, echoResp, [CSocket]);
        {error, closed} -> io:fwrite("Se cerró el socket. ~n",[])
    end,
    receptor(Socket).

echoResp(CSocket)->
    case gen_tcp:recv(CSocket, 0) of
        {ok, Packet} -> 
            io:fwrite("Servidor echo haciendo echo ~p ~n",[Packet]),
            gen_tcp:send(CSocket, Packet);
        {error, Reason} -> io:fwrite("Servidor falló el socket con la razón ~p. ~n",[Reason])
    end.

