#include "ring_buff.h"
#include "string.h"

uint8_t ring_buff_init(ring_buff *rb, uint8_t *array, size_t array_size) {
  rb->buff = array;
  rb->size = array_size;
  rb->available = 0;
  rb->head = 0;
  rb->tail = 0;
}

uint8_t ring_buff_clear(ring_buff *rb) {
  rb->available = 0;
  rb->head = 0;
  rb->tail = 0;
}

uint8_t ring_buff_put(ring_buff *rb, uint8_t *data, size_t data_size) {
  if (rb->size - rb->available < data_size) {
    return 0;
  }
  // TODO: Проверить
  size_t part1_size = (data_size <= rb->size - rb->head) ? data_size : rb->size - rb->head;
  size_t part2_size = data_size - part1_size;

  memcpy(&rb->buff[rb->head], data, part1_size);
  memcpy(rb->buff, &data[part1_size], part2_size);
  // for (size_t i = 0; i < data_size; i++) {
  //   rb->buff[(rb->head + i) % rb->size] = data[i];
  // }
  rb->head = (rb->head + data_size) % rb->size;
  rb->available += data_size;
  return 1;
}

uint8_t ring_buff_get(ring_buff *rb, uint8_t *data, size_t data_size) {
  if (data_size > rb->available) {
    return 0;
  }
  // TODO: Проверить
  size_t part1_size = (data_size <= rb->size - rb->head) ? data_size : rb->size - rb->head;
  size_t part2_size = data_size - part1_size;

  memcpy(&rb->buff[rb->head], data, part1_size);
  memcpy(rb->buff, &data[part1_size], part2_size);
  // for (size_t i = 0; i < data_size; i++) {
  //   data[i] = rb->buff[(rb->tail + i) % rb->size];
  // }
  rb->available -= data_size;
  rb->tail = (rb->tail + data_size) % rb->size;
  return 1;
}