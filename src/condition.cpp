
#include "condition_list.hpp"

ConditionList::~ConditionList()
{
  Node *node = mHead;
  while (node)
  {
    Node *next = node->mNext;
    delete node;
    node = next;
  }
}

void ConditionList::add(Condition *aCondition)
{
  Node *head = mHead;
  mHead = new Node;
  mHead->mCondition = aCondition;
  mHead->mPrevious = 0;  
  mHead->mNext = head;
}
