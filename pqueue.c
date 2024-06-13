#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "character/character.h"
#include "pqueue.h"

/* ============================================================================
STRUCTS!
============================================================================ */

/*Usar listas enlazadas junto con la estructura principal*/

struct s_pqueue {
  size_t size;
  struct s_node *first;
};

struct s_node {
  Character personaje;
  struct s_node *next;  
  //charttype_t tipo;
  float iniciativa;
};

/* ============================================================================
INVREP
============================================================================ */

static bool invrep(pqueue q) {
  return q != NULL ;
}

/* ============================================================================
NEW
============================================================================ */

pqueue pqueue_empty(void) {
  pqueue q = malloc (sizeof(struct s_pqueue));
  if (q == NULL) {
    printf ("Error allocating memory");
    exit(EXIT_FAILURE);
  }
  q->first = NULL;
  q->size = 0;
  
  assert(invrep(q) );

  
  return q;
}

/* ============================================================================
ENQUEUE
============================================================================ */

static float calculate_priority(Character character) {

  unsigned int vivo;
  float tipo = 0;

  charttype_t tipo_personaje = character_ctype (character); 
  if (tipo_personaje == agile) {
    tipo = 1.5;
  } else if (tipo_personaje == tank) {
      tipo = 0.8;
  }

  bool esta_vivo = character_is_alive(character);
  if (esta_vivo) {
    vivo = 1;
  }else {
    vivo = 0;
  }
  unsigned int iniciative = character_agility(character) * tipo *  vivo;
  return iniciative;
}



static struct s_node *create_node(Character character) {
  struct s_node *new_node = malloc(sizeof(struct s_node));  
  assert(new_node != NULL);

  new_node->personaje = character;
  new_node->next = NULL;
  new_node->iniciativa = calculate_priority(character);;

  return new_node;
}

pqueue pqueue_enqueue(pqueue q, Character character) {
  assert(invrep(q));
  struct s_node *new_node = create_node(character);
  
  if (q->first == NULL || q->first->iniciativa < new_node->iniciativa) {
    new_node->next = q->first;
    q->first = new_node;
    
  }
  else{  
    struct s_node *p = q->first;
    while (p->next != NULL && calculate_priority(p->next->personaje) >= new_node->iniciativa) {
      p = p->next;
    }
    new_node->next = p->next;
    p->next = new_node;
  }
  q->size ++;
  assert(invrep(q) && !pqueue_is_empty(q));
  return q;
}

/* ============================================================================
IS EMPTY?
============================================================================ */

bool pqueue_is_empty(pqueue q) {
  assert (invrep(q));
  bool b = q->first == NULL;
  return b;
}

/* ============================================================================
PEEKS
============================================================================ */

Character pqueue_peek(pqueue q) {
  Character ch_may_prio = q->first->personaje;
  return ch_may_prio;
}

float pqueue_peek_priority(pqueue q) {
  unsigned int mayor_prioridad;

  mayor_prioridad = q->first->iniciativa;

  return mayor_prioridad;
}

/* ============================================================================
SIZE
============================================================================ */

unsigned int pqueue_size(pqueue q) {
  assert(invrep(q));
  unsigned int size;
  size = q->size;

  return size;
}

/* ============================================================================
COPY
============================================================================ */

pqueue pqueue_copy(pqueue q) {
  assert(invrep(q));
  pqueue copia = pqueue_empty();
  struct s_node *actual = q->first;
  while (actual != NULL) {
    copia = pqueue_enqueue(copia,actual->personaje);
    actual = actual->next;
  }
  return copia;
}

/* ============================================================================
DESTROY!
============================================================================ */
static struct s_node *destroy_node(struct s_node *node) {
  assert(node != NULL);
    node->next=NULL;
    free(node);
    node=NULL;
  assert(node == NULL);
  return node;
}

pqueue pqueue_dequeue(pqueue q) {
  assert(invrep(q));
  struct s_node *p = q->first;
  q->first = p->next;
  p = destroy_node(p);
  q->size--;
  assert(invrep(q));
  return q;
}

pqueue pqueue_destroy(pqueue q) {
    assert(invrep(q));
    struct s_node *node=q->first;
    while (node != NULL) {
        struct s_node *killme=node;
        node = node->next;
        killme = destroy_node(killme);
    }
    free(q);
    q = NULL;
    assert(q == NULL);
    return q;
}