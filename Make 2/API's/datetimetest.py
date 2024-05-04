from datetime import datetime, timedelta

print(datetime.now().strftime("%Y-%m-%dT%H:%MZ"))
print(datetime.now()-timedelta(minutes=1))

