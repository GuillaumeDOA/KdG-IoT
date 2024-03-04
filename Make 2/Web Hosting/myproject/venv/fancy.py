from flask import Flask, render_template

app = Flask(__name__)

@app.route("/")
def fancy(name="Guillaume"):
    return render_template('fancy.html',name=name)

@app.route("/<name>")
def fancy_name(name):
    return fancy(name)


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=4000)