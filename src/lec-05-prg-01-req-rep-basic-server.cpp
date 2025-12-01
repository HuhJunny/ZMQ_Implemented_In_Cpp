// server.cpp
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind("tcp://*:5555");

    while (true) {
        zmq::message_t request;

        socket.recv(request);
        string msg(static_cast<char*>(request.data()), request.size());
        cout << "Received request: " << msg << endl;

        this_thread::sleep_for(chrono::seconds(1));

        string reply = "World";
        socket.send(zmq::buffer(reply));
    }

    return 0;
}
