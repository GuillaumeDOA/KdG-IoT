from coapthon.client.helperclient import HelperClient

host = "10.6.121.220"
port = 5683
moisturePath = "/api/v1/wihR0tzGRvtrEAatL6Q1/telemetry"
lightPath = "/api/v1/g5F2BzUTbcinMCyyOVme/telemetry"
humidPath = "/api/v1/4Qcz6PoC4KLJlPJFEzie/telemetry"

moisturePayload = "{moisture:25}"
lightPayload = "{illuminance:350}"
humidPayload = "{humidity:30,temperature:21}"

client = HelperClient(server=(host, port))

# POST-Request
client.post(lightPath, lightPayload)
