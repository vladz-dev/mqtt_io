#pragma once

#include <memory>
#include <string>

namespace config {

class Configuration
{
public:
    static std::shared_ptr<Configuration> instance();

    std::string testDataDirectory() const;
    std::string mqttServerHostName() const;
    uint16_t mqttServerPort() const;
    std::string mqttClientRequestTopic() const;
    std::string mqttJobHandlerSubscriptionTopic() const;

private:
    Configuration(std::string testDataDirectory,
                  std::string mqttServerHostName,
                  uint16_t mqttServerPort,
                  std::string mqttClientRequestTopic,
                  std::string mqttJobHandlerSubscriptionTopic);

    std::string mTestDataDirectory;
    std::string mMqttServerHostName;
    uint16_t mMqttServerPort;
    std::string mMqttClientRequestTopic;
    std::string mMqttJobHandlerSubscriptionTopic;
};

} // namespace config
