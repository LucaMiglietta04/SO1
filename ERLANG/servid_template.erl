-module(servid_template).
-include("servid.hrl").
%%%%%%
%% Pequeño ejercicio de clase
%% el servidor aceptara 4 tipos de pedidos/mensajes:
%% + nuevoId(Nombre, PidResp) -> Se generará un nuevo identificar para `Nombre` y se responderá al cliente.
%% + buscarId(Id, PidResp) -> Se responde a `PidResp` el nombre asociado a `Id`.
%% + verLista(PidResp) -> Se envía a `Pidresp` la lista de pares (Id,Nombre).
%% + finalizar(PidResp) -> Se finaliza el servicio y se responde con un `ok`.
%%

%% Creación y eliminación del servicio
-export([iniciar/0, finalizar/1]).

%% Servidor
-export([serverinit/1]).

%% Librería de Acceso
-export([nuevoNombre/2, quienEs/2, listaDeIds/1]).

%% Funciones de testing
-export([test/0,client/1]).

% Iniciar crea el proceso servidor, y devuelve el PId.
iniciar() ->
  spawn(?MODULE,serverinit,[self()]).

%%%%%%%%%%%%%% Librería de Acceso
%% Dado un nombre y un servidor le pide que cree un identificador único.
nuevoNombre(Nombre, Servidor) ->
  Servidor ! #nuevoId{nombre = Nombre, clientId = self()},
  receive
    {ok, N} -> io:fwrite("Cliente ~p recibe el Id: ~p ~n",[self(),N]);
    _ -> error
  end. 

%% Función que recupera el nombre desde un Id
quienEs(Id, Servidor) ->
  Servidor ! {buscarId, Id, self()},
  receive
  {ok, Nombre} -> io:fwrite("Cliente ~p recibe el nombre ~p para el Id: ~p ~n",[self(), Nombre, Id])
  end.

%% Pedimos la lista completa de nombres e identificadores.
listaDeIds(Servidor) ->
  Servidor ! {verLista, self()},
  receive
    Lista -> io:fwrit("cliente ~p recibe la lista: ~p ~n",[self(), Lista])
    end.

% Ya implementada :D!
finalizar(Servidor) ->
  Servidor ! {finalizar, self()},
  receive
    {ok, termine} -> io:fwrit("cliente ~p recibió que el servidor terminó. ~n",[self()])
  end. 

%%%%%%%%%%% Servidor
%% Función de servidor de nombres.
serverinit(PInit) ->
    PInit ! ok,
    servnombres(maps:new(),0).
    %% Comenzamos con un mapa nuevo, y un contador en 0.


servnombres(Map, N) ->
    receive
        %% Llega una petición para crear un Id para nombre
        #nuevoId{nombre = Nombre, clientId = CId} -> 
          io:fwrite("Recibí la petición del cliente: ~p nuevoID: ~p ~n",[CId, N]),
          CId ! {ok, N},
          servnombres(maps:put(N, Nombre, Map), N+1);
        %% Llega una petición para saber el nombre de tal Id
        {buscarId, NId, CId} -> 
          io:fwrite("Buscando Id: ~p ~n",[NId]),
          case maps:find(NId, Map) of
            {ok, Nombre} -> CId ! {ok, Nombre};
            error -> CId ! {error, buacarId}
          end,
          servnombres(Map, N);
        %% Entrega la lista completa de Ids con Nombres.
        {verLista, CId} -> 
          CId !  maps:to_list(Map),
          servnombres(Map, N);
        %% Cerramos el servidor.
        {finalizar, CId } -> CId ! {ok, termine}
    end.

client(Server) ->
  nuevoNombre(Server, pepa),
  listaDeIds(Server),
  nuevoNombre(Server, pepe),
  listaDeIds(Server),
  quienEs(Server, 1),
  quienEs(Server, 2),
  finalizar(Server),
  ok.

test() ->
  Server = servid:iniciar(),
  spawn(?MODULE, client, [Server]),
  ok.
