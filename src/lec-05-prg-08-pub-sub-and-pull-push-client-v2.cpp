#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 1;
    }

    zmq::context_t ctx(1);
    zmq::socket_t subscriber(ctx, zmq::socket_type::sub);
    subscriber.set(zmq::sockopt::subscribe, "");
    subscriber.connect("tcp://localhost:5557");
    zmq::socket_t publisher(ctx, zmq::socket_type::push);
    publisher.connect("tcp://localhost:5558");

    string clientID = argv[1];
    srand(static_cast<unsigned>(time(nullptr)));

    while (true) {
        zmq::pollitem_t items[] = {{ static_cast<void*>(subscriber), 0, ZMQ_POLLIN, 0 }};
        zmq::poll(items, 1, 100);

        if (items[0].revents & ZMQ_POLLIN) {
            zmq::message_t msg;
            subscriber.recv(msg);
            string received(static_cast<char*>(msg.data()), msg.size());
            cout << clientID << ": receive status => " << received << endl;
        }
        else {
            int r = rand() % 100;
            if (r < 10) {
                this_thread::sleep_for(chrono::seconds(1));
                string msg = "(" + clientID + ":ON)";
                publisher.send(zmq::buffer(msg));
                cout << clientID << ": send status - activated" << endl;
            }
            else if (r > 90) {
                this_thread::sleep_for(chrono::seconds(1));
                string msg = "(" + clientID + ":OFF)";
                publisher.send(zmq::buffer(msg));
                cout << clientID << ": send status - deactivated" << endl;
            }
        }

        this_thread::sleep_for(chrono::milliseconds(10));
    }

    return 0;
}
