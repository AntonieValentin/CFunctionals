#include "functional.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void for_each(void (*func)(void *), array_t list) {
	void *p = list.data;
	for (int i = 1; i <= list.len; i++) {
		func(p);
		p += list.elem_size;
	}
}

array_t map(void (*func)(void *, void *),
			int new_list_elem_size,
			void (*new_list_destructor)(void *),
			array_t list)
{
	array_t new_list = list;
	new_list.elem_size = new_list_elem_size;
	new_list.destructor = new_list_destructor;
	new_list.len = list.len;
	void *p = list.data;
	void *k = calloc(new_list.elem_size, new_list.len);
	if (!k)
		exit(1);
	new_list.data = k;
	for (int i = 1; i <= list.len; i++) {
		func(k, p);
		p += list.elem_size;
		k += new_list.elem_size;
	}

	p = list.data;
	for (int i = 1; i <= list.len; i++) {
		if (list.destructor)
			list.destructor(p);
		p += list.elem_size;
	}

	free(list.data);
	return new_list;
}

array_t filter(boolean(*func)(void *), array_t list) {
	array_t new_list = list;
	new_list.elem_size = list.elem_size;
	new_list.len = 0;
	new_list.data = NULL;
	void *p = list.data;
	for (int i = 1; i <= list.len; i++) {
		if (func(p) == 1)
			new_list.len++;
		p += list.elem_size;
	}
	void *k = calloc(new_list.len, new_list.elem_size);
	if (!k)
		exit(1);
	new_list.data = k;
	p = list.data;
	for (int i = 1; i <= list.len; i++) {
		if (func(p) == 1) {
			memcpy(k, p, new_list.elem_size);
		k += new_list.elem_size;
		}
		p += list.elem_size;
	}
	free(list.data);
	return new_list;
}

void *reduce(void (*func)(void *, void *), void *acc, array_t list) {
	void *p = list.data;
	for (int i = 1; i <= list.len; i++) {
		func(acc, p);
		p += list.elem_size;
	}
	return acc;
}

void for_each_multiple(void (*func)(void **), int varg_c, ...) {
	va_list p;
	va_start(p, varg_c);
	int min = va_arg(p, array_t).len;
	for (int i = 2; i <= varg_c; i++) {
		int val = va_arg(p, array_t).len;
		if (val < min)
			min = val;
	}
	void **m = calloc(varg_c, sizeof(void *));
	if (!m)
		exit(1);
	va_end(p);
	va_start(p, varg_c);
	int coef;
	coef = 0;
	for (int k = 1; k <= min; k++) {
		va_start(p, varg_c);
	for (int i = 0; i < varg_c; i++) {
		array_t lista = va_arg(p, array_t);
		m[i] = lista.data + lista.elem_size * coef;
	}
	coef++;
	func(m);
	va_end(p);
	}
	free(m);
}

array_t map_multiple(void (*func)(void *, void **), int new_list_elem_size,
					 void (*new_list_destructor)(void *), int varg_c, ...) {
	va_list p;
	va_start(p, varg_c);
	int min = va_arg(p, array_t).len;
	for (int i = 2; i <= varg_c; i++) {
		int val = va_arg(p, array_t).len;
		if (val < min)
			min = val;
	}
	array_t new_list;
	new_list.elem_size = new_list_elem_size;
	new_list.data = calloc(min, new_list_elem_size);
	if (!new_list.data)
		exit(1);
	new_list.len = min;
	new_list.destructor = new_list_destructor;
	void **m;
	m = calloc(varg_c, sizeof(void *));
	if (!m)
		exit(1);
	va_end(p);
	va_start(p, varg_c);
	int coef;
	coef = 0;
	void *j = new_list.data;
	for (int k = 1; k <= min; k++) {
		va_start(p, varg_c);
		for (int i = 0; i < varg_c; i++) {
			array_t lista = va_arg(p, array_t);
			m[i] = lista.data + lista.elem_size * coef;
		}
		coef++;
		func(j, m);
		j += new_list_elem_size;
		va_end(p);
	}
	va_start(p, varg_c);
	for (int i = 0; i < varg_c; i++) {
		array_t lista = va_arg(p, array_t);
		void *v = lista.data;
		for (int j = 1; j <= lista.len; j++) {
			if (lista.destructor)
				lista.destructor(v);
			v += lista.elem_size;
	}
	free(lista.data);
	}
	va_end(p);
	free(m);
	return new_list;
}

void *reduce_multiple(void (*func)(void *, void **),
					  void *acc, int varg_c, ...) {
	va_list p;
	va_start(p, varg_c);
	int min = va_arg(p, array_t).len;
	for (int i = 2; i <= varg_c; i++) {
		int val = va_arg(p, array_t).len;
		if (val < min)
			min = val;
	}
	void **m;
	m = calloc(varg_c, sizeof(void *));
	if (!m)
		exit(1);
	va_end(p);
	va_start(p, varg_c);
	int coef;
	coef = 0;
	for (int k = 1; k <= min; k++) {
		va_start(p, varg_c);
		for (int i = 0; i < varg_c; i++) {
			array_t lista = va_arg(p, array_t);
			m[i] = lista.data + lista.elem_size * coef;
		}
		coef++;
		func(acc, m);
		va_end(p);
	}
	free(m);
	return acc;
}
