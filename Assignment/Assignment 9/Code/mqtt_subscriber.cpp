#include <iostream>
#include <string>
#include <mqtt/async_client.h>
#include <thread>
#include <fcntl.h>
#include <unistd.h>

const std::string SERVER_ADDRESS("tcp://io.adafruit.com:1883"); // <-- tcp

const std::string CLIENT_ID("cpp_subscriber_client");
const std::string TOPIC("nguyenlamminhhoa/feeds/relay");

const std::string USERNAME = "";
const std::string PASSWORD = "";

int fd = -1;  // File descriptor 

class callback : public virtual mqtt::callback
{
public:
    void message_arrived(mqtt::const_message_ptr msg) override {
        std::string payload = msg->to_string();
        std::cout << "Received message: " << payload << std::endl;

        if (payload == "ON") {
            std::cout << "[ACTION] Relay ON" << std::endl;
            if (fd != -1) write(fd, "ON\n", 3);  //  ON
        } 
        else if (payload == "OFF") {
            std::cout << "[ACTION] Relay OFF" << std::endl;
            if (fd != -1) write(fd, "OFF\n", 4); //  OFF
        }
    }
};

int main() {
    //Serial port 1 
    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        std::cerr << "Can not open serial port!" << std::endl;
    } else {
        std::cout << "Serial port open complete." << std::endl;
    }

    mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);

    callback cb;
    client.set_callback(cb);

    mqtt::connect_options connOpts;
    connOpts.set_user_name(USERNAME);
    connOpts.set_password(PASSWORD);

    try {
        client.connect(connOpts)->wait();
        client.subscribe(TOPIC, 1)->wait();
        std::cout << "Subscribed to topic: " << TOPIC << std::endl;

        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        return 1;
    }

    // close
    if (fd != -1) close(fd);

    return 0;
}
