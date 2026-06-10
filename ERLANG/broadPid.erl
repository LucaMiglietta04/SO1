-module(broadPid).

%% Librería
% Funciones control
-export([start/0, stop/0]).
% Funciones interacción
-export([broadcast/1, registrar/0]).
%%%
-export([loopBroadcast/1]).
-export([loopClient/0]).
-export([test/0, client/0]).


start() ->
    BroadcastPid = spawn(?MODULE, loopBroadcast, [[]]),
    register(servidor_brod, BroadcastPid). 
stop() ->
    servidor_brod ! stopp.
  
broadcast(Msg) ->
    servidor_brod ! {brod, Msg}.

registrar() ->
    servidor_brod ! {reg, self()}.

loopBroadcast(RegisteredPids) ->
    receive 
        stopp -> 
            io:fwrite("Servidor Broadcast terminó ~n", []);
        {reg, PidClient} -> 
            io:fwrite("El cliente ~p se registró. ~n", [PidClient]),
            loopBroadcast([PidClient | RegisteredPids]);
        {brod, Msg} -> 
            lists:foreach(fun (Pid) -> Pid ! Msg end, RegisteredPids),
            loopBroadcast(RegisteredPids)
    end.
  
loopClient() ->
    receive 
        Msg -> 
            io:fwrite("El cliente ~p recibió el mensaje en loop: ~p. ~n", [self(), Msg]),
            loopClient()
    end.

client() ->
    registrar(), 
    receive 
        Msg -> 
            io:fwrite("El cliente ~p recibió el mensaje inicial: ~p. ~n", [self(), Msg]),
            loopClient()
    end.

test() ->
    start(), 
    spawn(?MODULE, client, []), 
    spawn(?MODULE, client, []),
    timer:sleep(1000),
    broadcast(holi),
    timer:sleep(1000), 
    stop().