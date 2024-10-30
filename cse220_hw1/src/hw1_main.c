#include <stdio.h>
#include "hw1.h"

int main()
{
   int array[] = {-6, 823, 9, 1888, 0, -17, 9999999, -888888, 723, 1000, 1111};
unsigned char* actual_packets[4] = {0}; 
unsigned int num_expected_packets = 4;
unsigned int max_payload = 3 * sizeof(int);
unsigned int src_addr = 93737;
unsigned int dest_addr = 10973;
unsigned int src_port = 11;
unsigned int dest_port = 6;
unsigned int maximum_hop_count = 25;
unsigned int compression_scheme = 3;
unsigned int traffic_class = 14;

packetize_array_sf(array, 11, actual_packets, num_expected_packets, max_payload, src_addr, dest_addr, src_port, dest_port, maximum_hop_count, compression_scheme, traffic_class);
    //    print_packet_sf(packet);

    // compute_checksum_sf(packet);

//     unsigned char *packets[] = {
//         // \0 represents null
// "\x00\x16\xe2\x90\x00\x2a\xdd\xb6\x00\x00\x02\x4c\x81\x9c\xa4\xce\x00\x00\x00\x11\x00\x00\x00\x59\x00\x00\x00\x2a\x00\x00\x02\x77\x00\x00\x00\x34",
// "\x00\x16\xe2\x90\x00\x2a\xdd\xb6\x00\xa0\x02\x4c\x81\xa7\xee\xce\x00\x00\x03\x37\x00\x00\x03\x20\x00\x00\x04\x00\x00\x00\x04\x01\x00\x00\x00\x09",
// "\x00\x16\xe2\x90\x00\x2a\xdd\xb6\x00\xf0\x02\x4c\x9a\x37\xa5\xce\x00\x00\x07\x60\x00\x00\x00\x00\xff\xff\xff\xef\x00\x00\x00\x13\x00\x98\x96\x7f",
// "\x00\x16\xe2\x90\x00\x2a\xdd\xb6\x00\x50\x02\x4c\x81\x9b\x06\xce\x00\x00\x00\x4d\x00\x00\x00\x59\x00\x00\x00\x64\x00\x00\x00\x7d\xff\xff\xff\xfa",
// };
// unsigned char *string1 = packets[0];
// unsigned char string1[37] = packets[0];
// printf("%x", packets[0][1]);

    return 0;
}

// Part 1: Print the Fields and Payload of an ArrayNet Network Packet
// Given an ArrayNet network packet in the argument packet, the print_packet function prints
// the fields in the order shown as in the example in the previous section, ending each line with a
// newline. It prints all fields in decimal. The function may not print blank lines, extra spaces or
// extra newlines, labels, or any other output. You may not use the ntohl() function or similar built-in
// functions to perform any data conversions for you. For the payload, combine four-byte chunks to
// create 32-bit signed integers. Note carefully how the bytes of each integer are ordered in the payload.
