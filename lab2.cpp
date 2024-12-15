#include <iostream>

using namespace std;

const int NEED_TO_GALOP = 7;

class DynamicArray {
private:
    int* array;
    int capacity;
    int size;

    void resize(int newCapacity) {
        int* newArray = new int[newCapacity];
        for (int i = 0; i < size; ++i) {
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

public:
    DynamicArray() {
        array = new int[2];
        capacity = 2;
        size = 0;
    }

    ~DynamicArray() {
        delete[] array;
    }

    void addElement(int element) {
        if (size == capacity) {
            resize(capacity * 2);
        }
        array[size] = element;
        ++size;
    }

    int getSize() const {
        return size;
    }

    int& operator[](int index) {
        if (index >= 0 && index < size) {
            return array[index];
        } else {
            cout<<"Индекс вне диапазона!";
        }
    }

    void getElement(int index) /*const*/ {
        if (index >= 0 && index < size) {
            cout << "Элемент на индексе " << index << ": " << array[index] << std::endl;
        } else {
            cout << "Индекс вне диапазона!" << std::endl;
        }
    }

    void insertElement(int index, int element) {
        if (index < 0 || index > size) {
            cout << "Индекс вне диапазона!" << std::endl;
            return;
        }

        if (size == capacity) {
            resize(capacity * 2);
        }

        for (int i = size; i > index; --i) {
            array[i] = array[i - 1];
        }

        array[index] = element;
        ++size;
    }

    void printArray() const {
        std::cout << "Элементы массива: ";
        for (int i = 0; i < size; ++i) {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;
    }

    void deleteArray() {
        delete[] array;
        array = nullptr;
        size = 0;
        capacity = 0;
    }
};


void insertionSort(DynamicArray& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int temp = arr[i];
        int j = i - 1;
        
        while (j >= left && arr[j] > temp) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

int gallopSearchLeft(DynamicArray& arr, int value, int start, int end) {
    while (start < end) {
        int mid = (start + end) / 2;
        if (arr[mid] < value) {
            start = mid + 1;
        } else {
            end = mid;
        }
    }
    return start;
}

int gallopSearchRight(DynamicArray& arr, int value, int start, int end) {
    while (start < end) {
        int mid = (start + end) / 2;
        if (arr[mid] <= value) {
            start = mid + 1;
        } else {
            end = mid;
        }
    }
    return start;
}


int calculateMinRun(int n) {
    int r = 0;
    while (n >= 64) {
        r |= (n & 1);
        n >>= 1;
    }
    return n + r;
}


void merge(DynamicArray& arr, int left, int mid, int right) {
    int len1 = mid - left + 1;
    int len2 = right - mid;

    DynamicArray leftArr, rightArr;

    for (int i = 0; i < len1; i++) {
        leftArr.addElement(arr[left + i]);
    }
    for (int i = 0; i < len2; i++) {
        rightArr.addElement(arr[mid + 1 + i]);
    }

    int i = 0, j = 0, k = left;
    int gallopLeft = 0, gallopRight = 0;

    while (i < len1 && j < len2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
            gallopRight = 0;
            if (++gallopLeft >= NEED_TO_GALOP) {
                int newIdx = gallopSearchLeft(rightArr, leftArr[i], j, len2);
                while (j < newIdx) arr[k++] = rightArr[j++];
                gallopLeft = 0;
            }
        } else {
            arr[k++] = rightArr[j++];
            gallopLeft = 0;
            if (++gallopRight >= NEED_TO_GALOP) {
                int newIdx = gallopSearchRight(leftArr, rightArr[j], i, len1);
                while (i < newIdx) arr[k++] = leftArr[i++];
                gallopRight = 0;
            }
        }
    }

    while (i < len1) arr[k++] = leftArr[i++];
    while (j < len2) arr[k++] = rightArr[j++];
}

void timSort(DynamicArray& arr) {
    int n = arr.getSize();
    int minrun = calculateMinRun(n);

    for (int i = 0; i < n; i += minrun) {
        int right = std::min(i + minrun - 1, n - 1);
        insertionSort(arr, i, right);
    }

    for (int size = minrun; size < n; size = 2 * size) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = std::min(left + 2 * size - 1, n - 1);

            if (mid < right) {
                merge(arr, left, mid, right);
            }
        }
    }
}





int main(){
    system("clear");
    DynamicArray myArray;

    int n;
    std::cout << "Введите количество элементов в массиве: ";
    std::cin >> n;

    std::srand(static_cast<unsigned int>(std::time(0)));

    for (int i = 0; i < n; ++i) {
        int randomNum = std::rand() % 1000;
        myArray.addElement(randomNum);
    }

    std::cout << "Массив до сортировки:" << std::endl;
    myArray.printArray();

    timSort(myArray);

    std::cout << "Массив после сортировки:" << std::endl;
    myArray.printArray();
}