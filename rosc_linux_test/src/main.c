/*
 *	Copyright (c) 2013, Synapticon GmbH
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met:
 *
 *	1. Redistributions of source code must retain the above copyright notice, this
 *	   list of conditions and the following disclaimer.
 *	2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *	The views and conclusions contained in the software and documentation are those
 *	of the authors and should not be interpreted as representing official policies,
 *	either expressed or implied, of the FreeBSD Project.
 *
 *  main.c created by Christian Holl
 */


// /////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// /////////////////////////


#include <rosc/rosc.h>
#include <rosc/system/eth.h>
#include <rosc/sebs_parse_fw/adv_modules/sebs_parse_ros.h>

//The memory definitions for
//the size of all port buffers
//for the biggest available message
STATIC_SYSTEM_MESSAGE_TYPE_LIST_BEGIN
	char justatestsize[23];
STATIC_SYSTEM_MESSAGE_TYPE_LIST_END


char *test_request_message=
		"POST / HTTP/1.1\n"
		"Host: sncn-10:53556\n"
		"Accept-Encoding: gzip\n"
		"User-Agent: xmlrpclib.py/1.0.1 (by www.pythonware.com)\n"
		"Content-Type: text/xml\n"
//		"Content-Encoding: gzip\n"
		"Content-length: 339\n"
		"\n" //157
		"<?xml version='1.0'?>\n"
		"<methodCall>\n"
		"<![CDATA[Inhalt]]]]>"
		"<!-- narf narf -->"
		"<methodName>publisherUpdate</methodName>\n"
		"<params>\n"
		"<param>\n"
		"<value><string>/master</string></value>\n"
		"</param>\n"
		"<param>\n"
		"<value><string>/rosout</string></value>\n"
		"</param>\n"
		"<param>\n"
		"<value><array><data>\n"
		"<value><string>http://sncn-10:56998/</string></value>\n"
		"<value><string>http://192.168.0.1:56998/</string></value>\n"
		"<value><string>http://[2001:db8:85a3:8d3::370:7348]/</string></value>\n"

		"</data></array></value>\n"
		"</param>\n"
		"</params>\n"
		"</methodCall>";


char *test_response_message=
		"HTTP/1.0 200 OK\n"
		"Server: BaseHTTP/0.3 Python/2.7.3\n"
		"Date: Thu, 25 Apr 2013 15:28:39 GMT\n"
		"Content-Type: text/xml\n"
		"Content-LenGth: 303\n"
		"\n"
		"<?xml version='1.0'?>\n"
		"<methodResponse>\n"
		"<params>\n"
		"<param>\n"
		"<value><array><data>\n"
		"<value><int>1</int></value>\n"
		"<value><string>Registered [/ListeningNode] as provider of [/ListeningNode/set_logger_level]</string></value>\n"
		"<value><int>1</int></value>\n"
		"</data></array></value>\n"
		"</param>\n"
		"</params>\n"
		"</methodResponse>";


#include <rosc/com/xmlrpc_server.h>

int main_xmlrpctest()
{
	int rlen;
	int buffersize=1;
//	parse_act_t pact;

#if(1)
	char *msg=test_request_message;

#else
	char *msg=test_response_message;

#endif

	xmlrpc_data_t handler_data;
	sebs_parser_data_t parser_data;

	parser_data.handler_init=true;
	parser_data.handler_function=&xmlrpc;
	parser_data.handler_data=&handler_data;


	for(rlen=0;msg[rlen]!=0;rlen++);
	printf("Test Message Length: %i\n",rlen);

	int i;
	for(i=0;rlen-i*buffersize+buffersize>0;i++)
	{
		int len=buffersize;
		if(i*buffersize>rlen)
		{
			len=len-(i*buffersize-rlen);
		}
	//	printf("Current Chunk %i, Size %i: \n",i, len);
	//	xmlrpc_parse(msg+i*len,len,&pact);
		sebs_parser_frame(msg+i*len,len,&parser_data);
	}

	return (0);
}

// ///////////////////////
//     ROSTCP Testing   //
// ///////////////////////
#include <rosc/com/ros_handler.h>

char add_two_ints_rosrpc_query[] = {
//Message length
0x48, 0x00, 0x00, 0x00,

//Length of field
0x15, 0x00, 0x00, 0x00,

//service=/add_two_ints
0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x3d,
0x2f, 0x61, 0x64, 0x64, 0x5f, 0x74, 0x77, 0x6f,
0x5f, 0x69, 0x6e, 0x74, 0x73,

//length of field
0x07, 0x00, 0x00, 0x00,

//probe=1
0x70, 0x72, 0x6f, 0x62, 0x65, 0x3d, 0x31,

//length of field
0x14, 0x00, 0x00, 0x00,

//callerid=/rosservice
0x63, 0x61, 0x6c, 0x6c, 0x65, 0x72, 0x69, 0x64,
0x3d, 0x2f, 0x72, 0x6f, 0x73, 0x73, 0x65, 0x72,
0x76, 0x69, 0x63, 0x65,

//length of field
0x08, 0x00, 0x00, 0x00,

//md5sum=*
0x6d, 0x64, 0x35, 0x73,
0x75, 0x6d, 0x3d, 0x2a
};

