from flask import Flask, render_template

app = Flask(__name__)

@app.route("/")
@app.route("/index.html")
def index():
    return render_template('index.html')

@app.route("/shop.html")
def shop():
    return render_template('shop.html')

@app.route("/product-details.html")
def productdetails():
    return render_template('product-details.html')

@app.route("/contact.html")
def contact():
    return render_template('contact.html')

@app.route("/signin.html")
def signin():
    return render_template('signin.html')

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=4000)