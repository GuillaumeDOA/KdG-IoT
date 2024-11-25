#include <Arduino.h>
String PASSWORD = "KdGIoT0!";
String SSID = "IoT";
String URL = "https://v2.jokeapi.dev/joke/Any?blacklistFlags=nsfw,racist";

const char index_html[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Joke Web Server</title>
  <style>
    body { font-family: Arial, sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; background-color: #f0f0f0; }
    .container { text-align: center; padding: 20px; background: white; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2); border-radius: 8px; }
    .joke { font-size: 1.5em; margin-bottom: 20px; }
    .buttons { display: flex; gap: 10px; justify-content: center; }
    button { padding: 10px 20px; font-size: 1em; cursor: pointer; border: none; border-radius: 5px; }
    .like { background-color: #4CAF50; color: white; }
    .dislike { background-color: #f44336; color: white; }
    .next { background-color: #008CBA; color: white; }
  </style>
</head>
<body>
  <div class="container">
    <div id="joke" class="joke"></div>
    <div class="buttons">
      <button class="next" onclick="loadNextJoke()">Next Joke</button>
    </div>
  </div>

<script>
      function loadNextJoke() {
      fetch('/nextjoke')
        .then(response => response.text())
        .then(joke => {
          document.getElementById('joke').innerText = joke;
        });
    }
</script>
</body>
</html>
)rawliteral";