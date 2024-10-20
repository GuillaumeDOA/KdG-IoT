# Heb ChatGPT gberuikt voor het genereren van de tabel data
# OpenAI (2024) - ChatGPT - https://chat.openai.com/

import mysql.connector

# Verbinding maken met de database
mydb = mysql.connector.connect(
    host="172.19.0.2",
    user="root",
    password="myPassword",  # Vul hier het juiste wachtwoord in
    database="iot_management"
)

mycursor = mydb.cursor()

#Vraag input aan gebruiker
sensor_id = input("Wat is de sensor_id voor de gemete waarde: ")
reading = input("Wat is de gemeten waarde: ")

#Build command
sql = """
INSERT INTO sensor_readings (sensor_id, reading_value)
VALUES (%s, %s);
"""

#Execute command
mycursor.execute(sql, (sensor_id, reading))

mydb.commit()

print("records inserted")

# Sluit de databaseverbinding
mycursor.close()
mydb.close()
