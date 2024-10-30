#include "hw1.h"
#include <math.h>

// PART 1:
#define FOURBITS 4

// this method performs use bitwise operators AND and shift right to access bitfields
// header: all bits (operand 1)
// setter: the bits needed as second operand to extract the bits we need (operand 2)
// shiftVal: the number of bits we need to shift right to get to our needed bits
unsigned long extractNeededBits(unsigned long header, unsigned long setter, int shiftVal)
{
    // purpose of & and setter is to mask so that I only get info from the bits I need by removing all other bits
    // shifting to the right so the bits I need are in the right place value to be converted from hex to dec
    return ((header & setter) >> shiftVal);
}

// this method extracts information of the packet array and stores up to 8 bytes of information in a single long
// packet: array of characters represented as hexadecimals themselves
// start: the starting byte we want to store
// end: the ending byte we want to store
unsigned long extract8bytes(unsigned char packet[], int start, int end)
{
    // going through each char index to store all hexadecimal into a single string stored as a long
    unsigned long header = packet[start];

    for (int i = start + 1; i < end; i++)
    {
        // shift 8 bits to the left to make space for next 8 bits
        header <<= 8;
        // append next hexadecimal to the long
        header |= packet[i];
    }
    return header;
}

// this method takes an integer decimal value, converts it as a hexadecimal and stores each hexavalue into a character array

// this method extracts information for each integer in the payload and stores it in an array
// p: pointer to the array to store all the decimal values
// size: number of integers in our payload
void extractPayload(unsigned char packet[], int *p, int size)
{
    // start reading in indices from index 16 of the packet array
    int start = 16;
    // traversing through every integer
    for (int i = 0; i < size; i++)
    {
        int val = 0;
        // traversing through 4 bytes in the packet array to extract info on each integer
        for (int j = 0; j < 4; j++)
        {
            if (j != 0)
                val <<= 8;
            val |= packet[start++];
        }
        // printf("%d\n", val);
        p[i] = val;
    }
}

// this method computes all the information in a header and stores it in an array
void extractHeader(unsigned char packet[], int *p)
{
    // going through each char index to store all hexadecimal into a single string stored as a long
    unsigned long header1 = extract8bytes(packet, 0, 8);
    unsigned long header2 = extract8bytes(packet, 8, 16);
    // Source Address: 28-bit network address of the transmitting host
    p[0] = extractNeededBits(header1, 0xFFFFFFF000000000, 36);
    // Destination Address: 28-bit network address of the receiving host
    p[1] = extractNeededBits(header1, 0x0000000FFFFFFF00, 8);
    // Source Port (“SP” in the diagram above): 4-bit network port of the transmitting host
    p[2] = extractNeededBits(header1, 0x00000000000000F0, 4);
    // Destination Port (“DP): 4-bit network port of the receiving host
    p[3] = extractNeededBits(header1, 0x000000000000000F, 0);
    // Fragment Offset: 14-bit byte offset of the payload’s integer in the original array
    p[4] = extractNeededBits(header2, 0xFFFC000000000000, 50);
    // Packet Length: 14-bit total length of the packet in bytes, including the header and payload
    p[5] = extractNeededBits(header2, 0x0003FFF000000000, 36);
    // Maximum Hop Count (“MH”): 5-bit field providing the maximum number of routers the packet may visit while traveling from the source to the destination.
    p[6] = extractNeededBits(header2, 0x0000000F80000000, 31);
    // Checksum: the 23-bit remainder of dividing the sum of:
    // all of the fields (except for the Checksum itself) and
    // the absolute values of the 32-bit integers in the payload
    // by the divisor 223-1. Again, this is a fictional network protocol. This checksum calculation will accept corrupted payloads!
    p[7] = extractNeededBits(header2, 0x000000007FFFFF00, 8);
    // Compression Scheme (“C”): 2-bit field providing the ID # of the compression scheme used to compress the payload.
    p[8] = extractNeededBits(header2, 0x00000000000000C0, 6);
    // Traffic Class (“TC”): 6-bit field used to indicate the quality of service required by this packet.
    p[9] = extractNeededBits(header2, 0x000000000000003F, 0);
}

