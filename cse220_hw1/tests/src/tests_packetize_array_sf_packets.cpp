#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "gtest/gtest.h"
#include "tests_aux.h"
extern "C" {
#include "hw1.h"
}

class packetize_array_sf_TestSuite : public testing::Test { };

TEST(packetize_array_sf_TestSuite, Test1_contents_of_packets_array)
{
	INFO("Packetize a short array. packets[] is the optimal length. All packets have the same size.");
	#include "packetize_array_sf_args01.h"
	const char *expected_packets[] = {
	"\x00\x1e\x0f\x32\x0e\xf4\x86\xcd\x00\x00\x02\x07\x10\xd6\x01\x0f\x00\x00\x00\x0a\x00\x00\x00\x0b\x00\x00\x00\x0c\x00\x00\x00\x0d",
	"\x00\x1e\x0f\x32\x0e\xf4\x86\xcd\x00\x40\x02\x07\x10\xd6\x21\x0f\x00\x00\x00\x0e\x00\x00\x00\x0f\x00\x00\x00\x10\x00\x00\x00\x11",
	"\x00\x1e\x0f\x32\x0e\xf4\x86\xcd\x00\x80\x02\x07\x10\xd6\x41\x0f\x00\x00\x00\x12\x00\x00\x00\x13\x00\x00\x00\x14\x00\x00\x00\x15",
	};
	unsigned int expected_packet_lengths[] = {32, 32, 32, };
	packetize_array_sf(array, sizeof(array) / sizeof(array[0]), actual_packets,
			sizeof(actual_packets) / sizeof(actual_packets[0]), max_payload, src_addr, dest_addr, src_port, dest_port, 
			maximum_hop_count, compression_scheme, traffic_class);
	verify_packets((char **)expected_packets, actual_packets, expected_packet_lengths, num_expected_packets);
}

TEST(packetize_array_sf_TestSuite, Test2_contents_of_packets_array)
{
	INFO("Packetize a short array. packets[] is the optimal length. Last packet is smaller than the rest.");
	#include "packetize_array_sf_args02.h"
	const char *expected_packets[] = {
	"\x00\x16\xe2\x90\x00\x2a\xdd\xb6\x00\x00\x01\xcc\x81\x9c\xa3\xce\xff\xff\xff\xfa\x00\x00\x03\x37\x00\x00\x00\x09",
	"\x00\x16\xe2\x90\x00\x2a\xdd\xb6\x00\x30\x01\xcc\x81\xa0\xda\xce\x00\x00\x07\x60\x00\x00\x00\x00\xff\xff\xff\xef",
	"\x00\x16\xe2\x90\x00\x2a\xdd\xb6\x00\x60\x01\xcc\xa7\xc3\x00\xce\x00\x98\x96\x7f\xff\xf2\x6f\xc8\x00\x00\x02\xd3",
	"\x00\x16\xe2\x90\x00\x2a\xdd\xb6\x00\x90\x01\x8c\x81\xa1\xbc\xce\x00\x00\x03\xe8\x00\x00\x04\x57",
	};  
	unsigned int expected_packet_lengths[] = {28, 28, 28, 24, };
	packetize_array_sf(array, sizeof(array) / sizeof(array[0]), actual_packets,
			sizeof(actual_packets) / sizeof(actual_packets[0]), max_payload, src_addr, dest_addr, src_port, dest_port, 
			maximum_hop_count, compression_scheme, traffic_class);
	verify_packets((char **)expected_packets, actual_packets, expected_packet_lengths, num_expected_packets);
}
// "\x00\x16\xe2\x90\x00\x2a\xdd\xb6\x00\x00\x01\xcc\x81\x9c\xa3\xce
// 00 16 e2 90 00 2a dd b6 00 00 00 0c 80 00 00 c0
TEST(packetize_array_sf_TestSuite, Test3_contents_of_packets_array)
{
	INFO("Packetize a long array. packets[] is not long enough.");
	#include "packetize_array_sf_args03.h"
	const char *expected_packets[] = {
	"\x00\x16\xe2\x90\x00\x2a\xdd\xb6\x00\x00\x02\x4c\x81\x9c\xa4\xce\x00\x00\x00\x11\x00\x00\x00\x59\x00\x00\x00\x2a\x00\x00\x02\x77\x00\x00\x00\x34",
	"\x00\x16\xe2\x90\x00\x2a\xdd\xb6\x00\x50\x02\x4c\x81\x9b\x06\xce\x00\x00\x00\x4d\x00\x00\x00\x59\x00\x00\x00\x64\x00\x00\x00\x7d\xff\xff\xff\xfa",
	"\x00\x16\xe2\x90\x00\x2a\xdd\xb6\x00\xa0\x02\x4c\x81\xa7\xee\xce\x00\x00\x03\x37\x00\x00\x03\x20\x00\x00\x04\x00\x00\x00\x04\x01\x00\x00\x00\x09",
	"\x00\x16\xe2\x90\x00\x2a\xdd\xb6\x00\xf0\x02\x4c\x9a\x37\xa5\xce\x00\x00\x07\x60\x00\x00\x00\x00\xff\xff\xff\xef\x00\x00\x00\x13\x00\x98\x96\x7f",
	};
	unsigned int expected_packet_lengths[] = {36, 36, 36, 36, };
	packetize_array_sf(array, sizeof(array) / sizeof(array[0]), actual_packets,
			sizeof(actual_packets) / sizeof(actual_packets[0]), max_payload, src_addr, dest_addr, src_port, dest_port, 
			maximum_hop_count, compression_scheme, traffic_class);
	verify_packets((char **)expected_packets, actual_packets, expected_packet_lengths, num_expected_packets);
}

