#include <zmq.hpp>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    cout << "Publishing updates at weather server..." << endl;

    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::pub);
    socket.bind("tcp://*:5556");

    srand(static_cast<unsigned>(time(nullptr)));
    while (true) {

        int zipcode = rand() % 100000 + 1;
        int temperature = (rand() % 216) - 80;
        int relhumidity = (rand() % 50) + 10;

        string update = to_string(zipcode) + " " + to_string(temperature) + " " + to_string(relhumidity);
        socket.send(zmq::buffer(update));
    }

    return 0;
}