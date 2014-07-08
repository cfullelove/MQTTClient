#include <mosquittopp.h>
#include <iostream>
#include <vector>
#include <queue>
#include <Poco/BasicEvent.h>
#include <Poco/Mutex.h>

#ifndef NTPS_mqtt_client_included
#define NTPS_mqtt_client_included

class MQTTConnectEventArgs
{

public:

	int rv;

	MQTTConnectEventArgs( int r) :
		rv( r )
	{

	};

	~MQTTConnectEventArgs()
	{

	}
};

class MQTTMessageEventArgs
{
public:
	const mosquitto_message *message;

	MQTTMessageEventArgs( const mosquitto_message *_message ) : message( _message )
	{

	}

	~MQTTMessageEventArgs()
	{

	}
};

class MQTTPublishEventArgs
{
public:
	uint16_t mid;

	MQTTPublishEventArgs( uint16_t m ) : mid( m )
	{

	}

	~MQTTPublishEventArgs()
	{

	}
};

class MQTTSubscribeEventArgs
{
public:
	uint16_t mid;
	int qos_count;
	const uint8_t *granted_qos;

	MQTTSubscribeEventArgs( uint16_t _mid, int _qos_count, const uint8_t *_granted_qos ) :
		mid( _mid ),
		qos_count( _qos_count ),
		granted_qos( _granted_qos )
	{

	}

	~MQTTSubscribeEventArgs()
	{

	}

};

class MQTTDisconnectEventArgs
{
public:
	MQTTDisconnectEventArgs()
	{

	}

	~MQTTDisconnectEventArgs()
	{

	}
};

class MQTTUnsubscribeEventArgs
{
public:
	MQTTUnsubscribeEventArgs()
	{

	}


	~MQTTUnsubscribeEventArgs()
	{

	}
};

class MQTTErrorEventArgs
{
public:
	MQTTErrorEventArgs()
	{

	}

	~MQTTErrorEventArgs()
	{

	}
};

class MQTTClient : public mosquittopp::mosquittopp
{

public:
	
	Poco::BasicEvent<MQTTConnectEventArgs> clientConnect;
	Poco::BasicEvent<MQTTMessageEventArgs> clientMessage;
	Poco::BasicEvent<MQTTPublishEventArgs> clientPublish;
	Poco::BasicEvent<MQTTSubscribeEventArgs> clientSubscribe;
	Poco::BasicEvent<MQTTDisconnectEventArgs> clientDisconnect;
	Poco::BasicEvent<MQTTUnsubscribeEventArgs> clientUnsubscribe;
	Poco::BasicEvent<MQTTErrorEventArgs> clientError;


	MQTTClient( const char* id );

	~MQTTClient();

	void set_host(const std::string& name, const std::string& value);
	void set_host(const std::string value);

	bool connected();

	int publish( std::string topic, std::string message );
	int loop( int timeout );

	int connect();

	void on_connect( int rc );

	void on_disconnect();

	void on_message( const struct mosquitto_message *message );

	void on_publish(uint16_t mid);

	void on_subscribe( uint16_t mid, int qos_count, const uint8_t *granted_qos);

	void on_unsubscribe(uint16_t mid);

	void on_error();


private:
    std::queue<std::vector<std::string> > publishQueue;
    std::string host;

    Poco::Mutex mutex;

    bool _connected;

};

#endif
