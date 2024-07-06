// LOCAL IMPORTS START HERE
#include "../include/metric_producer.hpp"
// LOCAL IMPORTS END HERE

MetricProducer::MetricProducer() {
    const RdKafka::Conf *conf = getConfig_();
    
    _producer = getProducer_(conf);
    _topic = getTopic_();

    delete conf;
}

MetricProducer::~MetricProducer() {
    delete _topic;
    delete _producer;
}

RdKafka::Conf* MetricProducer::getConfig_() const {
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    std::string errstr;

    const std::string brokers = getEnvironmentVariable_("BOOTSTRAP_SERVERS");
    conf->set("metadata.broker.list", brokers, errstr);

    const std::string security_protocal = getEnvironmentVariable_("SECURITY_PROTOCOL");
    conf->set("security.protocol", security_protocal, errstr);

    const std::string sasl_mechanism = getEnvironmentVariable_("SASL_MECHANISM");
    conf->set("sasl.mechanisms", sasl_mechanism, errstr); 

    const std::string sasl_username = getEnvironmentVariable_("SASL_PLAIN_USERNAME");
    conf->set("sasl.username", sasl_username, errstr);

    const std::string sasl_password = getEnvironmentVariable_("SASL_PLAIN_PASSWORD");
    conf->set("sasl.password", sasl_password, errstr);

    return conf;
}


RdKafka::Producer* MetricProducer::getProducer_(const RdKafka::Conf* conf) const {
    std::string errstr;
    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);

    if (!producer) {
        throw std::runtime_error("Failed to create producer: " + errstr);
    }

    return producer;
}

RdKafka::Topic* MetricProducer::getTopic_() const {
    std::string errstr;
    const std::string topicName = getEnvironmentVariable_("TOPIC_NAME");
    RdKafka::Topic *topic = RdKafka::Topic::create(_producer, topicName, nullptr, errstr);

    if (!topic) {
        throw std::runtime_error("Failed to create topic: " + errstr);
    }

    return topic;
}


std::string MetricProducer::getEnvironmentVariable_(const char* name) const {
    const char* value = std::getenv(name);

    if (value == nullptr) {
        throw std::runtime_error("Environment variable not found");
    }

    return std::string(value);
}