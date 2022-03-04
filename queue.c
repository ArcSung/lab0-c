#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

static element_t *el_new(char *s)
{
    int str_size = sizeof(s);
    element_t *new_el = malloc(sizeof(element_t));

    if (new_el) {
        new_el->value = malloc(str_size);

        if (!new_el->value) {
            free(new_el);
            return NULL;
        }

        memcpy(new_el->value, s, str_size);
        return new_el;
    }

    return NULL;
}
/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
struct list_head *q_new()
{
    struct list_head *new = malloc(sizeof(struct list_head));

    if (new == NULL) {
        return NULL;
    }

    INIT_LIST_HEAD(new);

    return new;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (l == NULL) {
        return;
    }

    while (l->next != l) {
        q_release_element(q_remove_head(l, NULL, 0));
    }

    free(l);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *new_el = el_new(s);

    if (new_el) {
        list_add(&new_el->list, head);
        return true;
    }

    return false;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *new_el = el_new(s);

    if (new_el) {
        list_add_tail(&new_el->list, head);
        return true;
    }

    return false;
}

/*
 * Attempt to remove element from head of queue.
 * Return target element.
 * Return NULL if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 *
 * NOTE: "remove" is different from "delete"
 * The space used by the list element and the string should not be freed.
 * The only thing "remove" need to do is unlink it.
 *
 * REF:
 * https://english.stackexchange.com/questions/52508/difference-between-delete-and-remove
 */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    element_t *tmp_el;
    struct list_head *li;

    if (head == NULL || list_empty(head)) {
        return NULL;
    }

    li = head->next;
    int cpy_size =
        sizeof(tmp_el->value) > bufsize ? bufsize : sizeof(tmp_el->value);
    tmp_el = list_entry(li, element_t, list);
    memcpy(sp, tmp_el->value, cpy_size);

    li->next->prev = head;
    head->next = li->next;

    return tmp_el;
}

/*
 * Attempt to remove element from tail of queue.
 * Other attribute is as same as q_remove_head.
 */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    element_t *tmp_el;
    struct list_head *li;

    if (head == NULL || list_empty(head)) {
        return NULL;
    }

    li = head->prev;
    int cpy_size =
        sizeof(tmp_el->value) > bufsize ? bufsize : sizeof(tmp_el->value);
    tmp_el = list_entry(li, element_t, list);
    memcpy(sp, tmp_el->value, cpy_size);

    head->prev = li->prev;
    li->prev->next = head;

    return tmp_el;
}

/*
 * WARN: This is for external usage, don't modify it
 * Attempt to release element.
 */
void q_release_element(element_t *e)
{
    free(e->value);
    free(e);
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;

    return len;
}

/*
 * Delete the middle node in list.
 * The middle node of a linked list of size n is the
 * ⌊n / 2⌋th node from the start using 0-based indexing.
 * If there're six element, the third member should be return.
 * Return true if successful.
 * Return false if list is NULL or empty.
 */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (head == NULL || list_empty(head)) {
        return false;
    }

    int size = q_size(head) / 2 + 1;
    struct list_head *prev = head->prev;
    struct list_head *cur = head;
    element_t *tmp_el;

    for (int i = 0; i < size; i++) {
        prev = cur;
        cur = cur->next;
    }

    tmp_el = list_entry(cur, element_t, list);
    prev->next = cur->next;
    cur->next->prev = prev;

    q_release_element(tmp_el);

    return true;
}

/*
 * Delete all nodes that have duplicate string,
 * leaving only distinct strings from the original list.
 * Return true if successful.
 * Return false if list is NULL.
 *
 * Note: this function always be called after sorting, in other words,
 * list is guaranteed to be sorted in ascending order.
 */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/*
 * Attempt to swap every two adjacent nodes.
 */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(struct list_head *head)
{
    if (head == NULL) {
        return;
    }

    struct list_head *cur = head;
    struct list_head *prev = head->prev;

    do {
        struct list_head *tmp = cur->next;

        cur->next = prev;
        cur->prev = tmp;

        prev = cur;
        cur = tmp;
    } while (cur != head);

}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(struct list_head *head) {}
