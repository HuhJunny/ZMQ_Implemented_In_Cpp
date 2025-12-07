#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>


#pragma comment(lib, "ws2_32.lib")

using namespace std;

vector<string> split(const string& s, char separater) {
    vector<string> result;
    stringstream ss(s);
    string item;
    while (getline(ss, item, separater)) result.push_back(item);
    return result;
}

string search_nameserver(const string& ip_mask, const string& local_ip_addr, const string& port_nameserver) {
    zmq::context_t ctx(1);
    zmq::socket_t req(ctx, zmq::socket_type::sub);
    req.setsockopt(ZMQ_RCVTIMEO, 2000);
    req.setsockopt(ZMQ_SUBSCRIBE, "NAMESERVER", 10);
    for (int last = 1; last < 255; last++) {
        string target = "tcp://" + ip_mask + "." + to_string(last) + ":" + port_nameserver;
        req.connect(target);
    }
    try {
        zmq::message_t msg;
        req.recv(msg);
        string res((char*)msg.data(), msg.size());
        auto pos = res.find(':');
        if (pos != string::npos && res.substr(0, pos) == "NAMESERVER") {
            return res.substr(pos + 1);
        }
    }
    catch (...) {}
    return "";
}

void beacon_nameserver(const string& local_ip_addr, const string& port_nameserver) {
    zmq::context_t ctx(1);
    zmq::socket_t pub(ctx, zmq::socket_type::pub);
    string addr = "tcp://" + local_ip_addr + ":" + port_nameserver;
    pub.bind(addr);
    cout << "local p2p name server bind to " << addr << endl;
    while (true) {
        this_thread::sleep_for(chrono::seconds(1));
        string msg = "NAMESERVER:" + local_ip_addr;
        pub.send(zmq::buffer(msg));
    }
}

void user_manager_nameserver(const string& local_ip_addr, const string& port_subscribe) {
    vector<pair<string, string>> user_db;
    zmq::context_t ctx(1);
    zmq::socket_t socket(ctx, zmq::socket_type::rep);
    string addr = "tcp://" + local_ip_addr + ":" + port_subscribe;
    socket.bind(addr);
    cout << "local p2p db server activated at " << addr << endl;
    while (true) {
        zmq::message_t req;
        try {
            socket.recv(req);
            string msg((char*)req.data(), req.size());
            auto parts = split(msg, ':');
            if (parts.size() != 2) break;
			string ip = parts[0];
			string username = parts[1];
			user_db.push_back({ ip, username });
            cout << "user registration '" << username << "' from '" << ip << "'." << endl;
            socket.send(zmq::buffer("ok"));
        }
        catch (...) {
            break;
		}
    }
}

void relay_server_nameserver(const string& local_ip_addr, const string& port_chat_publisher, const string& port_chat_collector) {
    zmq::context_t ctx(1);
    zmq::socket_t publisher(ctx, zmq::socket_type::pub);
    publisher.bind("tcp://" + local_ip_addr + ":" + port_chat_publisher);
    zmq::socket_t collector(ctx, zmq::socket_type::pull);
    collector.bind("tcp://" + local_ip_addr + ":" + port_chat_collector);
    cout << "local p2p relay server activated at tcp://" << local_ip_addr << ":" << port_chat_publisher << "&" << port_chat_collector << endl;
    while (true) {
        zmq::message_t msg;
        collector.recv(msg);
        string message((char*)msg.data(), msg.size());
        cout << "p2p-relay:<==>" << message << endl;
        string relay = "RELAY:" + message;
        publisher.send(zmq::buffer(relay));
    }
}

