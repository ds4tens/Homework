n = int(input())

if n % 10 in range(5, 10) or n % 10 == 0 or n // 10 == 1:
    suffix = 'коров'
elif n % 10 == 1:
    suffix = 'корова'
else:
    suffix = 'коровы'
print('На лугу пасется', n, suffix)
