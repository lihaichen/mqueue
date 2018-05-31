
#include "list.h"

/**
 * @brief 双向循环链表初始化
 *
 * @param l 链表头
 */
void list_init(list_t* l) {
  l->next = l->prev = l;
}

/**
 * @brief 添加节点到链表尾
 *
 * @param l 链表
 * @param n 节点
 */
void list_insert_after(list_t* l, list_t* n) {
  l->next->prev = n;
  n->next = l->next;

  l->next = n;
  n->prev = l;
}

/**
 * @brief 添加节点到链表头
 *
 * @param l 链表
 * @param n 节点
 */
void list_insert_before(list_t* l, list_t* n) {
  l->prev->next = n;
  n->prev = l->prev;

  l->prev = n;
  n->next = l;
}

/**
 * @brief 移除节点
 *
 * @param n 节点
 */
void list_remove(list_t* n) {
  n->next->prev = n->prev;
  n->prev->next = n->next;

  n->next = n->prev = n;
}

/**
 * @brief 链表判空
 *
 * @param l 链表
 *
 * @return 为空返回1，不为空返回0
 */
int list_isempty(const list_t* l) {
  return l->next == l;
}
