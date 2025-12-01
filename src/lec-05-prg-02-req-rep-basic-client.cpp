// client.cpp
#include <zmq.hpp>
#include <string>
#include <iostream>

using namespace std;

int main() {
    zmq::context_t context(1);

    cout << "Connecting to hello world server..." << endl;
    zmq::socket_t socket(context, zmq::socket_type::req);
    socket.connect("tcp://localhost:5555");

    for (int i = 0; i < 10; ++i) {
        string msg = "Hello";
        cout << "Sending request " << i << " ..." << endl;

        socket.send(zmq::buffer(msg));

        zmq::message_t reply;
        socket.recv(reply);
        string reply_str(static_cast<char*>(reply.data()), reply.size());
        cout << "Received reply " << i << " [ " << reply_str << " ]" << endl;
    }

    return 0;
}
