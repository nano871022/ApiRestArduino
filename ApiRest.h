/**
 ApiRest.h - Librery help to create communication with services external,
  it create to comunicate con docs.google.com, but it's scale to use with
  others servers, it's enfocated to communicate for https protocole, it use 
  port 443, so, it use fingerprint over SHA1. FingerPrint's a certificate 
  over format SHA1, it's getting by browser or command over terminal- 
  Create : 4 Jun 2020 
  by Alejandro Parra Colombia
**/
#ifndef ApiRest_h
#define ApiRest_h

#include "Arduino.h"
#include <WiFiClientSecure.h>



class ApiRest{
	public:
		String get(String url);
		void setFingerPrint(const char *fingerPrint);
		void activeDebugMode();
		int getStatusConnection();
		String getDescriptionConnection();
	private:
		WiFiClientSecure httpsClient;
		void   statusConnection(String line);
		bool debugMode = false;
		int    status;
		String description;
		int    statusConnected;
		int    TIME_OUT_CONNECT = 1500;
		const  int RETRY_EMPTY = 10;
		String host(String url);
		int    port(WiFiClientSecure *client,String url);
		String agent = "Arduino/nodeMCU - ESP8266/IOT";
		String acceptFormat = "text/html,application/xhtml+xml";
		String language = "es-CO";
		String path(String url);
		String makeHeaders(String type,String host,String path,String params);
		String params(String url);
		int    connect(WiFiClientSecure *client,String host,int port);
		String responsePayload(WiFiClientSecure *client);
		void   responseHeader(WiFiClientSecure *client);
		const char *fingerprint;
		void print(String value);
		bool isHttps(String url);
};

#endif