// PART 1:
// this method prints the entire packet which includes the header and payload
void printPacket(unsigned char packet[])
{
    int header[10] = {0};
    extractHeader(packet, header);
    // now extract the necessary information of each field
    // Source Address: 28-bit network address of the transmitting host
    int source = header[0];
    printf("Source Address: %d\n", source);

    // Destination Address: 28-bit network address of the receiving host
    int destination = header[1];
    printf("Destination Address: %d\n", destination);

    // Source Port (“SP” in the diagram above): 4-bit network port of the transmitting host
    int sourcePort = header[2];
    printf("Source Port: %d\n", sourcePort);

    // Destination Port (“DP): 4-bit network port of the receiving host
    int destinationPort = header[3];
    printf("Destination Port: %d\n", destinationPort);

    // Fragment Offset: 14-bit byte offset of the payload’s integer in the original array
    int fragmentOffset = header[4];
    printf("Fragment Offset: %d\n", fragmentOffset);

    // Packet Length: 14-bit total length of the packet in bytes, including the header and payload
    int packetLength = header[5];
    printf("Packet Length: %d\n", packetLength);

    // Maximum Hop Count (“MH”): 5-bit field providing the maximum number of routers the packet may visit while traveling from the source to the destination.
    int hopCount = header[6];
    printf("Maximum Hop Count: %d\n", hopCount);

    // Checksum: the 23-bit remainder of dividing the sum of:
    // all of the fields (except for the Checksum itself) and
    // the absolute values of the 32-bit integers in the payload
    // by the divisor 223-1. Again, this is a fictional network protocol. This checksum calculation will accept corrupted payloads!
    int checkSum = header[7];
    printf("Checksum: %d\n", checkSum);

    // Compression Scheme (“C”): 2-bit field providing the ID # of the compression scheme used to compress the payload.
    int compressionScheme = header[8];
    printf("Compression Scheme: %d\n", compressionScheme);

    // Traffic Class (“TC”): 6-bit field used to indicate the quality of service required by this packet.
    int trafficClass = header[9];
    printf("Traffic Class: %d\n", trafficClass);

    // Payload: can be of any size
    // 16 bc header is 16 bytes. remaining difference will give us the amoung of storage for the payload.
    // divide by 4 because each integer is 4 bytes, so divide by 4 bytes to get the number of integers
    int numOfIntegers = (packetLength - 16) / FOURBITS;
    // will be using an array to store the integers
    if (numOfIntegers == 0)
    {
        printf("Payload: ");
    }
    else
    {
        // Payload of signed integer data (at most 2^14 - 16 bytes)
        //  if our payload size exceeds the given size, then ignore the bytes that go over the limit
        if ((numOfIntegers * 4) > (pow(2, 14) - 16))
        {
            numOfIntegers = (pow(2, 14) - 16) / FOURBITS;
        }
        int arr[numOfIntegers];
        extractPayload(packet, arr, numOfIntegers);
        printf("Payload: ");
        for (int i = 0; i < numOfIntegers; i++)
        {
            if (i == numOfIntegers - 1)
                printf("%d\n", arr[i]);
            else
                printf("%d ", arr[i]);
        }
    }
}
// ----------------------------------------------------------------------------------------------------------------------------
// PART 2:
#define CHECKSUMDIVISOR (pow(2, 23)) - 1
unsigned int computeChecksum(unsigned char packet[])
{
    // first gather sum of header
    int header[10] = {0};
    extractHeader(packet, header);
    unsigned long sumOfHeader = 0;
    for (int i = 0; i < 10; i++)
    {
        // printf("%d\n", header[i]);
        sumOfHeader += header[i];
        // printf("%lu\n", sumOfHeader);
    }
    // remove checksum (header[7]) from sum
    // printf("Header 7 : %d\n", header[7]);

    sumOfHeader -= header[7];
    // printf("Sum of header: %lu\n", sumOfHeader);

    // now gather sum of payload (accepts corrupted payload)
    // sum of payload means the absolute val of all values in the payload
    unsigned long sumOfPayload = 0;
    // header[5] = packet length
    // this accepts the entire payload even if it's corrupted and it exceeds the accepted limits
    int numOfIntegers = (header[5] - 16) / FOURBITS;
    int payload[numOfIntegers];
    extractPayload(packet, payload, numOfIntegers);
    for (int i = 0; i < numOfIntegers; i++)
    {
        // printf("payload %d\n", payload[i]);

        if ((int)payload[i] < 0)
        {

            sumOfPayload += (-1 * payload[i]);
        }
        else
            sumOfPayload += (payload[i]);
        // printf("sumOfPayload %lu\n", sumOfPayload);
    }
    // printf("Sum of payload %lu\n", sumOfPayload);
    unsigned long sumOfPacket = sumOfHeader + sumOfPayload;
    // printf("Sum of packet %d\n", sumOfPacket);

    // question: does the 23 bit length of the remainder matter?
    unsigned long result = (sumOfPacket) / (CHECKSUMDIVISOR);
    unsigned long remainder = sumOfPacket - (result * (CHECKSUMDIVISOR));
    // printf("Remainder: %lu\n", remainder);
    unsigned int finalResult = (unsigned int)remainder;
    // printf("Final result %d\n", finalResult);
    return finalResult;
}

