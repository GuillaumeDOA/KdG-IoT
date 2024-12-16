# Voor deze opdracht heb ik gebruik gemaakt van MariaDB  voor het hosten van een MySQL Database
# die draait op de lokale server bij mij thuis (TrueNAS) en Pipedream voor het webhook gedeelte
# en de render_template feature van Flask voor het tonen van de html pagina's
# Heb gebruik gemaakt van chatGPT voor het genereren van de html pagina's
# - OpenAI (2024) - ChatGPT - https://chat.openai.com/


from flask import Flask, redirect, request, render_template
import os
from dotenv import load_dotenv
from requests.auth import HTTPBasicAuth
import requests
import mysql.connector

load_dotenv()

PORT = os.getenv("PORT")
SECRET_TOKEN = os.getenv("SECRET_TOKEN")
DATABASE_ID = os.getenv("DATABASE_ID")
NOTION_VERSION = os.getenv("NOTION_VERSION")
CLIENT_ID = os.getenv("CLIENT_ID")
REDIRECT_URI = os.getenv("REDIRECT_URI")
SQL_HOST = os.getenv("SQL_HOST")
SQL_USER = os.getenv("SQL_USER")
SQL_PASSWORD = os.getenv("SQL_PASSWORD")

app = Flask(__name__)


@app.route("/", methods=["GET"])
def hello():
    return render_template('index.html', client_id=CLIENT_ID, redirect_uri=REDIRECT_URI)


@app.route("/login", methods=["GET"])
def notion_login():
    return (
        '<a href="https://api.notion.com/v1/oauth/authorize?owner=user&client_id='
        + CLIENT_ID
        + "&redirect_uri="
        + REDIRECT_URI
        + '&response_type=code">Login with Notion</a>'
    )


@app.route("/oauth/redirect", methods=["GET"])
def login_redirect():
    code = request.args.get("code")
    if not code:
        return "Geen authorization code gevonden."

    profile_data = get_token(code)

    workspace_name = profile_data.json()["workspace_name"]
    workspace_id = profile_data.json()["workspace_id"]
    user = profile_data.json()["owner"]["user"]["name"]
    email = profile_data.json()["owner"]["user"]["person"]["email"]
    access_token = profile_data.json()["access_token"]

    requests.post(
        "https://eom33zzx2ly3qyw.m.pipedream.net",
        json={
            "workspace_name": workspace_name,
            "workspace_id": workspace_id,
            "user": user,
            "email": email,
            "acces_token": access_token,
        },
    )

    mydb = mysql.connector.connect(
        host=SQL_HOST,
        user=SQL_USER,
        password=SQL_PASSWORD,
        database="notion_integration",
    )

    cursor = mydb.cursor()

    query = """
        INSERT INTO Users (name, email)
        SELECT %s, %s
        WHERE NOT EXISTS (
            SELECT 1 
            FROM Users 
            WHERE name = %s
        );
        """
    cursor.execute(query, (user, email, user))
    mydb.commit()
    
    query = "SELECT id FROM Users WHERE name = %s;"
    cursor.execute(query, (user,))
    result = cursor.fetchone()

    if result:
        user_id = result[0]
        query = """
            INSERT INTO Workspaces (id, name, user_id)
            SELECT %s, %s, %s
            WHERE NOT EXISTS (
                SELECT 1 
                FROM Workspaces 
                WHERE id = %s
            );        
        """
        cursor.execute(query, (workspace_id, workspace_name, user_id, workspace_id))

        query = """
            INSERT INTO Bots (token, user_id)
            SELECT %s, %s
            WHERE NOT EXISTS (
                SELECT 1 
                FROM Bots 
                WHERE token = %s
            );
        """
        cursor.execute(query, (access_token, user_id, access_token))

        mydb.commit()

    else:
        print("User not found.")


    cursor.close()
    mydb.close()

    return render_template('logged_in.html', user=user)


def get_token(code):
    base_url = "https://api.notion.com/v1/oauth/token/"
    basic = HTTPBasicAuth(CLIENT_ID, SECRET_TOKEN)

    data = {
        "grant_type": "authorization_code",
        "code": code,
        "redirect_uri": REDIRECT_URI,
    }

    auth_resp = requests.post(base_url, auth=basic, data=data)

    return auth_resp


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=PORT, debug=True)
