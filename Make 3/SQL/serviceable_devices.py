# Heb ChatGPT gberuikt voor het genereren van de tabel data
# OpenAI (2024) - ChatGPT - https://chat.openai.com/

import mysql.connector
from datetime import date, timedelta

# Verbinding maken met de database
mydb = mysql.connector.connect(
    host="172.19.0.2",
    user="root",
    password="myPassword",  # Vul hier het juiste wachtwoord in
    database="iot_management"
)

mycursor = mydb.cursor()

#Get date from 6 months ago
then = date.today() - timedelta(weeks=26)


# SQL-query
sql = """
SELECT devices.id, device_type, serial_number, installation_date, log_date
FROM devices
JOIN maintenance_logs ON devices.id = maintenance_logs.device_id
WHERE log_date < '{0}'
""".format(then)

# Voer de query uit
mycursor.execute(sql)

# Haal alle resultaten op
myresult = mycursor.fetchall()

# Druk de resultaten af
for res in myresult:
        print("Device id: {0} \nType: {1} \nSerial Number: {2} \nInstallation Date: {3}, Last Log Date: {4}\n".format(res[0], res[1], res[2], res[3], res[4]))

# Sluit de databaseverbinding
mycursor.close()
mydb.close()