char add_two_ints_rosrpc_response[] = {
//Message length
0xde, 0x00, 0x00, 0x00,

//Length of field
0x1d, 0x00, 0x00, 0x00,

//callerid=/add_two_ints_server
0x63, 0x61, 0x6c, 0x6c, 0x65, 0x72, 0x69, 0x64,
0x3d, 0x2f, 0x61, 0x64, 0x64, 0x5f, 0x74, 0x77,
0x6f, 0x5f, 0x69, 0x6e, 0x74, 0x73, 0x5f, 0x73,
0x65, 0x72, 0x76, 0x65, 0x72,

//Length of field
0x27, 0x00, 0x00, 0x00,
//md5sum=6a2e34150c00229791cc89ff309fff211
0x6d, 0x64, 0x35, 0x73, 0x75, 0x6d, 0x3d,
0x36, 0x61, 0x32, 0x65, 0x33, 0x34, 0x31, 0x35,
0x30, 0x63, 0x30, 0x30, 0x32, 0x32, 0x39, 0x37,
0x39, 0x31, 0x63, 0x63, 0x38, 0x39, 0x66, 0x66,
0x33, 0x30, 0x39, 0x66, 0x66, 0x66, 0x32, 0x31,

//Length of field
0x31, 0x00, 0x00, 0x00,
//request_type=beginner_tutorials/AddTwoIntsRequest
0x72, 0x65, 0x71, 0x75,
0x65, 0x73, 0x74, 0x5f, 0x74, 0x79, 0x70, 0x65,
0x3d, 0x62, 0x65, 0x67, 0x69, 0x6e, 0x6e, 0x65,
0x72, 0x5f, 0x74, 0x75, 0x74, 0x6f, 0x72, 0x69,
0x61, 0x6c, 0x73, 0x2f, 0x41, 0x64, 0x64, 0x54,
0x77, 0x6f, 0x49, 0x6e, 0x74, 0x73, 0x52, 0x65,
0x71, 0x75, 0x65, 0x73, 0x74,

//Length of field
0x33, 0x00, 0x00, 0x00,
//response_type=beginner_tutorials/AddTwoIntsResponse
0x72, 0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73,
0x65, 0x5f, 0x74, 0x79, 0x70, 0x65, 0x3d, 0x62,
0x65, 0x67, 0x69, 0x6e, 0x6e, 0x65, 0x72, 0x5f,
0x74, 0x75, 0x74, 0x6f, 0x72, 0x69, 0x61, 0x6c,
0x73, 0x2f, 0x41, 0x64, 0x64, 0x54, 0x77, 0x6f,
0x49, 0x6e, 0x74, 0x73, 0x52, 0x65, 0x73, 0x70,
0x6f, 0x6e, 0x73, 0x65,

//Length of field
0x22, 0x00, 0x00, 0x00,
//type=beginner_tutorials/AddTwoInts
0x74, 0x79, 0x70, 0x65, 0x3d, 0x62, 0x65, 0x67,
0x69, 0x6e, 0x6e, 0x65, 0x72, 0x5f, 0x74, 0x75,
0x74, 0x6f, 0x72, 0x69, 0x61, 0x6c, 0x73, 0x2f,
0x41, 0x64, 0x64, 0x54, 0x77, 0x6f, 0x49, 0x6e,
0x74, 0x73 };

/*
rostopic pub /test std_msgs/Int8MultiArray """
layout:
  dim: [{label: 'label', size: 5, stride: 1},{label: 'gnampf', size: 6,
stride: 7} ]
  data_offset: 0
data: [1, 2, 3, 4]
""" -r 1
*/

