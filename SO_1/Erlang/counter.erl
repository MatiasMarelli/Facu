-module(counter).
-export([count/0,client/3,play/0]).

count() ->
	receive 
		fin -> counterok;
		{N,sumar,client} ->
			io:fwrite("sume~n"),
			client ! {N+1,self()},
			count();	
		{N,print,client} ->
			io:fwrite("Numero :~p~n",N),
			client ! {N,self()},
			count()			
	end.
	


client([],_,serv) -> 
	serv ! fin,
	io:fwrite("Se acabo"),
	clientok;
client([Hd|Tl],n,pid) ->

	pid ! {n,Hd,self()},
	receive 
		{new_n,serv} -> client(Tl,new_n,serv)
	end.


play() ->
	serv=spawn (counter,count,[]),

	cli=spawn (counter,client,[[sumar,print,sumar,print,sumar,print],0,serv]),

	playok.

