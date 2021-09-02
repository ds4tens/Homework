from datetime import datetime

text = input()

for i in range(0, len(text) - 10):
    try:
        print(datetime.strptime(text[i:i + 10], '%d.%m.%Y'))
    except ValueError:
        pass
