from flask import Flask, render_template

app = Flask(__name__)


people = {"Guillaume": ["IT Consultant", "Antwerp, Belgium", "guillaume@outlook.be", "0412345678"],
          "Kofi": ["Painter", "London, UK", "kofi@outlook.com", "0487654321"],
          "Loïc": ["Electrician", "Paris, France", "loic@gmail.fr", "0814725836"],
          "Kenzo": ["Carpenter", "Frankfurt, Germany", "kenzo@kdg.be", "0732165498"],
          "Nils": ["Welder", "Barcelona, Spain", "nils@outlook.sp", "0315935746"]}


@app.route("/")
def main(_name="Guillaume", _job="IT Consultant", _location="Antwerp, Belgium", _email="guillaume2000@hotmail.be", _phone="0471990383"):
    return render_template('index.html', name=_name, job=_job, location=_location, email=_email, phone=_phone)


@app.route("/<person>")
def person(person):
    if (person in people.keys()):
        return main(person, people[person][0], people[person][1], people[person][2], people[person][3])
    else:
        return main()


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
