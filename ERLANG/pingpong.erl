-module(pingpong).
-export([])

% Proceso Pong
pong()-> 
    receive 
    {N, PidPing} -> io:fwrite("Pong received: guion dobladito p guion dobladito n", [N])

% Proceso Ping 
ping()->


play()->
    PidPing = spawn(pingpong, ping, []),  
    PidPong = spawn(pingpong, pong, []),
    PidPong ! {10, PidPing},
    playok. 
