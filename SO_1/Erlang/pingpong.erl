-module(pingpong).
-export([pong/0,ping/0,play/0]).



pong()->
	receive
		{0, PidPing} -> 
			io:fwrite ("Chau~n"),
			PidPing ! {fin,self()},
			pongok;
		
		{N , PidPing} -> 
			io:fwrite("HOLA recv: ~p ~n",[N]),
			PidPing ! {(N-1), self()},
		pong()

	end.
ping() -> 
	receive
		{fin , _PidPong} ->
			io:fwrite("Fin ping! ~n"),
			pingok;
		{N,PidPong} ->
			io:fwrite("Ping!"),
		  	PidPong ! {N , self()},
		ping()
			

        end.

play()->
	Toto=spawn(pingpong,ping,[]),               
	
	Mati=spawn(pingpong,pong,[]),

	Toto ! {11, Mati},

	playok.
