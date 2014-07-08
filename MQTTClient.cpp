#include <mosquittopp.h>
#include "MQTTClient.h"
#include <Poco/Mutex.h>
#include <vector>

MQTTClient::MQTTClient( const char* id ) : mosquittopp( id )
{
	host = std::string( "" );
};

MQTTClient::~MQTTClient()
{
};

void MQTTClient::set_host(const std::string& name, const std::string& value)
{
	host = std::string( value );
}

void MQTTClient::set_host( const std::string value )
{
	host = std::string( value );
}

bool MQTTClient::connected()
{
	return _connected;
}

int MQTTClient::publish( std::string topic, std::string message )
{
	std::vector<std::string> pubData(2);
	pubData[0] = topic;
	pubData[1] = message;
	
	Poco::Mutex::ScopedLock lock( mutex );
	publishQueue.push( pubData );
	return 0;
}

int MQTTClient::connect()
{
	Poco::Mutex::ScopedLock lock( mutex );
	if ( host == "" )
		return false;

	return mosquittopp::connect( host.c_str() );
}

int MQTTClient::loop( int timeout = 1000 )
{
	std::vector<std::string> pubData;
	Poco::Mutex::ScopedLock lock( mutex );
	while ( ! publishQueue.empty() )
	{
		pubData = publishQueue.front();
		publishQueue.pop();
		mosquittopp::publish( NULL, (pubData[0]).c_str(), (pubData[1]).size(), (uint8_t*) (pubData[1]).c_str(),  0, false );
	}
	return mosquittopp::loop( timeout );
}

void MQTTClient::on_connect( int rc )
{
	_connected = true;
	
    MQTTConnectEventArgs eventArgs( rc );
    clientConnect.notify( this, eventArgs );
};

void MQTTClient::on_disconnect()
{
	_connected = false;

	MQTTDisconnectEventArgs eventArgs;
	clientDisconnect.notify( this, eventArgs );

};

void MQTTClient::on_message( const struct mosquitto_message *message )
{

	MQTTMessageEventArgs eventArgs( message );

    clientMessage.notify( this, eventArgs );
};

void MQTTClient::on_publish(uint16_t mid)
{
	MQTTPublishEventArgs eventArgs( mid );

	clientPublish.notify( this, eventArgs );
};

void MQTTClient::on_subscribe( uint16_t mid, int qos_count, const uint8_t *granted_qos)
{
	MQTTSubscribeEventArgs eventArgs( mid, qos_count, granted_qos );

	clientSubscribe.notify( this, eventArgs );
};

void MQTTClient::on_unsubscribe(uint16_t mid)
{
	MQTTUnsubscribeEventArgs eventArgs;

	clientUnsubscribe.notify( this, eventArgs );
};

void MQTTClient::on_error()
{
	MQTTErrorEventArgs eventArgs;
	clientError.notify( this, eventArgs );
};
