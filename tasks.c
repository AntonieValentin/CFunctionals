#include "functional.h"
#include "tasks.h"
#include "tests.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void f1(void *acc, void *p) {
*(int *)(acc - (*(int *)(acc + 4) + (p - acc) / 4) * sizeof(int)) = *(int *)p;
}

void f1_2(void *p) {
if (*(int *)p == 0)
	*(int *)p = (*(int *)(p - sizeof(int)));
}

void f2(void *p, void **k) {
memcpy(&((number_t *)p)->integer_part, k[0], sizeof(int));
memcpy(&((number_t *)p)->fractional_part, k[1], sizeof(int));
((number_t *)p)->string = calloc(19, sizeof(char));
if (!((number_t *)p)->string)
	exit(1);
snprintf(((number_t *)p)->string, 19, "%d.%d", *(int *)k[0], *(int *)k[1]);
}

void f2_destructor(void *p) {
free(((number_t *)p)->string);
}

boolean f3(void *p) {
if (((student_t *)p)->grade >= 5)
	return 1;
else
	return 0;
}

void f3_2(void *p, void *k) {
void *v = calloc(100, sizeof(char));
if (!v)
	exit(1);
strcpy(v, ((student_t *)k)->name);
memcpy(p, &v, sizeof(char *));
}

void f4_2(void *acc, void *p) {
*(int *)acc += *(int *)p;
}

void f4(void *k, void *p) {
int *acc = calloc(1, sizeof(int));
if (!acc)
	exit(1);
*(int *)acc = 0;
acc = reduce(f4_2, acc, *(array_t *)p);
*(int *)k = *(int *)acc;
free(acc);
}

void f4_3(void *p, void **k) {
if (*(int *)k[0] >= *(int *)k[1])
	*(boolean *)p = true;
else
	*(boolean *)p = false;
(void)p;
}

void f5(void *p) {
if (*(int *)p == 0)
	*(int *)p = (*(int *)(p - sizeof(int))) ^ 1;
}

void f5_2(void *p) {
if (*(int *)p == 0)
	*(int *)p = (*(int *)(p - sizeof(int))) + 1;
}

void f5_3(void *p, void **k) {
*(char **)p = calloc(100, sizeof(char));
if (!*(char **)p)
	exit(1);
strcpy(*(char **)p, *(char **)(k[0] + (*(int *)k[1] - 1) * sizeof(char *)));
}

void f5_destructor(void *p) {
free(*(char **)p);
}

void f6_3(void *p) {
if (*(int *)p == 0)
	*(int *)p = *(int *)(p - sizeof(int)) + 1;
}

void f6(void *p) {
	if ((*(array_t *)p).len == 0) {
		(*(array_t *)p).len = (*(array_t *)(p - sizeof(array_t))).len;
		(*(array_t *)p).destructor = NULL;
		(*(array_t *)p).elem_size = sizeof(int);
		(*(array_t *)p).data = calloc((*(array_t *)p).len, sizeof(int));
		if (!(*(array_t *)p).data)
			exit(1);
		*(int *)(*(array_t *)p).data =
		*(int *)(*(array_t *)(p - sizeof(array_t))).data + 1;
	}
}

void f6_2(void *p) {
for_each(&f6_3, *(array_t *)p);
}

void f6_destructor(void *p) {
free((*(array_t *)p).data);
}

array_t reverse(array_t list) {
array_t new_list;
new_list.elem_size = sizeof(int);
new_list.len = list.len + 1;
new_list.data = calloc(new_list.len, sizeof(int));
if (!new_list.data)
	exit(1);
*(int *)new_list.data =
(new_list.data + (new_list.len - 1) * sizeof(int) - list.data) / 4 - 1;
for_each(&f1_2, new_list);
reduce(&f1, new_list.data + (list.len - 1) * sizeof(int), list);
new_list.len--;
void *p = realloc(new_list.data, new_list.len * sizeof(int));
if (!p)
	exit(1);
new_list.data = p;
return new_list;
}

array_t create_number_array(array_t integer_part, array_t fractional_part) {
	array_t lista = map_multiple(&f2, sizeof(number_t),
								 &f2_destructor, 2, integer_part,
								 fractional_part);
	return lista;
}

array_t get_passing_students_names(array_t list) {
	array_t lista = filter(&f3, list);
	array_t new_list = map(&f3_2, sizeof(char *), NULL, lista);
	return new_list;
}

array_t check_bigger_sum(array_t list_list, array_t int_list) {
	array_t lista = map(&f4, sizeof(int), NULL, list_list);
	array_t new_list = map_multiple(&f4_3, sizeof(boolean), NULL, 2,
									lista, int_list);
	return new_list;
}

array_t get_even_indexed_strings(array_t list) {
	array_t aux;
	aux.len = list.len / 2 + list.len % 2;
	aux.elem_size = sizeof(int);
	aux.destructor = NULL;
	aux.data = calloc(aux.len, aux.elem_size);
	if (!aux.data)
		exit(1);
	*(int *)(aux.data) = 1;
	for_each(&f5_2, aux);
	array_t final_list = map_multiple(&f5_3, sizeof(char *),
									  f5_destructor, 2, list, aux);
	return final_list;
}

array_t generate_square_matrix(int n) {
	array_t new_list;
	new_list.elem_size = sizeof(array_t);
	new_list.len = n;
	new_list.data = calloc(n, sizeof(array_t));
	if (!new_list.data)
		exit(1);
	new_list.destructor = &f6_destructor;
	(*(array_t *)new_list.data).elem_size = sizeof(int);
	(*(array_t *)new_list.data).len = n;
	(*(array_t *)new_list.data).data = calloc(n, sizeof(int));
	if (!(*(array_t *)new_list.data).data)
		exit(1);
	*(int *)(*(array_t *)new_list.data).data = 1;
	(*(array_t *)new_list.data).destructor = NULL;
	for_each(&f6, new_list);
	for_each(&f6_2, new_list);
	return new_list;
}