string get_local_ip() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    char hostname[256];
    gethostname(hostname, sizeof(hostname));

    addrinfo hints = {};
    hints.ai_family = AF_INET;

    addrinfo* info;
    getaddrinfo(hostname, nullptr, &hints, &info);

    string ip = "127.0.0.1";
    for (auto p = info; p != nullptr; p = p->ai_next) {
        sockaddr_in* addr = (sockaddr_in*)p->ai_addr;
        ip = inet_ntoa(addr->sin_addr);
        break;
    }

    freeaddrinfo(info);
    WSACleanup();
    return ip;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "usage is 'python dechat.py _user-name_'." << endl;
        return 0;
    }
    cout << "starting p2p chatting program." << endl;

    string ip_addr_p2p_server = "";
	string port_nameserver = "9001";
	string port_chat_publisher = "9002";
	string port_chat_collector = "9003";
	string port_subscribe = "9004";

    string user_name = argv[1];
    string ip_addr = get_local_ip();
    string ip_mask = ip_addr.substr(0, ip_addr.rfind('.'));

    cout << "searching for p2p server." << endl;

    string name_server_ip_addr = search_nameserver(ip_mask, ip_addr, port_nameserver);
    if (name_server_ip_addr.empty()) {
		ip_addr_p2p_server = ip_addr;
        cout << "p2p server is not found, and p2p server mode is activated." << endl;
        thread beacon_thread(beacon_nameserver, ip_addr, port_nameserver);
		beacon_thread.detach();
		cout << "p2p beacon server is activated." << endl;
        thread db_thread(user_manager_nameserver, ip_addr, port_subscribe);
		db_thread.detach();
		cout << "p2p subscriber database server is activated." << endl;
        thread relay_thread(relay_server_nameserver, ip_addr, port_chat_publisher, port_chat_collector);
		relay_thread.detach();
		cout << "p2p message relay server is activated." << endl;
    }
    else {
		ip_addr_p2p_server = name_server_ip_addr;
        cout << "p2p server found at " << name_server_ip_addr << ", and p2p client mode is activated." << endl;
    }

	cout << "starting user registration procedure." << endl;

    zmq::context_t db_client_ctx(1);
    zmq::socket_t db_client_socket(db_client_ctx, zmq::socket_type::req);
    db_client_socket.connect("tcp://" + ip_addr_p2p_server + ":" + port_subscribe);
    string reg = ip_addr + ":" + user_name;
    db_client_socket.send(zmq::buffer(reg));
    zmq::message_t rep;
    db_client_socket.recv(rep);
    cout << "user registration to p2p server completed." << endl;

	cout << "starting message transfer procedure." << endl;

    zmq::context_t relay_client(1);
    zmq::socket_t p2p_rx(relay_client, zmq::socket_type::sub);
    p2p_rx.setsockopt(ZMQ_SUBSCRIBE, "RELAY", 5);
    p2p_rx.connect("tcp://" + ip_addr_p2p_server + ":" + port_chat_publisher);
    zmq::socket_t p2p_tx(relay_client, zmq::socket_type::push);
    p2p_tx.connect("tcp://" + ip_addr_p2p_server + ":" + port_chat_collector);

    cout << "starting autonomous message transmit and receive scenario." << endl;

    while (true) {
        zmq::pollitem_t items[] = {{ static_cast<void*>(p2p_rx), 0, ZMQ_POLLIN, 0 }};
        zmq::poll(items, 1, 100);

        if (items[0].revents & ZMQ_POLLIN) {
            zmq::message_t msg;
            p2p_rx.recv(msg);
            string message((char*)msg.data(), msg.size());
			auto parts = split(message, ':');
            cout << "p2p-recv::<<== " << parts[1] << ":" << parts[2] << endl;
        }
        else {
            int r = rand() % 100;

            if (r < 10 || r > 90) {
                this_thread::sleep_for(chrono::seconds(3));
                string msg = "(" + user_name + "," + ip_addr + string(r < 10 ? ":ON)" : ":OFF)");
                p2p_tx.send(zmq::buffer(msg));
                cout << "p2p-send::==>>" << msg << endl;
            }
        }
    }

	cout << "closing p2p chatting program." << endl;

	db_client_socket.close();
	p2p_rx.close();
	p2p_tx.close();
    db_client_ctx.shutdown();
	db_client_ctx.close();
	relay_client.shutdown();
	relay_client.close();

    return 0;
}