all: dec_server dec_client

dec_server : dec_server.cpp
	g++ -o dec_server dec_server.cpp	

dec_client : dec_client.cpp
	g++ -o dec_client dec_client.cpp
clean:
	@- rm *.o dec_server dec_client
	@- echo "Please ignore the above warnings"
