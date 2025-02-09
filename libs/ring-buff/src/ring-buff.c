#include "ring-buff.h"
#include "string.h"

uint8_t ring_buff_init(ring_buff_t *rb, uint8_t *array, size_t array_size) {
  rb->buff = array;
  rb->size = array_size;
  rb->head = 0;
  rb->tail = 0;
  return 1;
}

uint8_t ring_buff_clear(ring_buff_t *rb) {
  rb->head = 0;
  rb->tail = 0;
  return 1;
}

size_t ring_buff_available(size_t buff_size, size_t head, size_t tail) {
  if (tail <= head) {
    return head - tail;
  }
  return (buff_size - tail) + head;
}

uint8_t ring_buff_put(ring_buff_t *rb, uint8_t *data, size_t data_size) {
  size_t local_head = rb->head;
  size_t local_tail = rb->tail;
  if (rb->size - ring_buff_available(rb->size, local_head, local_tail) <= data_size) { // = чтобы head не совпал с tail
    return 0;
  }
  size_t part1_size = (data_size <= rb->size - local_head) ? data_size : rb->size - rb->head;
  size_t part2_size = data_size - part1_size;

  memcpy(&rb->buff[local_head], data, part1_size);
  memcpy(rb->buff, &data[part1_size], part2_size);
  rb->head = (local_head + data_size) % rb->size;
  return 1;
}

uint8_t ring_buff_get(ring_buff_t *rb, uint8_t *data, size_t data_size) {
  size_t local_head = rb->head;
  size_t local_tail = rb->tail;
  if (data_size > ring_buff_available(rb->size, local_head, local_tail)) {
    return 0;
  }
  // TODO: Проверить
  size_t part1_size = (data_size <= rb->size - local_tail) ? data_size : rb->size - local_tail;
  size_t part2_size = data_size - part1_size;

  memcpy(&rb->buff[local_tail], data, part1_size);
  memcpy(rb->buff, &data[part1_size], part2_size);
  rb->tail = (local_tail + data_size) % rb->size;
  return 1;
}