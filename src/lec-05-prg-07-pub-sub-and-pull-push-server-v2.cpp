#include <zmq.hpp>
#include <string>
#include <iostream>

using namespace std;

int main() {
    zmq::context_t ctx(1);
    zmq::socket_t publisher(ctx, zmq::socket_type::pub);
    publisher.bind("tcp://*:5557");
    zmq::socket_t collector(ctx, zmq::socket_type::pull);
    collector.bind("tcp://*:5558");

    while (true) {
        zmq::message_t msg;
        collector.recv(msg);
        string received(static_cast<char*>(msg.data()), msg.size());
        cout << "server: publishing update => " << received << endl;
        publisher.send(msg);
    }

    return 0;
}