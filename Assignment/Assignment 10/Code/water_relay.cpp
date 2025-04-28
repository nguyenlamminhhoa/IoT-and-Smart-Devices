#include <iostream>
#include <string>
#include <mqtt/async_client.h>
#include <thread>
#include <fcntl.h>
#include <unistd.h>

const std::string SERVER_ADDRESS("tcp://io.adafruit.com:1883");
const std::string CLIENT_ID("cpp_client_water_relay");
const std::string TOPIC_RELAY("nguyenlamminhhoa/feeds/relaycontrol");
const std::string TOPIC_SENSOR("nguyenlamminhhoa/feeds/watersensor");

const std::string USERNAME = "xxxx";
const std::string PASSWORD = "xxxx";

int fd = -1; // Serial port

class callback : public virtual mqtt::callback
{
public:
    void message_arrived(mqtt::const_message_ptr msg) override {
        std::string payload = msg->to_string();
        std::cout << "[MQTT Relay Command] " << payload << std::endl;

        if (payload == "ON") {
            if (fd != -1) write(fd, "ON\n", 3);
        } 
        else if (payload == "OFF") {
            if (fd != -1) write(fd, "OFF\n", 4);
        }
    }
};

int main() {
    // Open serial port
    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        std::cerr << "Cannot open serial port!" << std::endl;
        return 1;
    }
    std::cout << "Serial port opened." << std::endl;

    mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);
    callback cb;
    client.set_callback(cb);

    mqtt::connect_options connOpts;
    connOpts.set_user_name(USERNAME);
    connOpts.set_password(PASSWORD);

    try {
        client.connect(connOpts)->wait();
        client.subscribe(TOPIC_RELAY, 1)->wait();
        std::cout << "Connected and subscribed to " << TOPIC_RELAY << std::endl;

        char buf[100];
        while (true) {
            int n = read(fd, buf, sizeof(buf) - 1);
            if (n > 0) {
                buf[n] = '\0';
                std::string sensorValue(buf);
                sensorValue.erase(sensorValue.find_last_not_of(" \n\r\t")+1); // Trim
                std::cout << "[Sensor] " << sensorValue << std::endl;

                if (!sensorValue.empty()) {
                    auto msg = mqtt::make_message(TOPIC_SENSOR, sensorValue);
                    msg->set_qos(1);
                    client.publish(msg);
                }
                else {
                    std::cout << "[Warning] Empty sensor value, skip sending MQTT." << std::endl;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "MQTT Error: " << exc.what() << std::endl;
        return 1;
    }

    if (fd != -1) close(fd);
    return 0;
}
