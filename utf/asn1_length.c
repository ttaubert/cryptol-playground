#include <stdint.h>

uint32_t der_indefinite_length(uint8_t *buf, uint32_t buf_len)
{
uint8_t* end = buf + buf_len;
uint32_t len, ret, dataLen;
unsigned char tag, lenCode;
int dataLenLen;

len = 0;
while ( 1 ) {
 if ((buf + 2) > end) {
     return(0);
 }
 
 tag = *buf++;
 lenCode = *buf++;
 len += 2;
 
 if ( ( tag == 0 ) && ( lenCode == 0 ) ) {
     return(len);
 }
 
 if ( lenCode == 0x80 ) {	/* indefinite length */
     ret = der_indefinite_length(buf, end - buf); /* recurse to find length */
     if (ret == 0)
 	return 0;
     len += ret;
     buf += ret;
 } else {			/* definite length */
     if (lenCode & 0x80) {
 	/* Length of data is in multibyte format */
 	dataLenLen = lenCode & 0x7f;
 	switch (dataLenLen) {
 	  case 1:
 	    dataLen = buf[0];
 	    break;
 	  case 2:
 	    dataLen = (buf[0]<<8)|buf[1];
 	    break;
 	  case 3:
 	    dataLen = ((unsigned long)buf[0]<<16)|(buf[1]<<8)|buf[2];
 	    break;
 	  case 4:
 	    dataLen = ((unsigned long)buf[0]<<24)|
 		((unsigned long)buf[1]<<16)|(buf[2]<<8)|buf[3];
 	    break;
 	  default:
 	    return -1; // SECFailure
 	}
     } else {
 	/* Length of data is in single byte */
 	dataLen = lenCode;
 	dataLenLen = 0;
     }

     /* skip this item */
     buf = buf + dataLenLen + dataLen;
     len = len + dataLenLen + dataLen;
 }
}
}
