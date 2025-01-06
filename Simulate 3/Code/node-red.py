import sys

if len(sys.argv) <= 2:
    exit(0)

topic = sys.argv[1]
value = sys.argv[2]

match topic:
    case "lock":
        print("Lock ", value)
        # SEND TO BLOCKBAX LOGIC
    case "co2":
        print("co2", value)
        # SEND TO BLOCKBAX LOGIC 
    case "humidity":
        print("humidity", value)
        # SEND TO BLOCKBAX LOGIC
    case "temperature":
        print("temperature", value)
        # SEND TO BLOCKBAX LOGIC
