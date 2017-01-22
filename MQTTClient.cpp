#include <iostream>
#include "mosquittopp.h"
#include "MQTTClient.h"

class MQTTClient::MQTTClientImpl : public mosqpp::mosquittopp
{
public:
    MQTTClientImpl(const std::string &host,
                   unsigned int port,
                   unsigned int keepalive):
                                        host_(host),
                                        port_(port),
                                        keepalive_(keepalive),
                                        auth_method_(AUTH_METHOD::PLAIN)
    {
        init();
    }

    ~MQTTClientImpl()
    {
        std::cout << "~MQTTClientImpl" << std::endl;
        fini();
    }

    void set_auth_method(AUTH_METHOD method)
    {
        auth_method_ = method;
    }

    bool set_credentials(const std::string &user, const std::string &password)
    {
        int ret = username_pw_set(user.c_str(), password.c_str());
        return (ret == MOSQ_ERR_SUCCESS);
    }

    bool set_cafile(const std::string &cafile_path)
    {
        if (auth_method_ != AUTH_METHOD::CERTS)
            throw std::runtime_error("No method allowed for selected authentication method");

        int ret = tls_set(cafile_path.c_str());
        return (ret == MOSQ_ERR_SUCCESS);
    }

    bool set_psk(const std::string &identity, const std::string &psk)
    {
        if (auth_method_ != AUTH_METHOD::PSK)
            throw std::runtime_error("No method allowed for selected authentication method");

        int ret = tls_psk_set(psk.c_str(), identity.c_str());
        return (ret == MOSQ_ERR_SUCCESS);
    }

    bool connect()
    {
        bool ret = false;

        // non blocking connection to broker request
        int rc = connect_async(host_.c_str(), port_, keepalive_);
        if (rc == MOSQ_ERR_SUCCESS)
        {
            ret = true;
            loop_start();
        }

        return ret;
    }

    bool publish_message(const std::string &topic, const std::string &msg)
    {
        int ret = publish(nullptr, topic.c_str(), msg.size(), msg.c_str(), 1, false);
        return (ret == MOSQ_ERR_SUCCESS);
    }

    bool subscribe_to_topic(const std::string &topic)
    {
        int ret = subscribe(nullptr, topic.c_str());
        return (ret == MOSQ_ERR_SUCCESS);
    }

    bool unsubscribe_from_topic(const std::string &topic)
    {
        int ret = unsubscribe(nullptr, topic.c_str());
        return (ret == MOSQ_ERR_SUCCESS);
    }

private:
    void init()
    {
        mosqpp::lib_init();     
    }

    void fini()
    {
        // force exit
        loop_stop(true);
        mosqpp::lib_cleanup(); 
    }

    // all of these can be overwritten

    void on_connect(int rc) override
    {
        std::cout << "MQTTClientImpl - on_connect (" << rc << ")" << std::endl;
    }

    void on_disconnect(int rc) override
    {
        std::cout << "MQTTClientImpl - on_disconnect (" << rc << ")" << std::endl;
    }

    void on_publish(int mid) override
    {
        std::cout << "MQTTClientImpl - on_publish (" << mid << ")" << std::endl;
    }

    void on_message(const struct mosquitto_message *message) override
    {
        std::cout << "MQTTClientImpl - on_message(" << message->mid 
                    << " topic: " << std::string(message->topic) << " - message: "
                    << std::string((char *)message->payload, message->payloadlen) << ")" << std::endl;
    }

    void on_subscribe(int mid, int qos_count, const int *granted_qos) override
    {
        std::cout << "MQTTClientImpl - on_subscribe(" << mid << ")" << std::endl;
    }

    void on_unsubscribe(int mid) override
    {
        std::cout << "MQTTClientImpl - on_unsubscribe(" << mid << ")" << std::endl;
    }

    void on_log(int level, const char *str) override
    {
        std::cout << "MQTTClientImpl - on_log (" 
                  << level << " " << std::string(str) << ")" << std::endl;
    }

    void on_error() override
    {
        std::cout << "MQTTClientImpl - on_error ()" << std::endl;
    }

private:
    const std::string host_;
    unsigned int port_;
    unsigned int keepalive_;
    AUTH_METHOD auth_method_;
 };

/* implementation */

MQTTClient::MQTTClient(const std::string &host,
                        unsigned int port,
                        unsigned int keepalive)
{
    impl_ = std::make_unique<MQTTClientImpl>(host, port, keepalive);
}

MQTTClient::~MQTTClient() = default;

void MQTTClient::set_auth_method(AUTH_METHOD method)
{
    impl_->set_auth_method(method);
}

bool MQTTClient::set_credentials(const std::string &user, const std::string &password)
{
    return impl_->set_credentials(user, password);
}

bool MQTTClient::set_psk(const std::string &identity, const std::string &psk)
{
    return impl_->set_psk(identity, psk);
}

bool MQTTClient::set_cafile(const std::string &cafile_path)
{
    return impl_->set_cafile(cafile_path);
}

bool MQTTClient::connect()
{
    return impl_->connect();
}

bool MQTTClient::publish_message(const std::string &topic, const std::string &msg)
{
    return impl_->publish_message(topic, msg);
}

bool MQTTClient::subscribe(const std::string &topic)
{
    return impl_->subscribe_to_topic(topic);
}

bool MQTTClient::unsubscribe(const std::string &topic)
{
    return impl_->unsubscribe_from_topic(topic);
}
