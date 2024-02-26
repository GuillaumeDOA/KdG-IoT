from flask import Flask, render_template

app = Flask(__name__)

@app.route("/")
def hello_world(username="Guillaume"):
    return f'<h1>Hallo dit is de pagina van {username} </h1> \
            <div> Dit is mijn foto: <img src="static/image/dog.jpg"></div>\
            <div> Dit is mijn google link: <a href="https://www.google.com/search?q=Guillaume+de+Oliveira+Andrezo&sca_esv=e9be6bafba5c0576&rlz=1C1GCEA_enBE1083BE1083&sxsrf=ACQVn0-R6EspBZX6g-5ZfrB33wzA3aww-A%3A1708947018558&ei=SnbcZe7ZIdeekdUPjLiBMA&ved=0ahUKEwjupuzf88iEAxVXT6QEHQxcAAYQ4dUDCBA&uact=5&oq=Guillaume+de+Oliveira+Andrezo&gs_lp=Egxnd3Mtd2l6LXNlcnAiHUd1aWxsYXVtZSBkZSBPbGl2ZWlyYSBBbmRyZXpvMgUQIRigATIFECEYoAFIxEdQgwVY1UFwBngBkAEAmAGLAaABvxSqAQQzMC4yuAEDyAEA-AEBmAImoAKcF6gCE8ICChAAGEcY1gQYsAPCAhMQLhiABBiKBRhDGMcBGNEDGLADwgINEAAYgAQYigUYQxiwA8ICChAjGIAEGIoFGCfCAgQQIxgnwgISEC4YgAQYigUYQxjHARjRAxgKwgIKEAAYgAQYigUYQ8ICEBAAGIAEGIoFGEMYsQMYgwHCAiEQLhiABBiKBRhDGMcBGNEDGAoYlwUY3AQY3gQY4ATYAQHCAgsQABiABBiKBRiRAsICCxAAGIAEGLEDGIMBwgIIEC4YgAQYsQPCAg4QLhiABBiKBRixAxiDAcICERAuGIAEGLEDGIMBGMcBGNEDwgILEC4YgAQYsQMYgwHCAgcQIxjqAhgnwgIHEC4Y6gIYJ8ICGRAuGIAEGIoFGEMYxwEY0QMY6gIYtALYAQLCAhMQABiABBiKBRhDGOoCGLQC2AECwgIUEAAYgAQY4wQY6QQY6gIYtALYAQLCAhEQABiABBiKBRiRAhixAxiDAcICCBAuGLEDGIAEwgIEEC4YA8ICBRAuGIAEwgIOEAAYgAQYigUYsQMYgwHCAggQLhiABBjUAsICBRAAGIAEwgITEC4YAxiXBRjcBBjeBBjfBNgBAcICCBAAGIAEGMsBwgIIEC4YgAQYywHCAg4QLhiABBjLARjHARivAcICFxAuGIAEGMsBGJcFGNwEGN4EGOAE2AEBwgIREC4YgAQYywEYxwEYrwEYjgXCAgYQABgWGB7CAggQLhgWGB4YCsICBxAhGAoYoAGYAxiIBgGQBgq6BgYIARABGBS6BgYIAhABGAGSBwQzMC44&sclient=gws-wiz-serp" target="_blank">Search!</a></div>'
            
            
@app.route("/<username>")
def hello_world_username(username):
    return hello_world(username)