char peer0_0[] = {
0x7c, 0x06, 0x00, 0x00, 0xe3, 0x05, 0x00, 0x00,
0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x5f,
0x64, 0x65, 0x66, 0x69, 0x6e, 0x69, 0x74, 0x69,
0x6f, 0x6e, 0x3d, 0x23, 0x20, 0x50, 0x6c, 0x65,
0x61, 0x73, 0x65, 0x20, 0x6c, 0x6f, 0x6f, 0x6b,
0x20, 0x61, 0x74, 0x20, 0x74, 0x68, 0x65, 0x20,
0x4d, 0x75, 0x6c, 0x74, 0x69, 0x41, 0x72, 0x72,
0x61, 0x79, 0x4c, 0x61, 0x79, 0x6f, 0x75, 0x74,
0x20, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65,
0x20, 0x64, 0x65, 0x66, 0x69, 0x6e, 0x69, 0x74,
0x69, 0x6f, 0x6e, 0x20, 0x66, 0x6f, 0x72, 0x0a,
0x23, 0x20, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65,
0x6e, 0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20,
0x6f, 0x6e, 0x20, 0x61, 0x6c, 0x6c, 0x20, 0x6d,
0x75, 0x6c, 0x74, 0x69, 0x61, 0x72, 0x72, 0x61,
0x79, 0x73, 0x2e, 0x0a, 0x0a, 0x4d, 0x75, 0x6c,
0x74, 0x69, 0x41, 0x72, 0x72, 0x61, 0x79, 0x4c,
0x61, 0x79, 0x6f, 0x75, 0x74, 0x20, 0x20, 0x6c,
0x61, 0x79, 0x6f, 0x75, 0x74, 0x20, 0x20, 0x20,
0x20, 0x20, 0x20, 0x20, 0x20, 0x23, 0x20, 0x73,
0x70, 0x65, 0x63, 0x69, 0x66, 0x69, 0x63, 0x61,
0x74, 0x69, 0x6f, 0x6e, 0x20, 0x6f, 0x66, 0x20,
0x64, 0x61, 0x74, 0x61, 0x20, 0x6c, 0x61, 0x79,
0x6f, 0x75, 0x74, 0x0a, 0x69, 0x6e, 0x74, 0x38,
0x5b, 0x5d, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x64, 0x61,
0x74, 0x61, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
0x20, 0x20, 0x20, 0x20, 0x23, 0x20, 0x61, 0x72,
0x72, 0x61, 0x79, 0x20, 0x6f, 0x66, 0x20, 0x64,
0x61, 0x74, 0x61, 0x0a, 0x0a, 0x0a, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x0a, 0x4d,
0x53, 0x47, 0x3a, 0x20, 0x73, 0x74, 0x64, 0x5f,
0x6d, 0x73, 0x67, 0x73, 0x2f, 0x4d, 0x75, 0x6c,
0x74, 0x69, 0x41, 0x72, 0x72, 0x61, 0x79, 0x4c,
0x61, 0x79, 0x6f, 0x75, 0x74, 0x0a, 0x23, 0x20,
0x54, 0x68, 0x65, 0x20, 0x6d, 0x75, 0x6c, 0x74,
0x69, 0x61, 0x72, 0x72, 0x61, 0x79, 0x20, 0x64,
0x65, 0x63, 0x6c, 0x61, 0x72, 0x65, 0x73, 0x20,
0x61, 0x20, 0x67, 0x65, 0x6e, 0x65, 0x72, 0x69,
0x63, 0x20, 0x6d, 0x75, 0x6c, 0x74, 0x69, 0x2d,
0x64, 0x69, 0x6d, 0x65, 0x6e, 0x73, 0x69, 0x6f,
0x6e, 0x61, 0x6c, 0x20, 0x61, 0x72, 0x72, 0x61,
0x79, 0x20, 0x6f, 0x66, 0x20, 0x61, 0x0a, 0x23,
0x20, 0x70, 0x61, 0x72, 0x74, 0x69, 0x63, 0x75,
0x6c, 0x61, 0x72, 0x20, 0x64, 0x61, 0x74, 0x61,
0x20, 0x74, 0x79, 0x70, 0x65, 0x2e, 0x20, 0x20,
0x44, 0x69, 0x6d, 0x65, 0x6e, 0x73, 0x69, 0x6f,
0x6e, 0x73, 0x20, 0x61, 0x72, 0x65, 0x20, 0x6f,
0x72, 0x64, 0x65, 0x72, 0x65, 0x64, 0x20, 0x66,
0x72, 0x6f, 0x6d, 0x20, 0x6f, 0x75, 0x74, 0x65,
0x72, 0x20, 0x6d, 0x6f, 0x73, 0x74, 0x0a, 0x23,
0x20, 0x74, 0x6f, 0x20, 0x69, 0x6e, 0x6e, 0x65,
0x72, 0x20, 0x6d, 0x6f, 0x73, 0x74, 0x2e, 0x0a,
0x0a, 0x4d, 0x75, 0x6c, 0x74, 0x69, 0x41, 0x72,
0x72, 0x61, 0x79, 0x44, 0x69, 0x6d, 0x65, 0x6e,
0x73, 0x69, 0x6f, 0x6e, 0x5b, 0x5d, 0x20, 0x64,
0x69, 0x6d, 0x20, 0x23, 0x20, 0x41, 0x72, 0x72,
0x61, 0x79, 0x20, 0x6f, 0x66, 0x20, 0x64, 0x69,
0x6d, 0x65, 0x6e, 0x73, 0x69, 0x6f, 0x6e, 0x20,
0x70, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x69,
0x65, 0x73, 0x0a, 0x75, 0x69, 0x6e, 0x74, 0x33,
0x32, 0x20, 0x64, 0x61, 0x74, 0x61, 0x5f, 0x6f,
0x66, 0x66, 0x73, 0x65, 0x74, 0x20, 0x20, 0x20,
0x20, 0x20, 0x20, 0x20, 0x20, 0x23, 0x20, 0x70,
0x61, 0x64, 0x64, 0x69, 0x6e, 0x67, 0x20, 0x62,
0x79, 0x74, 0x65, 0x73, 0x20, 0x61, 0x74, 0x20,
0x66, 0x72, 0x6f, 0x6e, 0x74, 0x20, 0x6f, 0x66,
0x20, 0x64, 0x61, 0x74, 0x61, 0x0a, 0x0a, 0x23,
0x20, 0x41, 0x63, 0x63, 0x65, 0x73, 0x73, 0x6f,
0x72, 0x73, 0x20, 0x73, 0x68, 0x6f, 0x75, 0x6c,
0x64, 0x20, 0x41, 0x4c, 0x57, 0x41, 0x59, 0x53,
0x20, 0x62, 0x65, 0x20, 0x77, 0x72, 0x69, 0x74,
0x74, 0x65, 0x6e, 0x20, 0x69, 0x6e, 0x20, 0x74,
0x65, 0x72, 0x6d, 0x73, 0x20, 0x6f, 0x66, 0x20,
0x64, 0x69, 0x6d, 0x65, 0x6e, 0x73, 0x69, 0x6f,
0x6e, 0x20, 0x73, 0x74, 0x72, 0x69, 0x64, 0x65,
0x0a, 0x23, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x73,
0x70, 0x65, 0x63, 0x69, 0x66, 0x69, 0x65, 0x64,
0x20, 0x6f, 0x75, 0x74, 0x65, 0x72, 0x2d, 0x6d,
0x6f, 0x73, 0x74, 0x20, 0x64, 0x69, 0x6d, 0x65,
0x6e, 0x73, 0x69, 0x6f, 0x6e, 0x20, 0x66, 0x69,
0x72, 0x73, 0x74, 0x2e, 0x0a, 0x23, 0x20, 0x0a,
0x23, 0x20, 0x6d, 0x75, 0x6c, 0x74, 0x69, 0x61,
0x72, 0x72, 0x61, 0x79, 0x28, 0x69, 0x2c, 0x6a,
0x2c, 0x6b, 0x29, 0x20, 0x3d, 0x20, 0x64, 0x61,
0x74, 0x61, 0x5b, 0x64, 0x61, 0x74, 0x61, 0x5f,
0x6f, 0x66, 0x66, 0x73, 0x65, 0x74, 0x20, 0x2b,
0x20, 0x64, 0x69, 0x6d, 0x5f, 0x73, 0x74, 0x72,
0x69, 0x64, 0x65, 0x5b, 0x31, 0x5d, 0x2a, 0x69,
0x20, 0x2b, 0x20, 0x64, 0x69, 0x6d, 0x5f, 0x73,
0x74, 0x72, 0x69, 0x64, 0x65, 0x5b, 0x32, 0x5d,
0x2a, 0x6a, 0x20, 0x2b, 0x20, 0x6b, 0x5d, 0x0a,
0x23, 0x0a, 0x23, 0x20, 0x41, 0x20, 0x73, 0x74,
0x61, 0x6e, 0x64, 0x61, 0x72, 0x64, 0x2c, 0x20,
0x33, 0x2d, 0x63, 0x68, 0x61, 0x6e, 0x6e, 0x65,
0x6c, 0x20, 0x36, 0x34, 0x30, 0x78, 0x34, 0x38,
0x30, 0x20, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x20,
0x77, 0x69, 0x74, 0x68, 0x20, 0x69, 0x6e, 0x74,
0x65, 0x72, 0x6c, 0x65, 0x61, 0x76, 0x65, 0x64,
0x20, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x63,
0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c, 0x73, 0x0a,
0x23, 0x20, 0x77, 0x6f, 0x75, 0x6c, 0x64, 0x20,
0x62, 0x65, 0x20, 0x73, 0x70, 0x65, 0x63, 0x69,
0x66, 0x69, 0x65, 0x64, 0x20, 0x61, 0x73, 0x3a,
0x0a, 0x23, 0x0a, 0x23, 0x20, 0x64, 0x69, 0x6d,
0x5b, 0x30, 0x5d, 0x2e, 0x6c, 0x61, 0x62, 0x65,
0x6c, 0x20, 0x20, 0x3d, 0x20, 0x22, 0x68, 0x65,
0x69, 0x67, 0x68, 0x74, 0x22, 0x0a, 0x23, 0x20,
0x64, 0x69, 0x6d, 0x5b, 0x30, 0x5d, 0x2e, 0x73,
0x69, 0x7a, 0x65, 0x20, 0x20, 0x20, 0x3d, 0x20,
0x34, 0x38, 0x30, 0x0a, 0x23, 0x20, 0x64, 0x69,
0x6d, 0x5b, 0x30, 0x5d, 0x2e, 0x73, 0x74, 0x72,
0x69, 0x64, 0x65, 0x20, 0x3d, 0x20, 0x33, 0x2a,
0x36, 0x34, 0x30, 0x2a, 0x34, 0x38, 0x30, 0x20,
0x3d, 0x20, 0x39, 0x32, 0x31, 0x36, 0x30, 0x30,
0x20, 0x20, 0x28, 0x6e, 0x6f, 0x74, 0x65, 0x20,
0x64, 0x69, 0x6d, 0x5b, 0x30, 0x5d, 0x20, 0x73,
0x74, 0x72, 0x69, 0x64, 0x65, 0x20, 0x69, 0x73,
0x20, 0x6a, 0x75, 0x73, 0x74, 0x20, 0x73, 0x69,
0x7a, 0x65, 0x20, 0x6f, 0x66, 0x20, 0x69, 0x6d,
0x61, 0x67, 0x65, 0x29, 0x0a, 0x23, 0x20, 0x64,
0x69, 0x6d, 0x5b, 0x31, 0x5d, 0x2e, 0x6c, 0x61,
0x62, 0x65, 0x6c, 0x20, 0x20, 0x3d, 0x20, 0x22,
0x77, 0x69, 0x64, 0x74, 0x68, 0x22, 0x0a, 0x23,
0x20, 0x64, 0x69, 0x6d, 0x5b, 0x31, 0x5d, 0x2e,
0x73, 0x69, 0x7a, 0x65, 0x20, 0x20, 0x20, 0x3d,
0x20, 0x36, 0x34, 0x30, 0x0a, 0x23, 0x20, 0x64,
0x69, 0x6d, 0x5b, 0x31, 0x5d, 0x2e, 0x73, 0x74,
0x72, 0x69, 0x64, 0x65, 0x20, 0x3d, 0x20, 0x33,
0x2a, 0x36, 0x34, 0x30, 0x20, 0x3d, 0x20, 0x31,
0x39, 0x32, 0x30, 0x0a, 0x23, 0x20, 0x64, 0x69,
0x6d, 0x5b, 0x32, 0x5d, 0x2e, 0x6c, 0x61, 0x62,
0x65, 0x6c, 0x20, 0x20, 0x3d, 0x20, 0x22, 0x63,
0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c, 0x22, 0x0a,
0x23, 0x20, 0x64, 0x69, 0x6d, 0x5b, 0x32, 0x5d,
0x2e, 0x73, 0x69, 0x7a, 0x65, 0x20, 0x20, 0x20,
0x3d, 0x20, 0x33, 0x0a, 0x23, 0x20, 0x64, 0x69,
0x6d, 0x5b, 0x32, 0x5d, 0x2e, 0x73, 0x74, 0x72,
0x69, 0x64, 0x65, 0x20, 0x3d, 0x20, 0x33, 0x0a,
0x23, 0x0a, 0x23, 0x20, 0x6d, 0x75, 0x6c, 0x74,
0x69, 0x61, 0x72, 0x72, 0x61, 0x79, 0x28, 0x69,
0x2c, 0x6a, 0x2c, 0x6b, 0x29, 0x20, 0x72, 0x65,
0x66, 0x65, 0x72, 0x73, 0x20, 0x74, 0x6f, 0x20,
0x74, 0x68, 0x65, 0x20, 0x69, 0x74, 0x68, 0x20,
0x72, 0x6f, 0x77, 0x2c, 0x20, 0x6a, 0x74, 0x68,
0x20, 0x63, 0x6f, 0x6c, 0x75, 0x6d, 0x6e, 0x2c,
0x20, 0x61, 0x6e, 0x64, 0x20, 0x6b, 0x74, 0x68,
0x20, 0x63, 0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c,
0x2e, 0x0a, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x0a, 0x4d, 0x53, 0x47, 0x3a, 0x20,
0x73, 0x74, 0x64, 0x5f, 0x6d, 0x73, 0x67, 0x73,
0x2f, 0x4d, 0x75, 0x6c, 0x74, 0x69, 0x41, 0x72,
0x72, 0x61, 0x79, 0x44, 0x69, 0x6d, 0x65, 0x6e,
0x73, 0x69, 0x6f, 0x6e, 0x0a, 0x73, 0x74, 0x72,
0x69, 0x6e, 0x67, 0x20, 0x6c, 0x61, 0x62, 0x65,
0x6c, 0x20, 0x20, 0x20, 0x23, 0x20, 0x6c, 0x61,
0x62, 0x65, 0x6c, 0x20, 0x6f, 0x66, 0x20, 0x67,
0x69, 0x76, 0x65, 0x6e, 0x20, 0x64, 0x69, 0x6d,
0x65, 0x6e, 0x73, 0x69, 0x6f, 0x6e, 0x0a, 0x75,
0x69, 0x6e, 0x74, 0x33, 0x32, 0x20, 0x73, 0x69,
0x7a, 0x65, 0x20, 0x20, 0x20, 0x20, 0x23, 0x20,
0x73, 0x69, 0x7a, 0x65, 0x20, 0x6f, 0x66, 0x20,
0x67, 0x69, 0x76, 0x65, 0x6e, 0x20, 0x64, 0x69,
0x6d, 0x65, 0x6e, 0x73, 0x69, 0x6f, 0x6e, 0x20,
0x28, 0x69, 0x6e, 0x20, 0x74, 0x79, 0x70, 0x65,
0x20, 0x75, 0x6e, 0x69, 0x74, 0x73, 0x29, 0x0a,
0x75, 0x69, 0x6e, 0x74, 0x33, 0x32, 0x20, 0x73,
0x74, 0x72, 0x69, 0x64, 0x65, 0x20, 0x20, 0x23,
0x20, 0x73, 0x74, 0x72, 0x69, 0x64, 0x65, 0x20,
0x6f, 0x66, 0x20, 0x67, 0x69, 0x76, 0x65, 0x6e,
0x20, 0x64, 0x69, 0x6d, 0x65, 0x6e, 0x73, 0x69,
0x6f, 0x6e, 0x0a, 0x26, 0x00, 0x00, 0x00, 0x63,
0x61, 0x6c, 0x6c, 0x65, 0x72, 0x69, 0x64, 0x3d,
0x2f, 0x72, 0x6f, 0x73, 0x74, 0x6f, 0x70, 0x69,
0x63, 0x5f, 0x31, 0x33, 0x39, 0x39, 0x32, 0x5f,
0x31, 0x33, 0x37, 0x30, 0x39, 0x36, 0x32, 0x30,
0x34, 0x39, 0x37, 0x36, 0x38, 0x0d, 0x00, 0x00,
0x00, 0x74, 0x63, 0x70, 0x5f, 0x6e, 0x6f, 0x64,
0x65, 0x6c, 0x61, 0x79, 0x3d, 0x30, 0x27, 0x00,
0x00, 0x00, 0x6d, 0x64, 0x35, 0x73, 0x75, 0x6d,
0x3d, 0x64, 0x37, 0x63, 0x31, 0x61, 0x66, 0x33,
0x35, 0x61, 0x31, 0x62, 0x34, 0x37, 0x38, 0x31,
0x62, 0x62, 0x65, 0x37, 0x39, 0x65, 0x30, 0x33,
0x64, 0x64, 0x39, 0x34, 0x62, 0x37, 0x63, 0x31,
0x33, 0x0b, 0x00, 0x00, 0x00, 0x74, 0x6f, 0x70,
0x69, 0x63, 0x3d, 0x2f, 0x74, 0x65, 0x73, 0x74,
0x1c, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65,
0x3d, 0x73, 0x74, 0x64, 0x5f, 0x6d, 0x73, 0x67,
0x73, 0x2f, 0x49, 0x6e, 0x74, 0x38, 0x4d, 0x75,
0x6c, 0x74, 0x69, 0x41, 0x72, 0x72, 0x61, 0x79 };


