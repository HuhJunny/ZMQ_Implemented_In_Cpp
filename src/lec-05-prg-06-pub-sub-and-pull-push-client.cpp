#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    zmq::context_t ctx(1);
    zmq::socket_t subscriber(ctx, zmq::socket_type::sub);
    subscriber.set(zmq::sockopt::subscribe, "");
    subscriber.connect("tcp://localhost:5557");
    zmq::socket_t publisher(ctx, zmq::socket_type::push);
    publisher.connect("tcp://localhost:5558");

    srand(static_cast<unsigned>(time(nullptr)));
    while (true) {
        zmq::pollitem_t items[] = {{ static_cast<void*>(subscriber), 0, ZMQ_POLLIN, 0 }};
        zmq::poll(items, 1, 100);

        if (items[0].revents & ZMQ_POLLIN) {
            zmq::message_t msg;
            subscriber.recv(msg);
            string received(static_cast<char*>(msg.data()), msg.size());
            cout << "I: received message " << received << endl;
        }
        else {
            int r = rand() % 100;
            if (r < 10) {
                string s = to_string(r);
                publisher.send(zmq::buffer(s));
                cout << "I: sending message " << r << endl;
            }
        }

        this_thread::sleep_for(chrono::milliseconds(10));
    }

    return 0;
}