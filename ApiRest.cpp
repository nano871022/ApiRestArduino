#include "Arduino.h"
#include "ApiRest.h"
#include <WiFiClientSecure.h>

String ApiRest::get(String url){
  	String ihost = host(url);
  	int    iport = port(&httpsClient,url);
  	String ipath = path(url);
  	String iparams = params(url);
  	httpsClient.setTimeout(TIME_OUT_CONNECT);
  	if(ipath == ihost){
  		ipath = "/";
  	}
  	if(connect(&httpsClient,ihost,iport)==0){
  		String sendGet = makeHeaders("GET ",ihost,ipath,iparams);
  		print(sendGet+"\r\n");
  		httpsClient.print(sendGet);
  		responseHeader(&httpsClient);
  		print(String("Status: ")+statusConnected+"\r\n");
  		print(String("Description: ")+description+"\r\n");
		return responsePayload(&httpsClient);
	}
	return String("Device can not connect with ")+ihost;
}

void   ApiRest::statusConnection(String line){
	if(line.indexOf("HTTP") >= 0){
		int spaceDetected = line.indexOf(" ");
		String statusConnect = line.substring(spaceDetected+1);
		spaceDetected = statusConnect.indexOf(" ");
		description = statusConnect.substring(spaceDetected+1);
		statusConnect = statusConnect.substring(0,spaceDetected);
		statusConnected = statusConnect.toInt();
	}
}	

String ApiRest::host(String url){
	String host;
	if(isHttps(url)){
		host = url.substring(8);
		int pathInit = host.indexOf("/");
		if(pathInit >= 0){
			host = host.substring(0,pathInit);
		}
		int portPos = host.indexOf(":");
		if(portPos>=0){
			host = host.substring(0,portPos);
		} 
	}
	return host;
}

int    ApiRest::port(WiFiClientSecure *client,String url){
	int    port = 80;
	String host;
	if(isHttps(url)){
		port = 443;
		host = url.substring(8);
	}
	int pathInit = host.indexOf("/");
	if(pathInit >= 0){
		host = host.substring(0,pathInit);
	}
	int portPos = host.indexOf(":");
	if(portPos>=0){
		host = host.substring(portPos+1);
		port = host.toInt();
	} 
	if(port == 443){
		client->setFingerprint(fingerprint);
	}
	return port;
}

String ApiRest::path(String url){
    String path;
	if(isHttps(url)){
		path = url.substring(8);
	}
	int pathInit = path.indexOf("/");
	if(pathInit >= 0){
		path = path.substring(pathInit);
	}
	pathInit = path.indexOf("?");
	if(pathInit >= 0){
		path = path.substring(0,pathInit);
	}
	return path;	
}

String ApiRest::params(String url){
	int paramInit = url.indexOf("?");
	if(paramInit >= 0){
		return url.substring(paramInit+1);
	}
	return "";
}

String ApiRest::makeHeaders(String type,String host,String path,String params){
	if(params.length()>0){
		params = "?"+params;
	}
	return String(type)+path+params+" HTTP/1.1\r\n"+
			"Host: "+host+"\r\n"+
			"User-Agent: "+agent+"\r\n"+
			"Accept: "+acceptFormat+"\r\n"+
			"Accept-Language: "+language+"\r\n"+
			"Cache-Control: max-age=0 \r\n"+
			"Connection: close \r\n"+
			"\r\n";
}

void ApiRest::responseHeader(WiFiClientSecure *client){
	int empty = 0;
	while(client->connected()){
		String line = client->readStringUntil('\n');
		if(line == "\r" || (line == "\n" && line.length() == 1)){
			break;
		}else if(line.length() < 2){
			if(empty == RETRY_EMPTY){
				break;
			}
			empty++;
		}else{
			statusConnection(line);
		}
	}
}

String ApiRest::responsePayload(WiFiClientSecure *client){
	String line;
	String buffer = "";
	int i = 0;
	while(client->available()){
		line = client->readStringUntil('\n');
		buffer += line+"\r\n";
		i++;
	}
	print(String("It read ")+i+" lines \r\n");
	return buffer;
}

int ApiRest::connect(WiFiClientSecure *client,String host,int port){
	int retryConnect = 0;
	print("Connecting");
	while((!client->connect(host,port)) && (retryConnect < 30)){
		print(".");
		delay(400);
		retryConnect++;
	}
	if(retryConnect == 30){
		return -1;
	}
	print("\r\n");
	return 0;
}

void ApiRest::setFingerPrint(const char *fingerPrint){
	fingerprint = fingerPrint;
}

void ApiRest::print(String value){
	if(debugMode){
		Serial.print(value);
	}
}

bool ApiRest::isHttps(String url){
	return url.indexOf("https://")>=0 || url.indexOf("HTTPS://")>=0;
}

void ApiRest::activeDebugMode(){
	debugMode = true;
}

int ApiRest::getStatusConnection(){
	return statusConnected;
}
String ApiRest::getDescriptionConnection(){
	return description;
}	