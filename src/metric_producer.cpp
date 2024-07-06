// LOCAL IMPORTS START HERE
#include "metric_producer.hpp"
// LOCAL IMPORTS END HERE

MetricProducer::MetricProducer()
{
	const RdKafka::Conf* conf = getConfig_();

	_producer = getProducer_(conf);
	_topic = getTopic_();

	delete conf;
}

MetricProducer::~MetricProducer()
{
    RdKafka::ErrorCode error_code = RdKafka::ERR__TIMED_OUT;

    // TODO: Consider checing for other errors while flushing
    // Clear out all messages in the producer
    while (error_code == RdKafka::ERR__TIMED_OUT){
        error_code = _producer->flush(1000);
    }

	delete _topic;
	delete _producer;
}

bool MetricProducer::produce(const std::string& application_name,
							 const std::string& metric_name) const
{
	nlohmann::json data;
	data["application"] = application_name;
	data["metric_name"] = metric_name;

	// TODO: Hnadle datetime
	std::string current_datetime_str = "2023-04-01 12:00:00";
	data["timestamp"] = current_datetime_str;

	// Convert the JSON object to a string
	std::string message = data.dump();

	// Produce a message
	RdKafka::ErrorCode resp = _producer->produce(_topic,
												 RdKafka::Topic::PARTITION_UA,
												 RdKafka::Producer::RK_MSG_COPY,
												 const_cast<char*>(message.c_str()),
												 message.size(),
												 nullptr,
												 nullptr);
    
    _producer->poll(0);
	return resp == RdKafka::ERR_NO_ERROR;
}

RdKafka::Conf* MetricProducer::getConfig_() const
{
	RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
	std::string errstr;

	const std::string brokers = getEnvironmentVariable_("BOOTSTRAP_SERVERS");
	conf->set("metadata.broker.list", brokers, errstr);

	const std::string security_protocal = getEnvironmentVariable_("SECURITY_PROTOCOL");
	conf->set("security.protocol", security_protocal, errstr);

	const std::string sasl_mechanism = getEnvironmentVariable_("SASL_MECHANISM");
	conf->set("sasl.mechanisms", sasl_mechanism, errstr);

	const std::string sasl_username = getEnvironmentVariable_("SASL_PLAIN_USERNAME");
	conf->set("sasl.username", sasl_username, errstr);

    // TODO: Fix typo in env
	const std::string sasl_password = getEnvironmentVariable_("SASL_PLAN_PASSWORD");
	conf->set("sasl.password", sasl_password, errstr);

	return conf;
}

RdKafka::Producer* MetricProducer::getProducer_(const RdKafka::Conf* conf) const
{
	std::string errstr;
	RdKafka::Producer* producer = RdKafka::Producer::create(conf, errstr);

	if(!producer)
	{
		throw std::runtime_error("Failed to create producer: " + errstr);
	}

	return producer;
}

RdKafka::Topic* MetricProducer::getTopic_() const
{
    // TODO: Change environment variable name
	const std::string topicName = getEnvironmentVariable_("METRIC_TOPIC");
	std::string errstr;
	RdKafka::Topic* topic = RdKafka::Topic::create(_producer, topicName, nullptr, errstr);

	if(!topic)
	{
		throw std::runtime_error("Failed to create topic: " + errstr);
	}

	return topic;
}

std::string MetricProducer::getEnvironmentVariable_(const char* name) const
{
	const char* value = std::getenv(name);

	if(value == nullptr)
	{
		throw std::runtime_error("Environment variable not found: " + std::string(name));
	}

	return std::string(value);
}