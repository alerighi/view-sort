/*
 * View Sort - semplice programmino in C per visualizzare graficamente 
 * i principali algoritmi di sorting.
 * (c) 2016 Alessandro Righi - Licenza MIT
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <SDL2/SDL.h>
#include <string.h>

#define WIN_HEIGHT 500
#define WIN_WIDTH 1000

#define FPS 60

int delay = 1000;

#define SWAP(A, B)  \
{		    \
  int tmp = A;	    \
  A = B;            \
  B = tmp;          \
  usleep(delay);    \
}

struct thread_args {
  int *array;
  int length;
  void (*algoritmo)(int*, int);
};

#define HEAPSIZE(H) H[0]

void heapify(int *heap, int node) {
  int left = 2*node;
  int right = 2*node+1;
  int largest;

  if (left > HEAPSIZE(heap) || heap[node] >= heap[left]) {
    largest = node;
  } else {
    largest = left;
  }

  if (right <= HEAPSIZE(heap) && heap[right] > heap[largest]) {
    largest = right;
  }

  if (node != largest) {
    SWAP(heap[node], heap[largest]);
    heapify(heap, largest);
  }
}

void build_heap(int *heap) {
  for (int i = HEAPSIZE(heap)/2; i > 0; i--) {
    heapify(heap, i);
  }
}

void heap_sort(int *array, int length) {
  int *heap = realloc(array, (length+1)*sizeof(int));
  SWAP(heap[0], heap[length]);
  HEAPSIZE(heap) = length;

  build_heap(heap);

  for (int i = length; i > 1; i--) {
    SWAP(heap[1], heap[i]);
    HEAPSIZE(heap) -= 1;
    heapify(heap, 1);
  }
}

void radix_sort(int *array, int length)
{

  int a, b;
  int digit = 1;
  int *tmp = malloc(length*sizeof(int));

  if (!tmp) {
    perror("Memoria non sufficiente");
    exit(EXIT_FAILURE);
  }

  while (digit) {
    a = 0;
    b = 0;

    for (int i = 0; i < length; i++) {
      if (array[i] & digit) {
	tmp[b] = array[i];
	b++;
	usleep(1000);
      } else {
	array[a] = array[i];
	a++;
	usleep(delay);
      }
    }
    
    for (int i = 0; i < b; i++) {
      array[a] = tmp[i];
      a++;
      usleep(delay);
    }
    
    digit <<= 1; 
  }

  free(tmp);
}

void insertion_sort(int *array, int length)
{
  for (int j = 1; j < length; j++) {
    int key = array[j];
    int i = j - 1;
    while (i >= 0 && array[i] > key) {
      array[i+1] = array[i];
      i--;
      usleep(delay/10);
    }
    array[i+1] = key;
    usleep(delay*2);
  }
}

void merge(int *array, int p, int q, int r, int *tmp)
{
  int j = q+1;
  int k = p;
  int i = 0;
  
  while (p <= q || j <= r) {
    if (p <= q && (j > r || array[p]<=array[j])) {
      tmp[i] = array[p];
      p++;
    } else {
      tmp[i] = array[j];
      j++;
    }
    i++;
  }

  for (i--; i >= 0; i--) {
    array[k+i] = tmp[i];
    usleep(delay*2);
  }
}

void _merge_sort(int *array, int p, int r, int *tmp)
{
  if (p < r) {
    int q = (p+r)/2;
    _merge_sort(array, p, q, tmp);
    _merge_sort(array, q+1, r, tmp);
    merge(array, p, q, r, tmp);
  }
}

void merge_sort(int *array, int length)
{
  int *tmp = malloc(length*sizeof(int));
  _merge_sort(array, 0, length-1, tmp);
  free(tmp);
}


void bubble_sort(int *array, int length)
{
  while (length > 0) {
    for (int i = 0; i < length-1; i++) {
      if (array[i] > array[i+1]) {
	SWAP(array[i], array[i+1]);
      }
    }
    length--;
  }
}

int partition(int *array, int p, int r)
{
  int x = array[p];
  int i = p-1;
  int j = r+1;
  while (true) {
    do {
      j = j-1;
    } while (array[j] > x);
    do {
      i = i+1;
    } while (array[i] < x);
    if (i < j) {
      SWAP(array[j], array[i]);
    } else {
      return j;
    }
  }
}

int random_partition(int *array, int p, int r)
{
  int n = rand()%(r-p) + p;
  SWAP(array[p], array[n]);
  return partition(array, p, r);
}

void _quick_sort(int *array, int p, int r)
{
  if (p < r) {
    int q = random_partition(array, p, r);
    _quick_sort(array, p, q);
    _quick_sort(array, q+1, r);
  }
}
 
void quick_sort(int *array, int length)
{
  _quick_sort(array, 0, length-1);
}

int get_color(int index)
{
  index += 200;
  index %= 500; 
  
  if (index < 250) {
    return index;
  } else {
    return (500-index);
  }
}

void draw_element(SDL_Renderer *renderer, int element, int x, int width)
{
  SDL_SetRenderDrawColor(renderer, get_color(element+150), get_color(element+300),
			 get_color(element+450), 255);
  SDL_Rect rect;
  rect.x = x;
  rect.y = WIN_HEIGHT;
  rect.h = -element;
  rect.w = width;
  SDL_RenderFillRect(renderer, &rect);
  if (width > 4) {
    rect.w += 1;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
  }
}

void draw_array(SDL_Renderer *renderer, int *array, int length)
{
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
  int width = WIN_WIDTH/length;
  for (int i = 0; i < length; i++) {
    draw_element(renderer, array[i], i*width, width);
  }
  SDL_RenderPresent(renderer);
}

int* random_array(int length, int maxval)
{
  int *array = malloc(length*sizeof(int));
  srand(time(NULL));
  for (int i = 0; i < length; i++) {
    array[i] = rand()%maxval;
  }
  return array;
}

void* sort_thread_start(void *args)
{
  struct thread_args *sort_args = (struct thread_args *) args;
  sort_args->algoritmo(sort_args->array, sort_args->length);
  return NULL;
}

void (*get_algorithm(char *algoritmo))(int*, int)
{
   if (algoritmo != NULL) {
    if (!strcmp(algoritmo, "insertion")) {
      delay *= 3;
      return insertion_sort;
    } else if (!strcmp(algoritmo, "bubble")) {
      delay *= 3;
      return bubble_sort;
    } else if (!strcmp(algoritmo, "radix")) {
      delay *= 2;
      return radix_sort;
    } else if (!strcmp(algoritmo, "quick")) {
      delay *= 5;
      return quick_sort;
    } else if (!strcmp(algoritmo, "merge")) {
      delay *= 3;
      return merge_sort;
    } else if (!strcmp(algoritmo, "heap")) {
      delay *= 3;
      return heap_sort;
    } else {
      printf("Algoritmo specificato non valido! Uso merge sort!\n");
    }
   }
   return merge_sort;
}

void usage(void)
{
   printf("usage: visualsort [-asnh]\n"
	     "-a  [algoritmo] algorimi disponibili:\n"
	     "  * bubble\n"
	     "  * insertion\n"
	     "  * merge\n"
	     "  * heap\n"
	     "  * quick\n"
	     "  * radox\n"
	     "-s  velocità di avanzamento (0-200, default 50)\n"
	     "-n  numero elementi array (10-1000, default 1000)\n");
      exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
  
  SDL_Event event;

  int length = 200;
  int *array;

  pthread_t sort_thread;

  char *algoritmo = "merge";
  char title[100];
  
  char c;
  int i;

  void (*sort_algorithm)(int*, int);
  
  while ((c = getopt(argc, argv, "a:hs:n:")) != -1) {
    switch (c) {
    case 'a':
      algoritmo = optarg;
      break;
    case 's':
      i = atoi(optarg);
      if (i < 0 || i > 100) {
	printf("La velocità deve essere compresa fra 0 e 100!\n"
	       "Viene usato il valore di default 50\n");
      } else {
	delay = 200*(101-i);
      }
      break;
    case 'n':
      i = atoi(optarg);
      if (i < 10 || i > 1000) {
	printf("Il numero di elementi deve essere compreso fra 10 e 1000!\n"
	       "Viene usato il valore di default 1000\n");
      } else {
	length = i;
      }
      break;
    case 'h':
      usage();
    }
  }

  delay = delay*1000/length;
  sort_algorithm = get_algorithm(algoritmo);
  array = random_array(length, WIN_HEIGHT);
  
  strcpy(title, algoritmo);
  strcat(title, " sort");
  
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *win = SDL_CreateWindow(title, 100, 100, WIN_WIDTH, WIN_HEIGHT, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

  struct thread_args args;
  args.array = array;
  args.length = length;
  args.algoritmo = sort_algorithm;
  pthread_create(&sort_thread, NULL, sort_thread_start, (void *) &args);
  
  while (true) {

    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
	break;
      }
    }

    draw_array(renderer, array, length);
    usleep(1000000/FPS);
  }

  return EXIT_SUCCESS;
}