char peer1_0[] = {
0x79, 0x06, 0x00, 0x00, 0xe3, 0x05, 0x00, 0x00,
0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x5f,
0x64, 0x65, 0x66, 0x69, 0x6e, 0x69, 0x74, 0x69,
0x6f, 0x6e, 0x3d, 0x23, 0x20, 0x50, 0x6c, 0x65,
0x61, 0x73, 0x65, 0x20, 0x6c, 0x6f, 0x6f, 0x6b,
0x20, 0x61, 0x74, 0x20, 0x74, 0x68, 0x65, 0x20,
0x4d, 0x75, 0x6c, 0x74, 0x69, 0x41, 0x72, 0x72,
0x61, 0x79, 0x4c, 0x61, 0x79, 0x6f, 0x75, 0x74,
0x20, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65,
0x20, 0x64, 0x65, 0x66, 0x69, 0x6e, 0x69, 0x74,
0x69, 0x6f, 0x6e, 0x20, 0x66, 0x6f, 0x72, 0x0a,
0x23, 0x20, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65,
0x6e, 0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20,
0x6f, 0x6e, 0x20, 0x61, 0x6c, 0x6c, 0x20, 0x6d,
0x75, 0x6c, 0x74, 0x69, 0x61, 0x72, 0x72, 0x61,
0x79, 0x73, 0x2e, 0x0a, 0x0a, 0x4d, 0x75, 0x6c,
0x74, 0x69, 0x41, 0x72, 0x72, 0x61, 0x79, 0x4c,
0x61, 0x79, 0x6f, 0x75, 0x74, 0x20, 0x20, 0x6c,
0x61, 0x79, 0x6f, 0x75, 0x74, 0x20, 0x20, 0x20,
0x20, 0x20, 0x20, 0x20, 0x20, 0x23, 0x20, 0x73,
0x70, 0x65, 0x63, 0x69, 0x66, 0x69, 0x63, 0x61,
0x74, 0x69, 0x6f, 0x6e, 0x20, 0x6f, 0x66, 0x20,
0x64, 0x61, 0x74, 0x61, 0x20, 0x6c, 0x61, 0x79,
0x6f, 0x75, 0x74, 0x0a, 0x69, 0x6e, 0x74, 0x38,
0x5b, 0x5d, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x64, 0x61,
0x74, 0x61, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
0x20, 0x20, 0x20, 0x20, 0x23, 0x20, 0x61, 0x72,
0x72, 0x61, 0x79, 0x20, 0x6f, 0x66, 0x20, 0x64,
0x61, 0x74, 0x61, 0x0a, 0x0a, 0x0a, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x0a, 0x4d,
0x53, 0x47, 0x3a, 0x20, 0x73, 0x74, 0x64, 0x5f,
0x6d, 0x73, 0x67, 0x73, 0x2f, 0x4d, 0x75, 0x6c,
0x74, 0x69, 0x41, 0x72, 0x72, 0x61, 0x79, 0x4c,
0x61, 0x79, 0x6f, 0x75, 0x74, 0x0a, 0x23, 0x20,
0x54, 0x68, 0x65, 0x20, 0x6d, 0x75, 0x6c, 0x74,
0x69, 0x61, 0x72, 0x72, 0x61, 0x79, 0x20, 0x64,
0x65, 0x63, 0x6c, 0x61, 0x72, 0x65, 0x73, 0x20,
0x61, 0x20, 0x67, 0x65, 0x6e, 0x65, 0x72, 0x69,
0x63, 0x20, 0x6d, 0x75, 0x6c, 0x74, 0x69, 0x2d,
0x64, 0x69, 0x6d, 0x65, 0x6e, 0x73, 0x69, 0x6f,
0x6e, 0x61, 0x6c, 0x20, 0x61, 0x72, 0x72, 0x61,
0x79, 0x20, 0x6f, 0x66, 0x20, 0x61, 0x0a, 0x23,
0x20, 0x70, 0x61, 0x72, 0x74, 0x69, 0x63, 0x75,
0x6c, 0x61, 0x72, 0x20, 0x64, 0x61, 0x74, 0x61,
0x20, 0x74, 0x79, 0x70, 0x65, 0x2e, 0x20, 0x20,
0x44, 0x69, 0x6d, 0x65, 0x6e, 0x73, 0x69, 0x6f,
0x6e, 0x73, 0x20, 0x61, 0x72, 0x65, 0x20, 0x6f,
0x72, 0x64, 0x65, 0x72, 0x65, 0x64, 0x20, 0x66,
0x72, 0x6f, 0x6d, 0x20, 0x6f, 0x75, 0x74, 0x65,
0x72, 0x20, 0x6d, 0x6f, 0x73, 0x74, 0x0a, 0x23,
0x20, 0x74, 0x6f, 0x20, 0x69, 0x6e, 0x6e, 0x65,
0x72, 0x20, 0x6d, 0x6f, 0x73, 0x74, 0x2e, 0x0a,
0x0a, 0x4d, 0x75, 0x6c, 0x74, 0x69, 0x41, 0x72,
0x72, 0x61, 0x79, 0x44, 0x69, 0x6d, 0x65, 0x6e,
0x73, 0x69, 0x6f, 0x6e, 0x5b, 0x5d, 0x20, 0x64,
0x69, 0x6d, 0x20, 0x23, 0x20, 0x41, 0x72, 0x72,
0x61, 0x79, 0x20, 0x6f, 0x66, 0x20, 0x64, 0x69,
0x6d, 0x65, 0x6e, 0x73, 0x69, 0x6f, 0x6e, 0x20,
0x70, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x69,
0x65, 0x73, 0x0a, 0x75, 0x69, 0x6e, 0x74, 0x33,
0x32, 0x20, 0x64, 0x61, 0x74, 0x61, 0x5f, 0x6f,
0x66, 0x66, 0x73, 0x65, 0x74, 0x20, 0x20, 0x20,
0x20, 0x20, 0x20, 0x20, 0x20, 0x23, 0x20, 0x70,
0x61, 0x64, 0x64, 0x69, 0x6e, 0x67, 0x20, 0x62,
0x79, 0x74, 0x65, 0x73, 0x20, 0x61, 0x74, 0x20,
0x66, 0x72, 0x6f, 0x6e, 0x74, 0x20, 0x6f, 0x66,
0x20, 0x64, 0x61, 0x74, 0x61, 0x0a, 0x0a, 0x23,
0x20, 0x41, 0x63, 0x63, 0x65, 0x73, 0x73, 0x6f,
0x72, 0x73, 0x20, 0x73, 0x68, 0x6f, 0x75, 0x6c,
0x64, 0x20, 0x41, 0x4c, 0x57, 0x41, 0x59, 0x53,
0x20, 0x62, 0x65, 0x20, 0x77, 0x72, 0x69, 0x74,
0x74, 0x65, 0x6e, 0x20, 0x69, 0x6e, 0x20, 0x74,
0x65, 0x72, 0x6d, 0x73, 0x20, 0x6f, 0x66, 0x20,
0x64, 0x69, 0x6d, 0x65, 0x6e, 0x73, 0x69, 0x6f,
0x6e, 0x20, 0x73, 0x74, 0x72, 0x69, 0x64, 0x65,
0x0a, 0x23, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x73,
0x70, 0x65, 0x63, 0x69, 0x66, 0x69, 0x65, 0x64,
0x20, 0x6f, 0x75, 0x74, 0x65, 0x72, 0x2d, 0x6d,
0x6f, 0x73, 0x74, 0x20, 0x64, 0x69, 0x6d, 0x65,
0x6e, 0x73, 0x69, 0x6f, 0x6e, 0x20, 0x66, 0x69,
0x72, 0x73, 0x74, 0x2e, 0x0a, 0x23, 0x20, 0x0a,
0x23, 0x20, 0x6d, 0x75, 0x6c, 0x74, 0x69, 0x61,
0x72, 0x72, 0x61, 0x79, 0x28, 0x69, 0x2c, 0x6a,
0x2c, 0x6b, 0x29, 0x20, 0x3d, 0x20, 0x64, 0x61,
0x74, 0x61, 0x5b, 0x64, 0x61, 0x74, 0x61, 0x5f,
0x6f, 0x66, 0x66, 0x73, 0x65, 0x74, 0x20, 0x2b,
0x20, 0x64, 0x69, 0x6d, 0x5f, 0x73, 0x74, 0x72,
0x69, 0x64, 0x65, 0x5b, 0x31, 0x5d, 0x2a, 0x69,
0x20, 0x2b, 0x20, 0x64, 0x69, 0x6d, 0x5f, 0x73,
0x74, 0x72, 0x69, 0x64, 0x65, 0x5b, 0x32, 0x5d,
0x2a, 0x6a, 0x20, 0x2b, 0x20, 0x6b, 0x5d, 0x0a,
0x23, 0x0a, 0x23, 0x20, 0x41, 0x20, 0x73, 0x74,
0x61, 0x6e, 0x64, 0x61, 0x72, 0x64, 0x2c, 0x20,
0x33, 0x2d, 0x63, 0x68, 0x61, 0x6e, 0x6e, 0x65,
0x6c, 0x20, 0x36, 0x34, 0x30, 0x78, 0x34, 0x38,
0x30, 0x20, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x20,
0x77, 0x69, 0x74, 0x68, 0x20, 0x69, 0x6e, 0x74,
0x65, 0x72, 0x6c, 0x65, 0x61, 0x76, 0x65, 0x64,
0x20, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x63,
0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c, 0x73, 0x0a,
0x23, 0x20, 0x77, 0x6f, 0x75, 0x6c, 0x64, 0x20,
0x62, 0x65, 0x20, 0x73, 0x70, 0x65, 0x63, 0x69,
0x66, 0x69, 0x65, 0x64, 0x20, 0x61, 0x73, 0x3a,
0x0a, 0x23, 0x0a, 0x23, 0x20, 0x64, 0x69, 0x6d,
0x5b, 0x30, 0x5d, 0x2e, 0x6c, 0x61, 0x62, 0x65,
0x6c, 0x20, 0x20, 0x3d, 0x20, 0x22, 0x68, 0x65,
0x69, 0x67, 0x68, 0x74, 0x22, 0x0a, 0x23, 0x20,
0x64, 0x69, 0x6d, 0x5b, 0x30, 0x5d, 0x2e, 0x73,
0x69, 0x7a, 0x65, 0x20, 0x20, 0x20, 0x3d, 0x20,
0x34, 0x38, 0x30, 0x0a, 0x23, 0x20, 0x64, 0x69,
0x6d, 0x5b, 0x30, 0x5d, 0x2e, 0x73, 0x74, 0x72,
0x69, 0x64, 0x65, 0x20, 0x3d, 0x20, 0x33, 0x2a,
0x36, 0x34, 0x30, 0x2a, 0x34, 0x38, 0x30, 0x20,
0x3d, 0x20, 0x39, 0x32, 0x31, 0x36, 0x30, 0x30,
0x20, 0x20, 0x28, 0x6e, 0x6f, 0x74, 0x65, 0x20,
0x64, 0x69, 0x6d, 0x5b, 0x30, 0x5d, 0x20, 0x73,
0x74, 0x72, 0x69, 0x64, 0x65, 0x20, 0x69, 0x73,
0x20, 0x6a, 0x75, 0x73, 0x74, 0x20, 0x73, 0x69,
0x7a, 0x65, 0x20, 0x6f, 0x66, 0x20, 0x69, 0x6d,
0x61, 0x67, 0x65, 0x29, 0x0a, 0x23, 0x20, 0x64,
0x69, 0x6d, 0x5b, 0x31, 0x5d, 0x2e, 0x6c, 0x61,
0x62, 0x65, 0x6c, 0x20, 0x20, 0x3d, 0x20, 0x22,
0x77, 0x69, 0x64, 0x74, 0x68, 0x22, 0x0a, 0x23,
0x20, 0x64, 0x69, 0x6d, 0x5b, 0x31, 0x5d, 0x2e,
0x73, 0x69, 0x7a, 0x65, 0x20, 0x20, 0x20, 0x3d,
0x20, 0x36, 0x34, 0x30, 0x0a, 0x23, 0x20, 0x64,
0x69, 0x6d, 0x5b, 0x31, 0x5d, 0x2e, 0x73, 0x74,
0x72, 0x69, 0x64, 0x65, 0x20, 0x3d, 0x20, 0x33,
0x2a, 0x36, 0x34, 0x30, 0x20, 0x3d, 0x20, 0x31,
0x39, 0x32, 0x30, 0x0a, 0x23, 0x20, 0x64, 0x69,
0x6d, 0x5b, 0x32, 0x5d, 0x2e, 0x6c, 0x61, 0x62,
0x65, 0x6c, 0x20, 0x20, 0x3d, 0x20, 0x22, 0x63,
0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c, 0x22, 0x0a,
0x23, 0x20, 0x64, 0x69, 0x6d, 0x5b, 0x32, 0x5d,
0x2e, 0x73, 0x69, 0x7a, 0x65, 0x20, 0x20, 0x20,
0x3d, 0x20, 0x33, 0x0a, 0x23, 0x20, 0x64, 0x69,
0x6d, 0x5b, 0x32, 0x5d, 0x2e, 0x73, 0x74, 0x72,
0x69, 0x64, 0x65, 0x20, 0x3d, 0x20, 0x33, 0x0a,
0x23, 0x0a, 0x23, 0x20, 0x6d, 0x75, 0x6c, 0x74,
0x69, 0x61, 0x72, 0x72, 0x61, 0x79, 0x28, 0x69,
0x2c, 0x6a, 0x2c, 0x6b, 0x29, 0x20, 0x72, 0x65,
0x66, 0x65, 0x72, 0x73, 0x20, 0x74, 0x6f, 0x20,
0x74, 0x68, 0x65, 0x20, 0x69, 0x74, 0x68, 0x20,
0x72, 0x6f, 0x77, 0x2c, 0x20, 0x6a, 0x74, 0x68,
0x20, 0x63, 0x6f, 0x6c, 0x75, 0x6d, 0x6e, 0x2c,
0x20, 0x61, 0x6e, 0x64, 0x20, 0x6b, 0x74, 0x68,
0x20, 0x63, 0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c,
0x2e, 0x0a, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
0x3d, 0x3d, 0x0a, 0x4d, 0x53, 0x47, 0x3a, 0x20,
0x73, 0x74, 0x64, 0x5f, 0x6d, 0x73, 0x67, 0x73,
0x2f, 0x4d, 0x75, 0x6c, 0x74, 0x69, 0x41, 0x72,
0x72, 0x61, 0x79, 0x44, 0x69, 0x6d, 0x65, 0x6e,
0x73, 0x69, 0x6f, 0x6e, 0x0a, 0x73, 0x74, 0x72,
0x69, 0x6e, 0x67, 0x20, 0x6c, 0x61, 0x62, 0x65,
0x6c, 0x20, 0x20, 0x20, 0x23, 0x20, 0x6c, 0x61,
0x62, 0x65, 0x6c, 0x20, 0x6f, 0x66, 0x20, 0x67,
0x69, 0x76, 0x65, 0x6e, 0x20, 0x64, 0x69, 0x6d,
0x65, 0x6e, 0x73, 0x69, 0x6f, 0x6e, 0x0a, 0x75,
0x69, 0x6e, 0x74, 0x33, 0x32, 0x20, 0x73, 0x69,
0x7a, 0x65, 0x20, 0x20, 0x20, 0x20, 0x23, 0x20,
0x73, 0x69, 0x7a, 0x65, 0x20, 0x6f, 0x66, 0x20,
0x67, 0x69, 0x76, 0x65, 0x6e, 0x20, 0x64, 0x69,
0x6d, 0x65, 0x6e, 0x73, 0x69, 0x6f, 0x6e, 0x20,
0x28, 0x69, 0x6e, 0x20, 0x74, 0x79, 0x70, 0x65,
0x20, 0x75, 0x6e, 0x69, 0x74, 0x73, 0x29, 0x0a,
0x75, 0x69, 0x6e, 0x74, 0x33, 0x32, 0x20, 0x73,
0x74, 0x72, 0x69, 0x64, 0x65, 0x20, 0x20, 0x23,
0x20, 0x73, 0x74, 0x72, 0x69, 0x64, 0x65, 0x20,
0x6f, 0x66, 0x20, 0x67, 0x69, 0x76, 0x65, 0x6e,
0x20, 0x64, 0x69, 0x6d, 0x65, 0x6e, 0x73, 0x69,
0x6f, 0x6e, 0x0a, 0x26, 0x00, 0x00, 0x00, 0x63,
0x61, 0x6c, 0x6c, 0x65, 0x72, 0x69, 0x64, 0x3d,
0x2f, 0x72, 0x6f, 0x73, 0x74, 0x6f, 0x70, 0x69,
0x63, 0x5f, 0x31, 0x33, 0x38, 0x31, 0x37, 0x5f,
0x31, 0x33, 0x37, 0x30, 0x39, 0x36, 0x32, 0x30,
0x30, 0x39, 0x32, 0x32, 0x39, 0x0a, 0x00, 0x00,
0x00, 0x6c, 0x61, 0x74, 0x63, 0x68, 0x69, 0x6e,
0x67, 0x3d, 0x30, 0x27, 0x00, 0x00, 0x00, 0x6d,
0x64, 0x35, 0x73, 0x75, 0x6d, 0x3d, 0x64, 0x37,
0x63, 0x31, 0x61, 0x66, 0x33, 0x35, 0x61, 0x31,
0x62, 0x34, 0x37, 0x38, 0x31, 0x62, 0x62, 0x65,
0x37, 0x39, 0x65, 0x30, 0x33, 0x64, 0x64, 0x39,
0x34, 0x62, 0x37, 0x63, 0x31, 0x33, 0x0b, 0x00,
0x00, 0x00, 0x74, 0x6f, 0x70, 0x69, 0x63, 0x3d,
0x2f, 0x74, 0x65, 0x73, 0x74, 0x1c, 0x00, 0x00,
0x00, 0x74, 0x79, 0x70, 0x65, 0x3d, 0x73, 0x74,
0x64, 0x5f, 0x6d, 0x73, 0x67, 0x73, 0x2f, 0x49,
0x6e, 0x74, 0x38, 0x4d, 0x75, 0x6c, 0x74, 0x69,
0x41, 0x72, 0x72, 0x61, 0x79 };




