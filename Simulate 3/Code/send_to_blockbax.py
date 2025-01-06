# Import required libraries
import paho.mqtt.client as mqtt  # For MQTT communication
import json                      # For creating JSON messages
import ssl                       # For secure connection
from datetime import datetime    # For timestamps
import os                        # For environment variables
import time                      # For sleep function
from dotenv import load_dotenv   # For loading environment variables

# Load our secret keys from .env file
load_dotenv()

# MQTT connection settings
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

def send_co2_measurement(client, co2_value):
    """Function to send a CO2 measurement to Blockbax"""
    # Create timestamp in UTC format
    timestamp = datetime.utcnow().strftime("%Y-%m-%dT%H:%M:%S.%f")[:-3] + "Z"

    # Create the message in Blockbax format
    message = {
        "series": [{
            "ingestionId": "test-sensor$co-2",
            "measurements": [{
                "date": timestamp,
                "number": float(co2_value)
            }]
        }]
    }

    # Send to Blockbax
    topic = f"v1/projects/{project_id}/measurements"
    client.publish(topic, json.dumps(message), qos=1)
    print(f"Sent CO2 value: {co2_value} ppm")

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

    try:
        print("\nReady to send CO2 measurements!")
        print("Type a number to send a CO2 value")
        print("Type 'q' to quit the program")

        while True:
            user_input = input("\nEnter CO2 value in ppm: ")

            # Check for quit command
            if user_input.lower() == 'q':
                break

            # Try to send the measurement
            try:
                co2_value = float(user_input)
                send_co2_measurement(client, co2_value)
            except ValueError:
                print("Please enter a valid number!")

    except KeyboardInterrupt:
        print("\nProgram stopped by user")

    finally:
        client.loop_stop()
        client.disconnect()
        print("Disconnected from Blockbax")

# Run the program
if __name__ == "__main__":
    main()