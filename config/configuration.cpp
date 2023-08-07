#include "config/configuration.h"

namespace {

constexpr auto kTestDataDirectory = "/home/developer/Prj/repo/test_data";
constexpr auto kMqttServerHostName = "localhost";
constexpr uint16_t kMqttServerPort = 1883;
constexpr auto kMqttClientRequestPublishingTopic = "job";
constexpr auto kMqttJobHandlerSubscriptionTopic = "$share/dummy_group_id/job";

} // namespace

namespace config {

Configuration::Configuration(std::string testDataDirectory,
                             std::string mqttServerHostName,
                             uint16_t mqttServerPort,
                             std::string mqttClientRequestTopic,
                             std::string mqttJobHandlerSubscriptionTopic)
    : mTestDataDirectory{testDataDirectory}
    , mMqttServerHostName{std::move(mqttServerHostName)}
    , mMqttServerPort{mqttServerPort}
    , mMqttClientRequestTopic{std::move(mqttClientRequestTopic)}
    , mMqttJobHandlerSubscriptionTopic{std::move(mqttJobHandlerSubscriptionTopic)}
{}

std::shared_ptr<Configuration> Configuration::instance()
{
    static std::shared_ptr<Configuration> instance(
        new Configuration(kTestDataDirectory,
                          kMqttServerHostName,
                          kMqttServerPort,
                          kMqttClientRequestPublishingTopic,
                          kMqttJobHandlerSubscriptionTopic));
    return instance;
}

std::string Configuration::testDataDirectory() const
{
    return mTestDataDirectory;
}

std::string Configuration::mqttServerHostName() const
{
    return mMqttServerHostName;
}

uint16_t Configuration::mqttServerPort() const
{
    return mMqttServerPort;
}

std::string Configuration::mqttClientRequestTopic() const
{
    return mMqttClientRequestTopic;
}

std::string Configuration::mqttJobHandlerSubscriptionTopic() const
{
    return mMqttJobHandlerSubscriptionTopic;
}

} // namespace config