char MultiArrayInt8[] = {
//Whole message size
0x33, 0x00, 0x00, 0x00,
//Size dim array (elements)
0x02, 0x00, 0x00, 0x00,
//Size label (amount of chars)
0x05, 0x00, 0x00, 0x00,
//l     a     b     e     l
0x6c, 0x61, 0x62, 0x65, 0x6c,
//size var:5
0x05, 0x00, 0x00, 0x00,
//stride: 1
0x01, 0x00, 0x00, 0x00,

//Size label
0x06, 0x00, 0x00, 0x00,
//g    n      a     m    p     f
0x67, 0x6e, 0x61, 0x6d, 0x70, 0x66,
//size var:6
0x06, 0x00, 0x00, 0x00,
//stride: 7
0x07,0x00, 0x00, 0x00,
//offset
0x00, 0x00, 0x00, 0x00,

//data size (4)
0x04, 0x00, 0x00, 0x00,
//data array elements
0x01, 0x02, 0x03, 0x04 };

int main_tcprostest()
{
	int rlen;
	int buffersize=1;

#if(0)
	char *msg=add_two_ints_rosrpc_query;
	rlen=sizeof(add_two_ints_rosrpc_query);

#else
	char *msg=add_two_ints_rosrpc_response;
	rlen=sizeof(add_two_ints_rosrpc_response);
#endif


	ros_hander_data_t handler_data;
	sebs_parser_data_t parser_data;

	parser_data.handler_init=true;
	parser_data.handler_function=&ros_handler;
	parser_data.handler_data=&handler_data;


	printf("Test Message Length: %i\n",rlen);
	int i;


	for (i = 0; i < rlen; ++i) {

		if(msg[i]>=32 && msg[i]<=126)
		{
			printf("%c", msg[i]);
		}
		else
		{
			printf("[%i]", msg[i]);
		}
	}

	for(i=0;i<rlen;i++)
	{
		int len=buffersize;
		if(i*buffersize>rlen)
		{
			len=len-(i*buffersize-rlen);
		}
		sebs_parser_frame(msg+i*len,len,&parser_data);
	}

	return (0);
}


#include <rosc/sebs_parse_fw/sebs_parser_frame.h>
int main()
{
	printf("\n\nExecute --> rosc_linux_test\n\n");

	rosc_init();

	main_tcprostest();
	return (0);
}


