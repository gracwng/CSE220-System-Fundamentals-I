const char *packets[] = {
"\x00\x7d\x15\xa0\x7c\x93\xe1\x2d\x00\x00\x02\x4d\x04\x65\xb0\xc6\xff\xff\xff\xfe\xff\xff\xa4\xfc\xff\xff\xff\xfa\xff\xff\xff\xf8\xff\xff\xff\xf6",
"\x00\x7d\xff\xa0\x7c\x93\xe1\x2d\x01\x90\x01\x4d\x04\x66\x2c\xc6\xff\xff\xff\xba",
"\x00\x7d\x15\xa0\x7c\x93\xe1\x2d\x00\xa0\x02\x4d\x0a\x5a\x90\xc6\x3b\x9a\xca\x16\x3b\x9a\xca\x18\x3b\x9a\xca\x1a\x3b\x9a\xca\x02\x3b\x9a\xca\x1e",
"\x00\x7d\x15\xa0\x7c\x93\xe1\x2d\x01\x40\x02\x4d\x04\x67\x22\xc6\x00\x00\x00\x3c\xff\xff\xff\xc2\xff\xff\xff\xc0\xff\xff\xff\xbe\xff\xff\xff\xbc",
"\x00\x7d\x15\xa0\x7c\x93\xe1\x2d\x00\xf0\x02\x4d\x04\x66\xdc\xc6\x00\x00\x00\x32\x00\x00\x00\x34\x00\x00\x00\x36\x00\x00\x00\x38\x00\x00\x00\x3a",
"\x00\x7d\x15\xa0\x7c\x93\xe1\x2d\x00\x50\x02\x4d\x04\x65\xf6\xc6\x00\x00\x00\x0c\x00\x00\x00\x0e\x00\x00\x00\x10\x00\x00\x00\x12\x00\x00\x00\x14",
};
int reconstructed_array[] = {366059, -438149, 308188, 337875, 279409, 352166, 383100, -150811, 339004, -117725, 192925, 251745, -281364, 108083, -280129, 248978, -198909, -34268, 190780, -364879, 56147, 376455, 428668, -321580, 143290, -498948, 484241, -186267, -455707, 378225, };
int expected_array[] = {366059, -438149, 308188, 337875, 279409, 12, 14, 16, 18, 20, 192925, 251745, -281364, 108083, -280129, 50, 52, 54, 56, 58, 60, -62, -64, -66, -68, -498948, 484241, -186267, -455707, 378225, };
unsigned int expected_num_elements = 15;
