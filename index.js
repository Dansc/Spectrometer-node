
var serialport = require('serialport');
var parsers = serialport.parsers;

var WebSocketServer = require('ws').Server;
var fs = require("fs");

// Require mongodb drivers
var mongodb = require('mongodb');

// Make a mongoclient interface
var MongoClient = mongodb.MongoClient;

// Connection URL. This is where your mongodb server is running.
var url = 'mongodb://localhost:27017/arduino';



var SERVER_PORT = 8081; 	// port number for WebSocketServer
var wss = new WebSocketServer({port: SERVER_PORT}); // the WebSocketServer
var connections = new Array; 	// list of connections to the WebSocketServer


var SerialPort = serialport.SerialPort;
portName = process.argv[2];

console.log(portName);

var parser = new parsers.Readline({delimiter: '\r\n'}
var myPort = new SerialPort(portName, {
	baudRate: 115200,
	// look for return and newline at the end of each data packet
	parser: serialport.parsers.Readline('\n')
});

myPort.on('open', showPortOpen);
myPort.on('data', receiveSerialData);
myPort.on('close', showPortClose);
myPort.on('error', showError);
myPort.write('Hello');

// WebSocketServer event listeners:
wss.on('connection', handleConnection);



function handleConnection(client){
	console.log("New Connection"); // you have a new client
	connections.push(client); // Add this client to the connections
	client.on('message', sendToSerial); // client message to serial connection
	client.on('close', function(){
		console.log("connection closed");
		var position = connections.indexOf(client); // get client position in connections Array
		connections.splice(position, 1); // delete client from connections array. Splice method changes original array while slice would return the modified array
	});
}

function showPortOpen() {
   console.log('port open. Data rate: ' + myPort.options.baudRate);
}

function receiveSerialData(data) {
   console.log(data);
   saveLatestData(data);
}


function sendToSerial(data){

	// if a message is meant for arduino, see below
	// console.log("sending to serial: " + data);
	// myPort.write(data);

}

// This function broadcasts messages to all clients
function broadcast(data) {
	for (myConnection in connections){ // iterate through all connections
		connections[myConnection].send(data);  // send the data
		}
}


function saveLatestData(data){
	if (connections.length > 0) {
		broadcast(data);
	}
}

function showPortClose() {

   console.log('port closed.');

}

function showError(error) {
   //list_ports;
   console.log('Serial port error: ' + error);

}


function list_ports(){
// list serial ports:
 serialport.list(function (err, ports){
 	ports.forEach(function(port) {
 		console.log(port.comName);
 	});
 });
}