-module(broadcast).

%% Librería
% Funciones control
-export([start/0,stop/1]).
% Funciones interacción
-export([broadcast/2,registrar/1]).
%%%
-export([loopBroadcast/1]).
-export([loopClient/0]).
-export([test/0,client/1]).


start() ->
  spawn(?MODULE, loopBroadcast, [[]]).

stop(Broadcast) ->
  Broadcast ! stopp.  

broadcast(Broadcast, Msg) ->
  Broadcast ! {brod, Msg}.

registrar(Broadcast) ->
  Broadcast ! {reg, self()}.

loopBroadcast(RegisteredPids) ->
  % Ayuda: Utilizar la función lists:foreach(fun (Pid) -> Pid ! Msg end , PidsList)
  receive 
    stopp -> io:fwrite("Servidor Broadcast terminó ~n",[]);
    {reg, PidClient} -> io:fwrite("El cliente ~p se registró. ~n",[PidClient]),
    loopBroadcast([PidClient|RegisteredPids]);
  % fun ; 'funcion lambda'
    {brod,Msg} -> lists:foreach(fun (Pid) -> Pid ! Msg end , RegisteredPids),
    loopBroadcast(RegisteredPids)
  end.
  
client(Broadcast) ->
  registrar(Broadcast),
  receive 
    Msg -> io:fwrite("El cliente recibió el mensaje: ~p. ~n",[Msg]),
    loopClient()
  end.

loopClient()->
  receive 
    Msg -> 
    io:fwrite("El cliente recibió el mensaje: ~p ~p. ~n",[self(),Msg]),
    loopClient()
  end.

test() ->
  Broadcast= start(),
  spawn(?MODULE, client, [Broadcast]),
  spawn(?MODULE, client, [Broadcast]),
  timer:sleep(1000),
  broadcast(Broadcast, holi),
  stop(Broadcast).