TEST(packetize_array_sf_TestSuite, Test4_contents_of_packets_array)
{
	INFO("Packetize a long array. packets[] is long enough.");
	#include "packetize_array_sf_args04.h"
	const char *expected_packets[] = {
	"\x00\x02\xb6\x70\x00\x56\xce\xde\x00\x00\x02\x88\x96\x5f\x0c\x6c\xff\xf5\x28\x27\x00\x1d\x76\x86\xff\xcd\x89\x29\xff\xfa\x75\xb3\x00\x29\xed\x91\xff\xf4\x60\xbf",
	"\x00\x02\xb6\x70\x00\x56\xce\xde\x00\x60\x02\x88\xb9\x90\x8b\x6c\x00\x4a\x8b\x8a\x00\x48\xa3\xac\x00\x17\x24\x69\x00\x46\xd2\x8d\x00\x03\x16\x7a\xff\xbb\x2e\xeb",
	"\x00\x02\xb6\x70\x00\x56\xce\xde\x00\xc0\x02\x88\xb0\x5c\x97\x6c\xff\xe7\x1b\xb2\xff\xda\x28\xbb\xff\xfe\x70\xa3\x00\x26\x5c\x06\x00\x02\xc1\x3c\xff\xb9\x8e\x82",
	"\x00\x02\xb6\x70\x00\x56\xce\xde\x01\x20\x02\x88\xfc\x65\x0b\x6c\xff\xf2\x1a\x14\xff\xe8\x6f\xb8\x00\x00\x14\x5b\xff\xcc\x0c\xa8\xff\xfb\x76\x27\x00\x1d\xda\x4d",
	"\x00\x02\xb6\x70\x00\x56\xce\xde\x01\x80\x02\x88\xdb\xeb\xd0\x6c\x00\x16\x5d\xb6\xff\xc3\x4b\x1a\x00\x38\xce\x6e\xff\xeb\xa1\xa3\x00\x02\xa7\xf3\x00\x38\x81\x5f",
	"\x00\x02\xb6\x70\x00\x56\xce\xde\x01\xe0\x02\x88\xc5\x44\xa8\x6c\xff\xca\xce\x89\xff\xd1\xea\x5a\x00\x09\xac\xa6\x00\x0b\x08\x72\x00\x3f\x10\xcd\x00\x0d\xb4\x77",
	"\x00\x02\xb6\x70\x00\x56\xce\xde\x02\x40\x02\x88\xa3\xa3\xcc\x6c\x00\x4b\xb4\x71\x00\x3d\x51\x18\x00\x30\x72\x63\x00\x16\x8d\x9a\xff\xc5\xa7\x65\xff\xe7\x3d\x9d",
	"\x00\x02\xb6\x70\x00\x56\xce\xde\x02\xa0\x02\x88\xb8\xdf\xdc\x6c\x00\x44\x6d\x45\xff\xf9\x0e\x57\x00\x34\x17\x18\x00\x1c\xf8\x29\xff\xf9\x84\x9e\xff\xea\x8d\x14",
	"\x00\x02\xb6\x70\x00\x56\xce\xde\x03\x00\x01\x88\x81\xa4\xc4\x6c\x00\x49\x37\x6b\x00\x37\xe9\xf2",
	};
	unsigned int expected_packet_lengths[] = {40, 40, 40, 40, 40, 40, 40, 40, 24, };
	packetize_array_sf(array, sizeof(array) / sizeof(array[0]), actual_packets,
			sizeof(actual_packets) / sizeof(actual_packets[0]), max_payload, src_addr, dest_addr, src_port, dest_port, 
			maximum_hop_count, compression_scheme, traffic_class);
	verify_packets((char **)expected_packets, actual_packets, expected_packet_lengths, num_expected_packets);
}