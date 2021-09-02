sizes = list(map(int, input().split()))

if sizes[0] == sizes[3] and sizes[1] == sizes[4] and sizes[2] == sizes[5]:
    print('Ячейки равны')
elif sizes[0] > sizes[3] and sizes[1] == sizes[4] and sizes[2] == sizes[5]:
    print('Первое поле больше второго')
elif sizes[0] < sizes[3] and sizes[1] == sizes[4] and sizes[2] == sizes[5]:
    print('Первое поле меньше второго')
else:
    print('Ящики несопоставимы')
