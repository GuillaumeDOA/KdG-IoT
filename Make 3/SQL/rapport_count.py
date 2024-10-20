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

# SQL-query
sql = """
SELECT customers.first_name, customers.last_name, COUNT(DISTINCT devices.id) AS 'Aantal Devices', COUNT(sensors.id) AS 'Aantal Sensoren'
FROM customers
JOIN devices ON customers.id = devices.customer_id
JOIN sensors ON devices.id = sensors.device_id
GROUP BY customers.id
ORDER BY customers.id;
"""

# Voer de query uit
mycursor.execute(sql)

# Haal alle resultaten op
myresult = mycursor.fetchall()

# Druk de resultaten af
for row in myresult:
	print("Customer: {0} {1}\n\tAmount of Devices: {2}\n\tAmount of Sensors: {3}\n".format(row[0],row[1],row[2],row[3]))

# Sluit de databaseverbinding
mycursor.close()
mydb.close()
