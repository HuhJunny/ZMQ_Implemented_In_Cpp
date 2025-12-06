#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

void worker_run(zmq::context_t* context, int id) {
    zmq::socket_t worker(*context, zmq::socket_type::dealer);
    worker.connect("inproc://backend");
    cout << "Worker#" << id << " started" << endl;
    while (true) {
        zmq::message_t ident;
		worker.recv(ident);
		zmq::message_t msg;
		worker.recv(msg);
        string ident_str(static_cast<char*>(ident.data()), ident.size());
        string msg_str(static_cast<char*>(msg.data()), msg.size());
        cout << "Worker#" << id << " received " << msg_str << " from " << ident_str << endl;
        worker.send(ident, zmq::send_flags::sndmore);
        worker.send(msg);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 1;
    }
    int num_server = stoi(argv[1]);

    zmq::context_t context(1);
    zmq::socket_t frontend(context, zmq::socket_type::router);
    frontend.bind("tcp://*:5570");

    zmq::socket_t backend(context, zmq::socket_type::dealer);
    backend.bind("inproc://backend");

    vector<thread> workers;
    for (int i = 0; i < num_server; i++) {
        workers.emplace_back(worker_run, &context, i);
    }

    zmq::proxy(frontend, backend);

    return 0;
}
