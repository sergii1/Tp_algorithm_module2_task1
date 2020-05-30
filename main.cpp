#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <map>

using namespace std;

/*Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
 * Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
 когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки
 принадлежности данной строки множеству.
1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
*/

class HashTable {
public:
    HashTable() {
        size = 0;
        capacity = 8;
        buffer = new string[capacity];
        for (int i = 0; i < capacity; ++i) {
            buffer[i] = nil;
        }
    }

    // Вставка ключа в хеш-таблицу
    bool insert(const string &str) {
        if (str.empty() || isNil(str) || isDeleted(str))
            return false;
        if (float(size) / float(capacity) >= 0.75)
            resize();

        int h = h1(str);
        int insertPos = -1;
        for (int i = 0; i < capacity; ++i) {
            h = hash(str, i, h);
            if (buffer[h] == str)
                return false;
            if (isNil(buffer[h])) {
                insertPos = insertPos == -1 ? h : insertPos;
                size++;
                buffer[insertPos] = str;
                return true;
            }
            if (isDeleted(buffer[h])&&insertPos==-1) {
                insertPos = h;
            }
        }

        if (insertPos != -1) {
            size++;
            buffer[insertPos] = str;
            return true;
        }
        return false;
    }

    bool deleteElement(const string &str) {
        if (str.empty() || isNil(str) || isDeleted(str))
            return false;

        int h = h1(str);

        for (int i = 0; i < capacity; ++i) {
            h = hash(str, i, h);
            if (isNil(buffer[h])) {
                return false;
            }
            if (buffer[h] == str) {
                buffer[h] = deleted;
                size--;
                return true;
            }
        }
        return false;
    }

    bool hasElement(const string &str) {
        if (str.empty() || isNil(str) || isDeleted(str))
            return false;
        int h = h1(str);
        for (int i = 0; i < capacity; ++i) {
            h = hash(str, i, h);
            if (isNil(buffer[h])) {
                return false;
            }
            if (buffer[h] == str) {
                return true;
            }
        }
        return false;
    }

    void printBuffer() {
        cout << "buffer is :" << endl;
        for (int i = 0; i < capacity; ++i) {
            if (!isNil(buffer[i]))
                cout << buffer[i] << " " << i << endl;
        }
        cout << endl;
    }

private:
    int h1(const string &str) {
        int hash = 0;
        for (char i : str)
            hash = (hash * p + int(i)) % capacity;
        return hash;
    }


    int hash(const string &str, int i, int prevHash) {
        return (prevHash + i) % capacity;
    }

    bool isNil(const string &str) {
        return str == nil;
    }


    bool isDeleted(const string &str) {
        return str == deleted;
    }

    void resize() {
        string *oldBuffer = buffer;
        int oldCapacity = capacity;

        capacity *= 2;
        size = 1;
        buffer = new string[capacity];
        for (int i = 0; i < capacity; ++i)
            buffer[i] = nil;

        for (int i = 0; i < oldCapacity; ++i) {
            if (oldBuffer[i] != deleted && oldBuffer[i] != nil)
                this->insert(oldBuffer[i]);
        }

        delete[] oldBuffer;
    }


    string *buffer;
    int capacity;
    int size;
    int p = 71;
    string nil = "NILL";
    string deleted = "DELETED";
};

void run(std::istream &in, std::ostream &out) {
    HashTable h;
    string ch;
    string str;
    bool res = false;
    getline(in, str);
    int i = 0;
    while (!str.empty()) {
        ch = str[0];
        const char *word = str.c_str() + 2;
        res = false;
        switch (ch[0]) {
            case '+': {
                res = h.insert(word);
                out << (res ? "OK" : "FAIL") << endl;
                break;
            }
            case '-': {
                res = h.deleteElement(word);
                out << (res ? "OK" : "FAIL") << endl;
                break;
            }
            case '?': {
                res = h.hasElement(word);
                out << (res ? "OK" : "FAIL") << endl;
                break;
            }
        }
        getline(in, str);
    }
}


int main() {
    run(cin, cout);
    return 0;
}
