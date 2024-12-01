from flask import Flask, redirect, request
import os
from dotenv import load_dotenv
from requests.auth import HTTPBasicAuth
import requests
load_dotenv()

PORT = os.getenv("PORT")
SECRET_TOKEN = os.getenv("SECRET_TOKEN")
DATABASE_ID = os.getenv("DATABASE_ID")
NOTION_VERSION = os.getenv("NOTION_VERSION")
CLIENT_ID = os.getenv("CLIENT_ID")
REDIRECT_URI = os.getenv("REDIRECT_URI")

app = Flask(__name__)

@app.route("/", methods=["GET"])
def hello():
    return (
        "Hello world<br>"
        + '<a href="/login">Go To Log in Page</a>'

    )

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

    return "Hey " + profile_data.json()["owner"]["user"]["name"]

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