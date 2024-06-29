/**
 * @file quack.cpp
 * Implementation of the Quack class.
 *
 */

template <class T>
Quack<T>::Quack() {
     data = vector<T>();
     n1 = 0;
     n2 = -1;
}

/**
 * Adds the parameter object to the right of the Quack.
 *
 * @param newItem object to be added to the Quack.
 */
template <class T>
void Quack<T>::pushR(T newItem) {
   data.push_back(newItem);
   n2++;
}

/**
 * Removes the object at the left of the Quack, and returns it to the
 * caller.
 *
 * See .h file for implementation notes.
 *
 * @return The item that used to be at the left of the Quack.
 */
template <class T>
T Quack<T>::popL() { 
    T temp = data[n1];
    n1++;
    int numElements = n2 - n1 + 1;
    if (numElements <= n1){
        for (int i = 0; i < numElements; i++){
            data[i] = data[n1+i];
        }
        data.resize(numElements);
        n2 = numElements-1;
        n1 = 0;
    }
    return temp;
}
/**
 * Removes the object at the right of the Quack, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Quack.
 */
template <class T>
T Quack<T>::popR() {
    T temp = data[n2];
    data.pop_back();
    n2--;
    int numElements = n2 - n1 + 1;
    if (numElements <= n1){
        for (int i = 0; i < numElements; i++){
            data[i] = data[n1+i];
        }
        data.resize(numElements);
        n2 = numElements-1;
        n1 = 0;
    }
    return temp;
}

/**
 * Finds the object at the left of the Quack, and returns it to the
 * caller. Unlike popL(), this operation does not alter the quack.
 *
 * @return The item at the front of the quack.
 */
template <class T>
T Quack<T>::peekL() {
    return data[n1];
}

/**
 * Finds the object at the right of the Quack, and returns it to the
 * caller. Unlike popR(), this operation does not alter the quack.
 *
 * @return the value of The item at the right of the quack.
 */
template <class T>
T Quack<T>::peekR() {
    return data[n2];
}

/**
 * Determines if the Quack is empty.
 *
 * @return bool which is true if the Quack is empty, false otherwise.
 */
template <class T>
bool Quack<T>::isEmpty() const {
    return data.empty();
}