// ----------------------------------------------------------------------------------------------------------------------------

// PART 3:
// this method reconstructs values in a packet into a single array by
// extracting each packet from an array of packets so their numbers are readable, checks the checksum vals to make sure they match,
// then extracts the values from the payload and puts them in the given array while also making sure we don't go over array bounds
unsigned int reconstructArray(unsigned char *packets[], unsigned int packets_len, int *array, unsigned int array_len)
{
    unsigned int count = 0;
    for (int i = 0; i < packets_len; i++)
    {
        // extract info from the header
        int header[10] = {0};
        extractHeader(packets[i], header);
        // field associations
        // index 4: fragment offset
        // index 7: checksum

        // For each packet, check to make sure checksums match using the checksum_sf function
        unsigned int checksum = header[7];
        // printf("checksum %d\n", checksum);

        unsigned int computedChecksum = compute_checksum_sf(packets[i]);
        // printf("computedChecksum %d\n", computedChecksum);
        // if checksums match:
        if (checksum == computedChecksum)
        {
            // Extract numbers from the payload
            int packetLength = header[5];
            // printf("packetlength %d\n", packetLength);
            int numOfIntegers = (packetLength - 16) / FOURBITS;
            int arr[numOfIntegers];
            extractPayload(packets[i], arr, numOfIntegers);
            // Use the fragment offset to determine which position the integers should be stored at
            int fragmentOffset = header[4] / FOURBITS;
            int j = 0;
            for (int i = fragmentOffset; i < (fragmentOffset + numOfIntegers) && i < array_len; i++)
            {
                array[i] = arr[j++];
                count++;
            }
            // for (int i = 0; i < array_len; i++){
            //     printf("%d. %d\n", i, array[i]);
            // }
        }
    }
    printf("count: %d", count);
    return count;

    // go through each index, store string in a character array. traverse through every hexadeciaml
}

// ----------------------------------------------------------------------------------------------------------------------------
// PART 4:
// this method handles how many bytes should be allocated for each packet

int calculatePacketSize(int totalIntegers, int maxIntsPerPayload)
{
    // number of bytes for each header
    int num_bytes = 16;
    // this handles how many bytes should be allocated for each payload
    int bytesPerPayload = ((totalIntegers - maxIntsPerPayload) >= 0) ? maxIntsPerPayload : totalIntegers;
    bytesPerPayload *= FOURBITS;
    num_bytes += bytesPerPayload;
    // printf("total num bytes for packet 1 %d\n", num_bytes);
    return num_bytes;
}

#define SIZEOFINTEGERINBITS 32
#define EIGHTBITS 8
// this method extracts hexadecimal from a given integer which is represented in binary and stores it in a character array that holds 4 characters (32 bits = 1 integer)
// pointer: points to the character array
// decimal val: the decimal value we are trying to convert
// size of field: the size of each field in bits to figure out which bits to extract from the entire 4 byte array
void extractHexFromBinary(unsigned char *pointer, unsigned int decimalVal, int shift)
{
    // char arrForEachField[4] = {0};
    // 32 - sizeOfField will tell us how many bits we have to shift in order to get the correct ordering of the hexadeciamls
    decimalVal <<= shift;
    int pointerForArr = 0;
    int i = 0;
    int shifter = 32;
    int extracter = 0xFF000000;
    // stores entire 32 bits of a character into an array even if there are some unused bits
    while (i < SIZEOFINTEGERINBITS)
    {
        // printf("Extracter %x\n", extracter);
        unsigned int eightBitBinNum = decimalVal & extracter;
        eightBitBinNum >>= (shifter - EIGHTBITS);
        shifter -= EIGHTBITS;
        unsigned char characters = eightBitBinNum;
        //    printf("Char %x\n", characters);
        // printf("Pointer val before %x\n", pointer[pointerForArr]);
        pointer[pointerForArr] = characters;
        // printf("Pointer val after %x\n", pointer[pointerForArr]);

        pointerForArr++;
        extracter >>= EIGHTBITS;
        i += EIGHTBITS;
    }

    // for (int i = 0; i < 4; i++)
    // {
    //     printf("%x", pointer[i]);
    //     printf("\n");
    // }
}

