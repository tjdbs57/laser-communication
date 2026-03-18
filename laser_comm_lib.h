// laser_comm_lib.h

#ifndef LASER_COMM_LIB_H
#define LASER_COMM_LIB_H

#include "config.h" 

unsigned char calculate_checksum(const unsigned char *data, int length);
void send_ack_or_nack(unsigned char type); 
unsigned char receive_control_byte_with_timeout(double timeout_s);
int send_data_packet(const char *data, int len); 
int receive_data_packet(unsigned char *buffer, int max_len);
void precise_delay(uint32_t us);
unsigned char read_byte(int pin, uint32_t bit_time_us);

#endif // LASER_COMM_LIB_H