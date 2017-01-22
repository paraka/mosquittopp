#ifndef _MQTT_CLIENT_H_
#define _MQTT_CLIENT_H_

#include <string>
#include <memory>

enum class AUTH_METHOD
{
    PLAIN, //default
    PSK,
    CERTS,
};

class MQTTClient
{
public:
    explicit MQTTClient(const std::string &host, 
               unsigned int port = DEFAULT_PORT,
               unsigned int keepalive = DEFAULT_KEEP_ALIVE);

    ~MQTTClient();

    // Set authentication method
    void set_auth_method(AUTH_METHOD method);

    // This must be set before connect
    bool set_credentials(const std::string &user, const std::string &password);

    // This must be set before connect
    bool set_cafile(const std::string &cafile_path);

    // This must be set before connect
    bool set_psk(const std::string &identity, const std::string &psk);

    // asyncronous connections. It will run in a different thread
    bool connect();

    bool publish_message(const std::string &topic, const std::string &msg);

    bool subscribe(const std::string &topic);

    bool unsubscribe(const std::string &topic);

private:
    class MQTTClientImpl;
    using MQTTClientImplPtr = std::unique_ptr<MQTTClientImpl>;
    MQTTClientImplPtr impl_;
    static const unsigned int DEFAULT_PORT = 1883;
    static const unsigned int DEFAULT_KEEP_ALIVE = 60;
};

#endif // _MQTT_CLIENT_H_
