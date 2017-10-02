#include "Particle.h"

// DWH - Can I send to any device name? DEVICE_NAME is defined in Ubidots.h. Consider changing.
#define DEVICE_NAME	(char *)"home"
#define TOKEN (char *)"A1E-56GQdgNkUTirNkH3unFtVAxhCTfs5E"  // Ubidots Default TOKEN

#include "UbidotsMQTT/UbidotsMQTT.h"

void LED_toggle();
#define BLUE_LED				D7

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    Serial.println("payload obtained from server:");
    for (int i=0;i<length;i++) {
        Serial.print((char)payload[i]); // prints the answer of the broker for debug purpose
    }
    // Some stuff to make with the payload obtained
        //
   //
    Serial.println();
}

Ubidots client(TOKEN, callback);

//--------------------------------------------------------------------------------------------------
void setup() {
	pinMode(BLUE_LED, OUTPUT);

    Serial.begin(115200);
    //ubidots.setDebug(true); //Uncomment this line for printing debug messages
	client.initialize();

	if (client.isConnected() ){
        // Insert as first parameter the device to subscribe and as second the variable label
        client.ubidotsSubscribe(DEVICE_NAME, "tag");
    }
}

//--------------------------------------------------------------------------------------------------
void loop() {
	if(!client.isConnected()){
        client.reconnect();
		client.ubidotsSubscribe(DEVICE_NAME, "tag");
    }

    // Publish routine, if the device and variables are not created they will be created
    float value = 1;
	Serial.println("Sending values");
    client.add("test-var-1", value); // Insert as first parameter your variable label
    client.add("test-var-2", value, "\"lat\":10.302, \"lng\":2.9384"); //Adds value with context
    /*client.add("tag", value);*/
    client.ubidotsPublish(DEVICE_NAME); // Insert your device label where the values will be stored in Ubidots

    // Client loop for publishing and to maintain the connection
    client.loop();
	LED_toggle();
	delay(1000);
}

//--------------------------------------------------------------------------------------------------
void LED_toggle() {
	static bool status = false;
	digitalWrite(BLUE_LED, status);
	status = !status;
}
