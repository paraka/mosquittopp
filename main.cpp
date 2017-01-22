#include <iostream>
#include <thread>
#include "MQTTClient.h"

void test_mqtt_certs(const std::string &host, unsigned int port)
{
    MQTTClient mqtt_client(host, port);

    mqtt_client.set_auth_method(AUTH_METHOD::CERTS);

    if (!mqtt_client.set_credentials("usuario", "usuario"))
        std::cout << "Error setting credentials" << std::endl;

    if (!mqtt_client.set_cafile("ca.crt"))
        std::cout << "Error setting CA key" << std::endl;

    if (!mqtt_client.connect())
        std::cout << "Error connecting" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    if (!mqtt_client.subscribe("topic"))
        std::cout << "Error subscribing to topic" << std::endl;

    if (!mqtt_client.publish_message("topic", "Hola mundo"))
        std::cout << "Error publishing msg" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    if (!mqtt_client.unsubscribe("topic"))
        std::cout << "Error unsubscribing from topic" << std::endl;

    if (!mqtt_client.publish_message("topic", "Adios mundo"))
        std::cout << "Error publishing msg" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void test_mqtt_psk(const std::string &host, unsigned int port)
{
    MQTTClient mqtt_client(host, port);

    mqtt_client.set_auth_method(AUTH_METHOD::PSK);

    if (!mqtt_client.set_psk("usuario", "70617373776f726473757065726368756e6761"))
        std::cout << "Error setting PSK key" << std::endl;

    if (!mqtt_client.connect())
        std::cout << "Error connecting" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    if (!mqtt_client.subscribe("topic"))
        std::cout << "Error subscribing to topic" << std::endl;

    if (!mqtt_client.publish_message("topic", "Hola mundo"))
        std::cout << "Error publishing msg" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    if (!mqtt_client.unsubscribe("topic"))
        std::cout << "Error unsubscribing from topic" << std::endl;

    if (!mqtt_client.publish_message("topic", "Adios mundo"))
        std::cout << "Error publishing msg" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main(int argc, char **argv)
{
    std::cout << "**** Attemping to test MQTT stuff... ****\n\n";

    test_mqtt_psk("localhost", 8883);
    test_mqtt_certs("localhost", 8883);

    std::cout << "Test finished" << std::endl;

    return 0;
}
