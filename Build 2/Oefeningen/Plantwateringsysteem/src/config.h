const int RESISTIEF_DROOG_INTERVAL_MIN = 0;
const int RESISTIEF_DROOG_INTERVAL_MAX = 2178;

const int RESISTIEF_VOCHTIG_INTERVAL_MIN = 2179;
const int RESISTIEF_VOCHTIG_INTERVAL_MAX = 2543;

const int RESISTIEF_NAT_INTERVAL_MIN = 2544;
const int RESISTIEF_NAT_INTERVAL_MAX = 3000;

const int CAPACITIEF_DROOG_INTERVAL_MIN = 3200;
const int CAPACITIEF_DROOG_INTERVAL_MAX = 2303;

const int CAPACITIEF_VOCHTIG_INTERVAL_MIN = 2302;
const int CAPACITIEF_VOCHTIG_INTERVAL_MAX = 1802;

const int CAPACITIEF_NAT_INTERVAL_MIN = 1801;
const int CAPACITIEF_NAT_INTERVAL_MAX = 1000;

const float MINIMUM_TEMPERATUUR = 5.0;
const float EXTRA_WATER_TEMPERATUUR = 25.0;

const unsigned long TIMER_DELAY = 20000;

const char BUTTON_PARAM[] = "PUMP";
const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <title>Dashboard Plantbewatering</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
        html {
            font-family: Arial;
            display: inline-block;
            text-align: center;
        }
        h2 {
            font-size: 3.0rem;
        }
        p {
            font-size: 1.5rem;
        }
        body {
            max-width: 600px;
            margin: 0px auto;
            padding-bottom: 25px;
        }
    </style>
</head>
<body>
  <h2>Dashboard plantbewatering</h2>
  <p>
     Percentage bodemvochtigheid: %BODEMVOCHTIGHEID%
  </p>
  <form>
     %BUTTONPLACEHOLDER%
  </form>

<script>
  function clickButton(element) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/update?PUMP=true", true);
    xhr.send();
  }
</script>
</body>
</html>
)rawliteral";