// now only extract the necessary bits and copy them into the final array
// pointer = pointer to the final packet array of characters
// decimal value = integer value we have to characterize
// fieldSize = upperbounded field size of the field. This tells us which integers we are taking
// startingIndex = inclusive. where to start storing the character
// endingIndex = exclusive
void storeCharactersInArray(unsigned char *pointer, unsigned int deciamlVal, int shift, int fieldSize, int startingIndex, int endingIndex)
{
    unsigned char arrForEachField[4] = {0};

    extractHexFromBinary(arrForEachField, deciamlVal, shift);
    // starting index of where to start reading in from the arr for each field array
    int startingIndexOfField = (int)(4 - (fieldSize / 8.0));
    // printf("%d\n", startingIndexOfField);
    // for (int i = 0; i < 4; i++){
    //                     printf("%x\n", arrForEachField[i]);

    //     }
    for (int i = startingIndex; (i < endingIndex) && (startingIndexOfField < 4); i++)
    {
        // printf("before %x\n", pointer[i]);

        pointer[i] |= arrForEachField[startingIndexOfField++];
        // printf("after %x\n", pointer[i]);
    }
}

// this method creates the header in a character array
void createHeader(unsigned char *pointer, unsigned int src_addr, unsigned int dest_addr,
                  unsigned int src_port, unsigned int dest_port, unsigned int fragmentOffset, unsigned int packetLength, unsigned int maximum_hop_count,
                  unsigned int compression_scheme, unsigned int traffic_class)
{
    // source address:
    storeCharactersInArray(pointer, src_addr, (32 - 28), 32, 0, 4);
    // printf("source %x \n", pointer[3]);

    // destination address:
    storeCharactersInArray(pointer, dest_addr, 0, 32, 3, 7);
    // printf("dest %x \n", pointer[3]);

    // source port:
    storeCharactersInArray(pointer, src_port, 4, 4, 7, 8);

    // destination port:
    storeCharactersInArray(pointer, dest_port, 0, 4, 7, 8);

    // fragment offset:
    storeCharactersInArray(pointer, fragmentOffset, 4, 16, 8, 10);

    // packet length:
    storeCharactersInArray(pointer, packetLength, 4, 24, 9, 12);

    // max hop count:
    storeCharactersInArray(pointer, maximum_hop_count, 7, 16, 11, 13);

    // checksum:
    // storeCharactersInArray(pointer, dest_port, 0, 24, 12, 15);

    // compression scheme:
    storeCharactersInArray(pointer, compression_scheme, 6, 8, 15, 16);

    // traffic class:
    storeCharactersInArray(pointer, traffic_class, 0, 8, 15, 16);
}

