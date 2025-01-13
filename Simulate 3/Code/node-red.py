import sys
import paho.mqtt.client as mqtt  # For MQTT communication
import json                      # For creating JSON messages
import ssl                       # For secure connection
from datetime import datetime    # For timestamps
import os                        # For environment variables
import time                      # For sleep function
from dotenv import load_dotenv   # For loading environment variables

load_dotenv()

# MQTT Connection settings
broker = "mqtt.blockbax.com"    # The server we want to connect to
port = 8883                     # The port for secure MQTT
username = os.getenv("ID")      # Our username from .env file
password = os.getenv("API")     # Our password from .env file
project_id = os.getenv("PROJECT")  # Our project ID from .env file

def on_connect(client, userdata, flags, rc, properties=None):
    """Function that runs when we connect to the broker"""
    if rc == 0:
        print("Successfully connected to Blockbax!")
    else:
        print(f"Connection failed with error code: {rc}")

def send_measurement(client, measurement, measurementType, ingestionID):
    """Function to send a measurement to Blockbax"""
    # Create timestamp in UTC format
    timestamp = datetime.utcnow().strftime("%Y-%m-%dT%H:%M:%S.%f")[:-3] + "Z"

    # Create the message in Blockbax format
    message = {
        "series": [{
            "ingestionId": ingestionID,
            "measurements": [{
                "date": timestamp,
                measurementType: measurement
            }]
        }]
    }

    # Send to Blockbax
    topic = f"v1/projects/{project_id}/measurements"
    client.publish(topic, json.dumps(message), qos=1)
    print(f"Sent value: {measurement} to id: {ingestionID}")

def main():
    """Main program"""
    # Setup MQTT client
    client = mqtt.Client(client_id=f"{username}_client", protocol=mqtt.MQTTv5)
    client.username_pw_set(username=username, password=password)
    client.tls_set(cert_reqs=ssl.CERT_REQUIRED, tls_version=ssl.PROTOCOL_TLSv1_2)
    client.on_connect = on_connect

    # Connect to Blockbax
    print("Connecting to Blockbax...")
    client.connect(broker, port)
    client.loop_start()

    # Wait a moment for the connection
    time.sleep(2)

    topic = sys.argv[1]
    value = sys.argv[2]
    
    valueType = ""
    ingestionID = ""
    
    match topic:
        case "lock":
            ingestionID = "kdg-test-type-groep-1$nfc"
            valueType = "text"
        case "co2":
            ingestionID = "kdg-test-type-groep-1$co-2-level"
            valueType = "number"
            value = int(value)
        case "humidity":
            ingestionID = "kdg-test-type-groep-1$humidity"
            valueType = "number"
            value = int(value)
        case "temperature":
            ingestionID = "kdg-test-type-groep-1$temperature"
            valueType = "number"
            value = int(value)
        case "motion":
            ingestionID = "kdg-test-type-groep-1$movement"
            valueType = "text"
        case "current":
            ingestionID = "kdg-test-type-groep-1$ac-current"
            valueType = "number"
            value = int(value)
            
    print(f"Topic: {topic}, measurement: {value}, ingestionID: {ingestionID}, valueType: {valueType}({type(value)})")
            
    send_measurement(client, value, valueType, ingestionID)
            
    client.loop_stop()
    client.disconnect()
    print("Disconnected from Blockbax")
    
# Run the program
if __name__ == "__main__":
    main()