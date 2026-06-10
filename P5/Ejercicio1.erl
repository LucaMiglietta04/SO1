%%a) Justifique cuales match de la funcion match_test deberian ser validos y cuales no.

% b) Implemente las funciones nombre/1 y apellido/1 para que devuelvan esos campos de las tuplas
% que obtienen como argumento utilizando pattern matching.

% c) Explique el resultado de cada una de las comparaciones de la funcion string_test/0 (es decir por
% que dan true o false).

% d) Implemente la funcion filtrar_por_apellido/2 para que devuelva los nombres (sin el apellido)
% de las personas de la lista Personas cuyo apellido coincide con Apellido utilizando comprensi´on de
% listas.

-module(intro).
-export([init/0]).

match_test () ->
    {A,B} = {5,4},
    {C,C} = {5,4},
    {B,A} = {4,5},
    {D,D} = {5,5}.

string_test () -> [
    helloworld == 'helloworld'
    "helloworld" < 'helloworld',
    helloworld == "helloworld",
    [$h,$e,$l,$l,$o,$w,$o,$r,$l,$d] == "helloworld",
    [104,101,108,108,111,119,111,114,108,100] < {104,101,108,108,111,119,111,114,108,100},
    [104,101,108,108,111,119,111,114,108,100] > 1,
    [104,101,108,108,111,119,111,114,108,100] == "helloworld"].

tuple_test (P1, P2) ->
    io:fwrite("El nombre de P1 es ~p y el apellido de P2 es ~p~n", [nombre(P1), apellido(P2)]).

apellido (P) -> ok.

nombre (P) -> ok.

filtrar_por_apellido(Personas, Apellido) -> ok.

init () ->
    P1 = {persona, {nombre, "Juan"}, {apellido, "Gomez"}},
    P2 = {persona, {nombre, "Carlos"}, {apellido, "Garcia"}},
    P3 = {persona, {nombre, "Javier"}, {apellido, "Garcia"}},
    P4 = {persona, {nombre, "Rolando"}, {apellido, "Garcia"}},
    match_test(),
    tuple_test(P1, P2),
    string_test(),

Garcias = filtrar_por_apellido([P4, P3, P2, P1], "Garcia").