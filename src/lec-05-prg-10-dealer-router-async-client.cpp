#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 1;
    }

    string clientID = argv[1];

    zmq::context_t ctx(1);
    zmq::socket_t socket(ctx, zmq::socket_type::dealer);
    socket.set(zmq::sockopt::routing_id, clientID);
    socket.connect("tcp://localhost:5570");
    cout << "Client " << clientID << " started" << endl;
    int reqs = 0;
    while (true) {
        reqs++;
        cout << "Req #" << reqs << " sent.." << endl;
        string request = "request #" + to_string(reqs);
        socket.send(zmq::buffer(request));
        
        zmq::pollitem_t items[] = {{ static_cast<void*>(socket), 0, ZMQ_POLLIN, 0 }};
		zmq::poll(items, 1, 1000);
		if (items[0].revents & ZMQ_POLLIN) {
            zmq::message_t reply;
            socket.recv(reply);
            string msg(static_cast<char*>(reply.data()), reply.size());
            cout << clientID << " received: " << msg << endl;
        }
        this_thread::sleep_for(chrono::seconds(1));
    }

    return 0;
}
