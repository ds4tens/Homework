import re

class MyDict:
    """
    Класс словаря в котором будет хранится
    Название текста: Слово: Позиции
    """
    def __init__(self):
        self.my_dict = dict() #основной словарь файл:слова:позиции
        self.dict_weight_TF=dict() #словарь индексов TF
        self.IDF=0 #IDF
        self.dict_file_all_words=dict() #словарь всех слов текста без их позиции
    
    def __file_check(self,filename):
        """Функция проверки существования файлы"""
        try:
            open(filename)
            return True
        except IOError:
            print("Неверное имя файла, в словарь ничего не добавили")
            return 0

    def __create_dict_weight(self,num):
        """Создаем TF словарь"""
        key = list(self.my_dict.keys())[-1]
        self.dict_weight_TF[key] = num #TF словарь

    def add_text(self, filename):
        if self.__file_check(filename):
            self.my_dict[filename[6:-4]]=dict() #создается словарь на текстовый файл
            with open(filename) as file:
                words = []
                words = re.findall(r"([a-zA-Z\-]+)", file.read().lower()) #собираем все слова
                self.__create_dict_weight(len(words))  #создаем словарь TF
                self.dict_file_all_words[filename[6:-4]] = words #добавляем все слова
                for index in range(len(words)):
                    try:
                        self.my_dict[filename[6:-4]][words[index]].append(index) #добавляю слова в словарь и их позиции в тексте
                    except KeyError:
                        self.my_dict[filename[6:-4]][words[index]] = [] #если слова нет, то соаздаем список для позиций
                        self.my_dict[filename[6:-4]][words[index]].append(index) #добавляю позицию слова
        else:
            pass
