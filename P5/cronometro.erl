% Ej. 2 (Temporizacion).
% a) Implemente una funcion wait/1 que tome como argumento una cantidad de milisegundos y espere
% ese tiempo.
% b) Implemente un cronometro que reciba tres argumentos, Fun, Hasta y Periodo y ejecute Fun/0 cada
% Periodo milisegundos hasta que hayan pasado Hasta milisegundos sin bloquear el int´erprete.
% Un caso de prueba serıa:
% cronometro(fun () -> io:fwrite("Tick~n") end, 60000, 5000).
% que imprimirıa Tick cada 5 segundos durante un minuto.

-module(cronometro).
-export([wait/1, cronometro/3]).


wait(N)->
    receive
        after 
            N -> ok
    end.

cronometro(Fun, Hasta, Periodo)->
    case (Hasta =< Periodo) of
        true -> fin;

        false ->
            wait(Periodo),
            Fun(), 
            cronometro (Fun, Hasta-Periodo, Periodo)

    end.

    % Ayuda: Utilizar la función lists:foreach(fun (Pid) -> Pid ! Msg end , PidsList)
    %    fun ; 'funcion lambda'
    % {brod,Msg} -> lists:foreach(fun (Pid) -> Pid ! Msg end , RegisteredPids),

  

