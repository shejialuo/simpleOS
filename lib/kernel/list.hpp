#ifndef _LIB_KERNEL_LIST_HPP
#define _LIB_KERNEL_LIST_HPP

#include "global.hpp"

#define offset(struct_type,member) (int)(&((struct_type*)0)->member)
#define elem2entry(struct_type, struct_member_name, elem_ptr) \
	 (struct_type*)((int)elem_ptr - offset(struct_type, struct_member_name))

struct ListElem {
  struct ListElem* previous;
  struct ListElem* next;
};

using element_t = struct ListElem;

struct List {
  element_t head;
  element_t tail;
};

using list_t = struct List;

using callback = bool (element_t* element, int argument);

void listInitialization(list_t* list);
void listInsertBefore(element_t* before, element_t* element);
void listPush(list_t* list, element_t* element);
void listAppend(list_t* list, element_t* element);
void listRemove(element_t * element);
element_t* listPop(list_t* list);
bool elementFind(list_t* list, element_t* element);
element_t* listTraversal(list_t* list, callback function, int argument);
uint32_t listLength(list_t* list);
bool listEmpty(list_t* list);

#endif // _LIB_KERNEL_LIST_HPP
