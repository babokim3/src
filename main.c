//
//  main.c
//  pbm_rw
//
//  Created by Artem Lenskiy on 4/16/15.
//  Copyright (c) 2015 Artem Lenskiy. All rights reserved.
//

//#include "pbm.h"
//
//int main(int argc, const char * argv[]) {
//
//    char *data;
//    struct image_header ih;
//    unsigned long data_size;
//
//    if (argc != 3) {
//        printf("Not enought arguments, the program call should as follows: %s <input.pbm> <output.pbm>", argv[0]);
//        return 0;
//    }
//
//    data = readImage(argv[1], &data_size, &ih);
//    if(data != NULL)
//        writeImage(argv[2], data, ih);
//
//    return 0;
//}

#include "pbm.h"
#include "rtp.h"
#define PAYLOAD_SIZE 1000
#define DATA_SIZE 100000000
int main(int argc, const char * argv[]) {

	char *data;
	struct image_header ih;
	char image_name[] = "/home/user/workspace/pbm/Debug/flowers.pbm";
	//Users/artemlenskiy/Desktop/flowers.pbm";
	char o_image_name[] = "/home/user/workspace/pbm/Debug/ycbcr_flowers.pbm";
	//"/Users/artemlenskiy/Desktop/flowers_o.pbm";

	unsigned long data_size;




	int                 sock, ch;
	struct sockaddr_in  local;
	struct sockaddr_in  to;
	u_int32             rtp_stream_address;
	unsigned long       send_data_length;
	char                *send_data = malloc(DATA_SIZE);

	if (argc != 3) {
		printf("\nUsage: %s <ip> <port>\n", argv[0]);
		exit(1);
	}
	send_data_length = 0;
	//    while ((ch = (int)data) != EOF){ // reading data
	//	int i ;
	//	for( i = 0; i<data_size; i++){
	//		ch=(int)data[i];
	//		printf("%d ",ch);
	//		send_data[send_data_length] = (unsigned char) ch;
	//		send_data_length++;
	//	}
	int i;
	while ((ch = getchar()) != EOF){ // reading data from standard input, could be redirected from a file
		send_data[send_data_length] = (char) ch;
		send_data_length++;

	}

	send_data = readImage(image_name, &send_data_length, &ih);

	if(send_data != NULL)
		writeImage(o_image_name, send_data, ih);

	// initialize RTP stream address
	rtp_stream_address = inet_addr(argv[1]);

	// if we got a valid RTP stream address
	if (rtp_stream_address != 0) {
		sock = socket(AF_INET, SOCK_DGRAM, 0); // create new socket for sending datagrams
		if (sock >= 0) {
			// prepare local address
			memset(&local, 0, sizeof(local));
			local.sin_family      = AF_INET;
			local.sin_port        = htons(INADDR_ANY);
			local.sin_addr.s_addr = htonl(INADDR_ANY);
			// bind to local address
			if (bind(sock, (struct sockaddr *)&local, sizeof(local)) == 0) {
				// prepare RTP stream address
				memset(&to, 0, sizeof(to));
				to.sin_family      = AF_INET;
				to.sin_port        = htons(atoi(argv[2]));
				to.sin_addr.s_addr = rtp_stream_address;
				// send RTP packets

				rtp_send_packets( sock, &to, send_data, send_data_length, PAYLOAD_SIZE);
			}
			close(sock);
		}
	}
	//	free(data);


	return 0;
}

