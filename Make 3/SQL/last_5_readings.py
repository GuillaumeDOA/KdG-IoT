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

#Vraag input gebruiker
sensor_id = input("Voor welke sensor wil je de 5 laatste reading? ")

# SQL-query
sql = """
SELECT reading_value, reading_timestamp
FROM sensor_readings
WHERE sensor_id = %s
ORDER BY reading_timestamp DESC
LIMIT 5;
"""

# Voer de query uit
mycursor.execute(sql, (sensor_id,))

# Haal alle resultaten op
myresult = mycursor.fetchall()

# Druk de resultaten af
for reading in myresult:
	print("Reading: {0} \nTimestamp: {1}\n".format(reading[0], reading[1]))

# Sluit de databaseverbinding
mycursor.close()
mydb.close()
