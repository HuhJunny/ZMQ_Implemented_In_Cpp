#include <zmq.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <thread>
#include <chrono>

using namespace std;

vector<string> split(const string &s) {
    vector<string> result;
    stringstream ss(s);
    string item;
    while (ss >> item) result.push_back(item);
    return result;
}

using namespace std;
int main(int argc, char** argv) {
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::sub);

    cout << "Collecting updates from weather server..." << endl;
    socket.connect("tcp://localhost:5556");

    string zip_filter = "10001";
    if (argc > 1) zip_filter = argv[1];
    socket.set(zmq::sockopt::subscribe, zip_filter);

    int total_temp = 0;
    int count = 0;

    for (int i = 0; i < 20; ++i) {
        zmq::message_t msg;
        socket.recv(msg);

        string received(static_cast<char*>(msg.data()), msg.size());
        vector<string> parts = split(received);

        if (parts.size() == 3) {
            int temperature = stoi(parts[1]);
            total_temp += temperature;
            count++;
            cout << "Received temperature for zipcode '"
                << zip_filter << "' was " << temperature << " F\n";
        }
    }

    double avg = static_cast<double>(total_temp) / count;
    cout << "\nAverage temperature for zipcode '"
        << zip_filter << "' was " << avg << " F\n";

    cout << "\nPress Ctrl+C to exit.\n";
    while (true) {
        this_thread::sleep_for(chrono::seconds(1));
    }

    return 0;
}