// STL IMPORTS START HERE
#include <cstdlib>
#include <iostream>
// STL IMPORTS END HERE

// THIRD PARTY IMPORTS START HERE
#include <librdkafka/rdkafkacpp.h>

#include "json.hpp"
// THIRD PARTY IMPORTS END HERE

class MetricProducer
{
public:
	MetricProducer();
	~MetricProducer();

	bool produce(const std::string& application_name, const std::string& metric_name) const;

private:
	RdKafka::Producer* _producer;
	RdKafka::Topic* _topic;

	RdKafka::Conf* getConfig_() const;
	RdKafka::Producer* getProducer_(RdKafka::Conf* conf) const;
	RdKafka::Topic* getTopic_() const;

	std::string getEnvironmentVariable_(const char* name) const;
};