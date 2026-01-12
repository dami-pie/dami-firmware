
#ifndef __linked_list_h__
#define __linked_list_h__

#include <Arduino.h>
#include <stdexcept>

template <typename T>
class List
{
public:
  // protected:
  class node
  {
  public:
    T item;
    node *next;

    node(T _item, node *_next = NULL) : item(_item), next(_next){};
  };

  typedef node *link;
  link head;
  link tail;
  int size;

  link getNode(int index)
  {
    if (index >= count() || index < (-1 * count()))
      throw std::runtime_error("Index is out of list range!");

    node *curr = head;

    int limit = index + size % size;
    for (int i = 0; i < limit; i++)
      curr = curr->next;

    return curr;
  }

  typedef void (*ForEachCallback)(T cb);

  List()
  {
    size = 0;
    head = NULL;
    tail = NULL;
  }

  ~List()
  {

    for (link t = head; t != NULL; head = t)
    {
      t = head->next;
      delete head;
    }

    size = 0;
    tail = NULL;
  }

  bool isEmpty() const
  {
    return head == NULL;
  }

  void forEach(ForEachCallback cb)
  {
    for (link curr = this->head; curr != NULL; curr = curr->next)
      cb(curr->item);
  }

  int count() const
  {
    return size;
  }

  void remove(int index)
  {
    if (count() == 0)
      throw std::runtime_error("Impossible to remove, list is empty!");

    link n = head;
    if (index == 0)
    {
      head = n->next;
      delete n;
    }
    else
    {
      for (int i = 1; n && i < index; i++)
        n = n->next;

      if (!n)
        throw std::runtime_error("Fail to get node, index null");

      link _ = n->next;
      n->next = _->next;
      delete _;
    }

    size--;
  }

  void add(T item)
  {
    link t = tail;

    tail = new node(item);

    if (tail == NULL)
      throw std::runtime_error("Insufficient memory to create a new node.");

    if (isEmpty())
      head = tail;
    else
      t->next = tail;

    size++;
  }

  T get(int index)
  {
    link n = getNode(index);

    return n->item;
  }

  T operator[](int index)
  {
    return get(index);
  }
};

#endif