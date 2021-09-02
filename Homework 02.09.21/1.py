import re

text = input()
text = text.split(' ')
chunks = [re.sub("[\W\d_]", "", t) for t in text]


for word in chunks:
    print([word[i:i + 3] for i in range(0, len(word)-2)])

