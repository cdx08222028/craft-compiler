/*
    Goblin compiler v1.0 - The standard compiler for the Goblin language.
    Copyright (C) 2016  Daniel McCarthy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   Stack.h
 * Author: Daniel McCarthy
 *
 * Created on 27 June 2016, 21:37
 * 
 * Description: A simple stack template that has a stack pointer
 */

#ifndef STACK_H
#define STACK_H

#include <vector>
#include <stdexcept>

template <class T>
class Stack
{
public:
    void push(T elem);
    T pop();
    void erase(int start, int end);
    void reverse();
    T top();
    T first();
    T pop_first();
    void setEraseMode(bool erase_mode);
    void setSP(int pos);
    bool isEmpty();
    void empty();
    int size();
    int getSP();
    T getByIndex(int index);
    Stack<T> copy(int start, int end);
    std::vector<T> getElementsAsVector();
private:
    std::vector<T> elements;
    int sp = 0;
    bool erase_mode = false;

};

template <class T>
void Stack<T>::push(T elem)
{
    if (erase_mode)
    {
        elements.erase(elements.begin() + sp);
    }
    elements.insert(elements.begin() + sp, elem);
    sp++;
}

template <class T>
T Stack<T>::pop()
{
    T element = top();
    sp--;
    elements.erase(elements.begin() + sp);
    return element;
}

template <class T>
void Stack<T>::erase(int start, int end)
{
    int size = end - start;
    sp -= size;
    elements.erase(elements.begin() + start, elements.begin() + start + end);
}

template <class T>
void Stack<T>::reverse()
{
    Stack<T> reversed_elements;
    int sp = this->getSP();
    while (!this->isEmpty())
    {
        reversed_elements.push(this->pop());
    }
    this->elements = reversed_elements.getElementsAsVector();
    this->setSP(sp);

}

template <class T>
T Stack<T>::top()
{
    if (isEmpty())
    {
        throw std::out_of_range("Stack<>::top(): stack is empty");
    }

    if (sp - 1 >= this->elements.size())
    {
        throw std::out_of_range("Stack<>::top(): Stack pointer is above total existing elements");
    }
    return this->elements.at(sp - 1);
}

template <class T>
T Stack<T>::first()
{
    if (isEmpty())
    {
        throw std::out_of_range("Stack<>::first(): stack is empty");
    }

    return this->elements.at(0);
}

template <class T>
T Stack<T>::pop_first()
{
    T element = first();
    elements.erase(elements.begin());
    return element;
}

template <class T>
void Stack<T>::setEraseMode(bool erase_mode)
{
    this->erase_mode = erase_mode;
}

template <class T>
void Stack<T>::setSP(int pos)
{
    sp = pos;
}

template <class T>
bool Stack<T>::isEmpty()
{
    return elements.empty();
}

template <class T>
void Stack<T>::empty()
{
    sp = 0;
    elements.clear();
}

template <class T>
int Stack<T>::size()
{
    return elements.size();
}

template <class T>
int Stack<T>::getSP()
{
    return this->sp;
}

template <class T>
T Stack<T>::getByIndex(int index)
{
    if (index < elements.size())
        return elements.at(index);

    return NULL;
}

template <class T>
Stack<T> Stack<T>::copy(int start, int end)
{
    Stack<T> stack;
    for (int i = start; i < end; i++)
    {
        stack.push(this->getByIndex(i));
    }
    return stack;
}

template <class T>
std::vector<T> Stack<T>::getElementsAsVector()
{
    return this->elements;
}

#endif /* STACK_H */

