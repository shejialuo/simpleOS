#include "list.hpp"
#include "print.hpp"
#include "interrupt.hpp"

/*
  * A function to initialize the double linklist

  * param: struct list
  * return: void
*/
void listInitialization(list_t* list) {
  list->head.previous = nullptr;
  list->head.next = &list->tail;
  list->tail.previous = &list->head;
  list->tail.next = nullptr;
}

/*
  * A function to insert an element before `before`
  * element.

  * param: two list element
  * return: void
*/
void listInsertBefore(element_t* before, element_t* element) {
  InterruptionStatus oldStatus = interruptionDisable();

  before->previous->next = element;
  element->previous = before->previous;
  element->next = before;
  before->previous = element;

  interruptionSetStatus(oldStatus);
}

/*
  * A function to add the element to the list head

  * param: the list and the element
  * return: void
*/
void listPush(list_t* list, element_t* element) {
  listInsertBefore(list->head.next, element);
}

/*
  * A function to add element to the end of the list

  * param: the list and the element
  * return: void
*/
void listAppend(list_t* list, element_t* element) {
  listInsertBefore(&list->tail, element);
}

/*
  * A function to remove the list element

  * param: the element
  * return: void
*/
void listRemove(element_t * element) {
  InterruptionStatus oldStatus = interruptionDisable();

  element->previous->next = element->next;
  element->next->previous = element->previous;

  interruptionSetStatus(oldStatus);
}

/*
  * A function to remove the head of the list
  * and return the element

  * param: the list and the element
  * return: the head element
*/
element_t* listPop(list_t* list) {
  element_t* element = list->head.next;
  listRemove(element);
  return element;
}

/*
  * A function to find the element in the list

  * param: the list and the element
  * return: successful true, failed false
*/
bool elementFind(list_t* list, element_t* element) {
  element_t* p = list->head.next;
  while(p != &list->tail) {
    if(p == element) {
      return true;
    }
    p = p->next;
  }
  return false;
}

/*
  * A function to accept a callback and traverse the list
  * if callback ok, return the element else return the nullptr

  * param:
    1. the list
    2. the callback function
    3. the argument
  * return: the element
*/
element_t* listTraversal(list_t* list, callback function, int argument) {
  element_t* element = list->head.next;

  if(listEmpty(list)) {
    return nullptr;
  }

  while(element != &list->tail) {
    if(function(element,argument)) {
      return element;
    }
    element = element->next;
  }
  return nullptr;
}

/*
  * A function to get the list length

  * param: list
  * return: the list length
*/
uint32_t listLength(list_t* list) {
  element_t* element = list->head.next;
  uint32_t length = 0;
  while (element != &list->tail) {
    length++;
    element = element->next;
  }
  return length;
}

/*
  * A function to tell whether the list is empty

  * param: list
  * return: boolean
*/
bool listEmpty(list_t* list) {
  return (list->head.next == & list->tail ? true: false);
}
