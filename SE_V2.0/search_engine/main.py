from dict_lib import MyDict #мой написанный словарь
import re # регулярные выражения
import math as m # блок математики
import pprint # красивый вывод словаря

"""Список всех текстов"""
ALL_TEXTS = (
    "Texts\\food.txt",
    "Texts\\house.txt",
    "Texts\\my_day.txt",
    "Texts\\my_family.txt",
    "Texts\\numbers.txt",
    "Texts\\school.txt",
    "Texts\\vacation.txt"
)

def calc_TF(dictionary,request):
    """
    Считаем сколько раз встречается слово из запроса и делим на общее кол-во слов в документе
    Таким образом для каждого документа у нас будет свой TF показатель
    """
    temp_IDF = 0
    for key in dictionary.my_dict.keys():
        temp = 0
        for word in request: 
            if word in dictionary.my_dict[key]:
                temp+= len(dictionary.my_dict[key][word]) #считаем сколько раз встречается слово в тексте
                temp_IDF += 1 #если слово хоть раз попалось в тексте
        dictionary.dict_weight_TF[key]=temp/dictionary.dict_weight_TF[key]
        #IDF - log(кол-во документов/кол-во документов, в которых есть слово)
        dictionary.IDF=1-m.log10(len(ALL_TEXTS)/(temp_IDF+1.1)+2.1)
        """
        т.к этот показатель относительный
        я добавила некоторых констант, чтобы не было продлем с делением
        или проблем когда таких слов не найдено
        или они есть в каждом документе
        """

def calc_len(dictionary,request):
    """
    Я считала длину между словами по след. принципу 
    есть текст: A B C D
    общая длина = (B-A) + (C-B) + (D-C)
    """
    temp_dict = dict() 
    for key in dictionary.my_dict.keys():
        temp_word = []
        flag = True #флаг который меняет свое значение если есть хоть 1 слово в тексте

        temp_sum_len = 0 #длина между словами
        """left и right это индексы, которые будут использоваться для вывода снипета"""
        left = None #индекс левого(начального) слова
        right = None #индекс правого (конечного) слова

        temp_multi = 0 #множитель который нормирует дилну, если в тексте встретилось меньше слов, чем в запросе
        MAX_LEN = m.exp(25) #если в тексте нет ни одного слова из запроса, то длина будет равна этому значению

        for word in request:
            if word in dictionary.my_dict[key]:
                temp_word.append(word) #собираем слова которые есть в тексте из запроса
                flag = False
            else:
                temp_multi += 1 #если слова нет, то увеличиваем коэф.

        if flag:
            temp_multi = 0

        """В этот блок мы попадем, если только нашлось хоть одно слово из запроса"""
        if not flag:
            temp_sum_len = 0  #обнуляем значение

            left = dictionary.dict_file_all_words[key].index(temp_word[0]) #задаем левый индекс
            right = left #задаем правый индекс
        
            for i in range(len(temp_word)-1):
                temp_sum_len +=abs(dictionary.my_dict[key][temp_word[i+1]][0] - dictionary.my_dict[key][temp_word[i]][0])
                
                if left > dictionary.my_dict[key][temp_word[i+1]][0]:
                    left = dictionary.my_dict[key][temp_word[i+1]][0] #если есть слово левее, то переназначем
                
                if right < dictionary.my_dict[key][temp_word[i+1]][0]:
                    right = dictionary.my_dict[key][temp_word[i+1]][0] #если есть слово правее, то переназначаем
        
        """ 
        Если не было найдено ни одного слова из запроса в тексте
        То мы записываем значение MAX_LEN
        иначе будет записано значение, которое мы вычислили
        """

        temp_dict[key] = [(temp_sum_len+temp_multi*pow(2, temp_multi)) or MAX_LEN,left,right]
        """
            Если в запросе 10 слов, а в тексте присутвует только 2 слова из запроса
            и в тексте они идут друг за другом, тогда длина будет равна нулю
            и это будет самым релевантным ответом на наш запрос
            но это не очень коректно
            поэтому я решила добавить счетчик отсутствущих слов
            он будет накидывать штрафную длину за каждое пропущенное слово
        """

    return temp_dict #возвращаем полученный словарь текст:длина

def find_snipet(dictionary, min_len_dict,request):
    """
    В этой функции на основе полученного словаря длины
    Я буду строить снипеты текста для наиболее релевантных документов
    Снипет строится по след. принципу
    У меня есть словарь, в котором хранятся слова и их позиции в тексте (не символьные, а словарные)
    Пример: Я хочу получить 86 баллов
    я:1, хочу:2, получить:3, 86:4, баллов:5
    имея левую и правую позицию из поиска длины
    мы ищем наши элементы по их значениям и выводим
    """
    flag = True
    list_keys = list(min_len_dict.items())
    list_keys.sort(key=lambda i:i[1]) #в python нет автоматической сортировки словаря, поэтому я сортировала его через список
    for items in list_keys[:3]: #выводим только ТОП-3 
        if items[1][0] != m.exp(25):
            flag = False
            if not flag:
                for word in dictionary.dict_file_all_words[items[0]][items[1][1]:items[1][2]+1]: #берем срез с левого слова до правого
                    if word in request:
                        print(word.upper(),end=" ")
                    else:
                        print(word, end=" ")
                print("\nFilename: {}, длина: {}".format(items[0],items[1][0]))
    if flag:
        print("не было найдено документов подходящих под запрос")
                

def print_TF_IDF_order(dictionary):
    """
    Функция, которая печатает ТОП-3 документа по TF-IDF
    """
    flag = True
    IDF=dictionary.IDF
    list_keys = list(dictionary.dict_weight_TF.items())
    list_keys.sort(key=lambda i:i[1],reverse=True)
    for name in list_keys[:3]:
        if name[1]!=0:
            flag = False
            print("Имя файла: {} TF-IDF равен {}".format(name[0],name[1]*IDF))
    if flag:
        print("не было найдено документов подходящих под запрос")

def dict_to_file (dictionary):
    str_dict = pprint.pformat(dictionary.my_dict)
    with open("texts\\output.txt", "w") as out:
        out.write(str_dict)

dictionary = MyDict() #словарь 

for filename in ALL_TEXTS:
    dictionary.add_text(filename) #читаем все файлы

print("Что будем искать?")

search_string = input()
temp = re.findall(r"([a-zA-Z\-]+)", search_string.lower()) #разбиваю поисковую строку на слова

search_words=[]
for word in temp:
    if word not in search_words: #убираем повторы из запроса
        search_words.append(word)

calc_TF(dictionary,search_words)

print("Вывод TF-IDF ранжировки документов")
print_TF_IDF_order(dictionary)

temp_dict = calc_len(dictionary, search_words)
print("Вывод доукментов по минимальной длине между словами")
find_snipet(dictionary, temp_dict,search_words) #выводим сниппеты

dict_to_file(dictionary) #помещаем полученный словарь в текстовый файл