// to create payload, we need a pointer to the payload array. We need the starting index of the integer we want to read in (inclusive).
// we need the max number of integers we are allowed to read in (exclusive).
void createPayload(unsigned char *pointer, int startingIndexOfArray, int endingIndexOfArray, int *arr, unsigned int arr_len)
// every integer is 32 bits so just add them to the payload array by 4 indices
{
    // traverse through each integer and allocate 4 indices in the payload array for it
    // extract character array of each integer and copy into payload array
    int pointerForPayload = 0;
    for (int i = 0; i < endingIndexOfArray; i++)
    {
        // printf("Integer decimal %d\n ", arr[i+startingIndexOfArray]);

        unsigned char arrForEachInt[4] = {0};
        extractHexFromBinary(arrForEachInt, arr[i + startingIndexOfArray], 0);
        memcpy((pointer + pointerForPayload), arrForEachInt, (sizeof(arrForEachInt) / sizeof(arrForEachInt[0])));
        // for (int i = 0; i < 12; i++)
        // {
        //     printf("Payload %x\n ", pointer[i]);
        // }
        pointerForPayload += 4;
    }
}
unsigned int packetizeArray(int *array, unsigned int array_len, unsigned char *packets[], unsigned int packets_len,
                            unsigned int max_payload, unsigned int src_addr, unsigned int dest_addr,
                            unsigned int src_port, unsigned int dest_port, unsigned int maximum_hop_count,
                            unsigned int compression_scheme, unsigned int traffic_class)
{

    // first figure out how many packets we need by calculating how many integers can fit in each payload (max_payload/4) = maxInt (rounded up)
    // if maxInt is greater than the packets len size, then set maxInt as the packets len size otherwise keep the same
    // traverse through packets array maxInt times. allocate new memory for each new packet

    int totalIntegers = array_len;
    int maxIntsPerPayload = max_payload / FOURBITS;
    int maxPackets = ((int)(ceil((double)array_len / maxIntsPerPayload)) > packets_len) ? packets_len : (int)(ceil((double)array_len / maxIntsPerPayload));
    unsigned int fragmentOffset = 0;
    // starting index of the array of integers we want to read from
    int indexForArray = 0;
    // traverse through each packet and create memory space for each packet
    for (int i = 0; i < maxPackets; i++)
    // for (int i = 0; i < 1; i++)
    {
        // calculate the size needed for each packet
        int num_bytes = calculatePacketSize(totalIntegers, maxIntsPerPayload);
        totalIntegers -= maxIntsPerPayload;
        // double check this. it could be wrong...
        // printf("num bytes %d\n", num_bytes);
        packets[i] = malloc(num_bytes);
        // packets[i][0] = 1 + 6;
        // printf("%x", packets[i][]);
        // calculate the character array of the header. the only things empty are the fragment offset, packet length and checksum
        unsigned char header[16] = {0};
        createHeader(header, src_addr, dest_addr, src_port, dest_port, fragmentOffset, num_bytes, maximum_hop_count, compression_scheme, traffic_class);
        // check sum will be caluclated last unsigned int checkSum =
        // fragmentOffset += (totalIntegers >= maxIntsPerPayload) ? maxIntsPerPayload : totalIntegers;

        // now create the payload. have a variable to keep score of the index of our integer
        int sizeOfPayload = num_bytes - 16;
        // this gives us the limit of how many elements to read from the integer array
        int numOfIntsPerPayload = sizeOfPayload / 4;
        unsigned char payload[sizeOfPayload]; 
        // for (int i = 0; i < sizeOfPayload; i++){
        //     printf("%d\n", payload[i]);
        // }
        createPayload(payload, indexForArray, numOfIntsPerPayload, array, array_len);
        indexForArray += numOfIntsPerPayload;
        fragmentOffset += maxIntsPerPayload;
        // for (int i = 0; i < 16; i++)
        // {
        //     printf("%x ", header[i]);
        // }
        // for (int i = 0; i < sizeOfPayload; i++)
        // {
        //     printf("%x ", payload[i]);
        // }
        // printf("\n");

        // now copy header and payload into the packet index. and then calculate the checksum
        memcpy(packets[i], header, sizeof(header) / sizeof(header[0]));
        memcpy(packets[i] + 16, payload, sizeof(payload) / sizeof(payload[0]));

        // now calculate the checksum
        unsigned int checksum = computeChecksum(packets[i]);
        storeCharactersInArray(header, checksum, 0, 24, 12, 15);
        memcpy(packets[i], header, sizeof(header) / sizeof(header[0]));

        // printf("%d ", checksum);
        // printf("%x ", checksum);

        // for (int j = 0; j < num_bytes; j++)
        // {
        //     printf("%x ", packets[i][j]);
        // }
        // printf("\n");
    }
    return maxPackets;
}

void print_packet_sf(unsigned char packet[])
{
    (void)packet;
    printPacket(packet);
}

unsigned int compute_checksum_sf(unsigned char packet[])
{
    // checksum, offset and payload
    (void)packet;
    return computeChecksum(packet);
    // return -1;
}

unsigned int reconstruct_array_sf(unsigned char *packets[], unsigned int packets_len, int *array, unsigned int array_len)
{
    (void)packets;
    (void)packets_len;
    (void)array;
    (void)array_len;
    return reconstructArray(packets, packets_len, array, array_len);
    // return -1;
}

unsigned int packetize_array_sf(int *array, unsigned int array_len, unsigned char *packets[], unsigned int packets_len,
                                unsigned int max_payload, unsigned int src_addr, unsigned int dest_addr,
                                unsigned int src_port, unsigned int dest_port, unsigned int maximum_hop_count,
                                unsigned int compression_scheme, unsigned int traffic_class)
{
    (void)array;
    (void)array_len;
    (void)packets;
    (void)packets_len;
    (void)max_payload;
    (void)src_addr;
    (void)dest_addr;
    (void)src_port;
    (void)dest_port;
    (void)maximum_hop_count;
    (void)compression_scheme;
    (void)traffic_class;
    // not sure if i passed in packets correctly
    return packetizeArray(array, array_len, packets, packets_len, max_payload, src_addr, dest_addr, src_port, dest_port, maximum_hop_count, compression_scheme, traffic_class);
    // return -1;
}
